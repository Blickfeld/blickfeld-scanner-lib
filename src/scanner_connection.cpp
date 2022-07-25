/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "scanner_connection.h"
#include "blickfeld/utils.h"
#include "blickfeld/protocol_exception.h"
#include "blickfeld/exception.h"
#include "blickfeld/error.custom.pb.h"

#include <string>
#include <asio.hpp>

using namespace std;

namespace blickfeld {

const uint32_t scanner_connection::protocol_version = 1;
const uint32_t scanner_connection::default_server_port = 8000;
#ifdef HAVE_OPENSSL
const uint32_t scanner_connection::default_server_ssl_port = 8800;
#endif

scanner_connection::scanner_connection(asio::io_context& io_context, string hostname, std::chrono::duration<long int> timeout) :
	io_context(io_context),
	hostname(hostname),
	timeout(timeout),
#ifdef HAVE_OPENSSL
	socket(std::make_shared<asio::ip::tcp::socket>(io_context))
#else
	socket(io_context)
#endif
{
	connect(hostname, default_server_port);
}

#ifdef HAVE_OPENSSL
scanner_connection::scanner_connection(asio::io_context& io_context, std::string hostname, asio::ssl::context& ssl_context, std::chrono::duration<long int> timeout) :
	io_context(io_context),
	hostname(hostname),
	timeout(timeout),
	socket(std::make_shared<asio::ssl::stream<asio::ip::tcp::socket> >(io_context, ssl_context))
{
	connect(hostname, default_server_ssl_port);
	socket.handshake(asio::ssl::stream_base::client);
}

#endif

void scanner_connection::connect(const std::string full_hostname, const uint32_t default_port) {
	// Determine endpoint
	string hostname = full_hostname;
	string port = to_string(default_port);
	string::size_type colon_pos = hostname.find(":");
	if(colon_pos != string::npos) {
		port = hostname.substr(colon_pos + 1);
		hostname = hostname.substr(0, colon_pos);
	}

	// Prepare synchronous context
	io_context.reset();

	// Set up sync flags and deadline timer
	auto expired = make_shared<bool>(), connected = make_shared<bool>();
	*expired = false, *connected = false;
	asio::steady_timer timer(io_context);
	timer.expires_from_now(timeout);
	timer.async_wait([expired, connected](const asio::error_code& ec) {
			*expired = !*connected;
		});

	// Resolve hostname & connect
	asio::ip::tcp::resolver resolver(io_context);
	resolver.async_resolve(hostname, port, [this, connected](const asio::error_code& ec, asio::ip::tcp::resolver::iterator endpoint_iterator) {
			if (ec) {
				throw protocol::CreateErrorConnectionAbort("Failed to resolve hostname. Failed with error: " + ec.message());
			}

			socket.async_connect(*endpoint_iterator, [this, connected](const asio::error_code& ec) {
				if (ec) {
					throw protocol::CreateErrorConnectionAbort("Failed to connect. Failed with error: " + ec.message());
				}

				*connected = true;
			});
		});

	while(io_context.run_one() && !*connected) {
		if (*expired)
			throw protocol::CreateErrorConnectionAbort("Connection timeout after " + to_string(timeout.count()) + " seconds");
	}
	timer.cancel();
}

void scanner_connection::send(const protocol::Request &req) {
	auto msg_size = req.ByteSizeLong();

	std::ostream send_stream(&output_buffer);
	send_stream.write(reinterpret_cast<const char*>(&msg_size), sizeof(uint32_t));
	req.SerializeToOstream(&send_stream);

	asio::write(socket, output_buffer);
}

void scanner_connection::recv(protocol::Response &resp) {
	if (async_read_active) {
		while (1) {
			{
				std::unique_lock<std::mutex> lock(receive_mutex);
				if(async_received.data_case() != async_received.DATA_NOT_SET) {
					resp = async_received;
					async_received.clear_data();
					break;
				}
			}
			io_context.run_one();
		}
	} else {
		std::istream recv_stream(&input_buffer);

		// Prepare synchronous context
		io_context.reset();

		// Set up sync flags and deadline timer
		auto expired = make_shared<bool>(), received = make_shared<bool>();
		*expired = false, *received = false;
		asio::steady_timer timer(io_context);
		timer.expires_from_now(timeout);
		timer.async_wait([expired, received](const asio::error_code& ec) {
				*expired = !*received;
			});

		// Asynchronous read
		asio::async_read(socket, input_buffer, asio::transfer_exactly(sizeof(uint32_t)), [this, &resp, received](const asio::error_code& ec, std::size_t length) {
				if (ec) {
					throw protocol::CreateErrorConnectionAbort("Failed to read message size with error code: " + ec.message());
				}

				uint32_t size;
				std::istream recv_stream_size(&input_buffer);
				recv_stream_size.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
				asio::async_read(socket, input_buffer, asio::transfer_exactly(size), [this, &resp, received](const std::error_code& ec, std::size_t length) {
					if (ec) {
						throw protocol::CreateErrorConnectionAbort("Failed to read message buffer with error code: " + ec.message());
					}

					std::istream recv_stream(&input_buffer);
					parse_response(recv_stream, resp);

					// Successful read
					*received = true;
				});
			});

		// Make asynchronous read, synchronous
		while(io_context.run_one() && !*received) {
			if (*expired)
				throw protocol::CreateErrorConnectionAbort("Timeout after " + to_string(timeout.count()) + " seconds");
		}
		timer.cancel();
	}
}

void scanner_connection::parse_response(std::istream &recv_stream, protocol::Response &resp) {
	// Parse the message.
	resp.Clear();
	if (!resp.ParseFromIstream(&recv_stream)) {
		throw str_exception("Message parse failed.");
	}

	// Check for error
	if (resp.has_error()) {
		throw_typed_protocol_exception(resp.error());
	}

	// Check for event
	if (resp.has_event() && resp.event().data_case() != protocol::stream::Event::DATA_NOT_SET) {
		for (auto cb : event_callbacks[resp.event().data_case()])
			cb(resp.event());
		return;
	}

	if (async_read_active) {
		std::unique_lock<std::mutex> lock(receive_mutex);
		async_received = async_resp;
	}
}

void scanner_connection::subscribe(const protocol::stream::Event::DataCase event_id, event_callback_t cb) {
	event_callbacks[event_id].push_back(cb);
}

void scanner_connection::async_recv() {
	asio::async_read(socket, input_buffer, asio::transfer_exactly(sizeof(uint32_t)), [this](const std::error_code& ec, std::size_t length) {
			if (ec) {
				throw str_exception("Failed to read size.");
			}
			std::istream recv_stream_size(&input_buffer);

			uint32_t size;
			recv_stream_size.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));
			asio::async_read(socket, input_buffer, asio::transfer_exactly(size), [this](const std::error_code& ec, std::size_t length) {
				if (ec) {
					throw str_exception("Failed to read message.");
				}
				std::istream recv_stream(&input_buffer);

				parse_response(recv_stream, async_resp);

				async_recv();
			});
		});
}

void scanner_connection::async_run() {
	if (async_read_active)
		return;

	async_read_active = true;
	async_recv();
}

}
