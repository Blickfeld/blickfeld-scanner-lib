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

scanner_connection::scanner_connection(asio::io_context& io_context, string hostname) :
	io_context(io_context),
	hostname(hostname),
#ifdef HAVE_OPENSSL
	socket(std::make_shared<asio::ip::tcp::socket>(io_context))
#else
	socket(io_context)
#endif
{

	string port = to_string(default_server_port);
	string::size_type colon_pos = hostname.find(":");
	if(colon_pos != string::npos) {
		port = hostname.substr(colon_pos + 1);
		hostname = hostname.substr(0, colon_pos);
	}

	asio::ip::tcp::resolver resolver(io_context);
	socket.connect(*resolver.resolve(hostname, port).begin());
}

#ifdef HAVE_OPENSSL
scanner_connection::scanner_connection(asio::io_context& io_context, std::string hostname, asio::ssl::context& ssl_context) :
	io_context(io_context),
	hostname(hostname),
	socket(std::make_shared<asio::ssl::stream<asio::ip::tcp::socket> >(io_context, ssl_context))
{
	string port = to_string(default_server_ssl_port);
	string::size_type colon_pos = hostname.find(":");
	if(colon_pos != string::npos) {
		port = hostname.substr(colon_pos + 1);
		hostname = hostname.substr(0, colon_pos);
	}

	asio::ip::tcp::resolver resolver(io_context);
	socket.connect(*resolver.resolve(hostname, port).begin());
	socket.handshake(asio::ssl::stream_base::client);
}

#endif

void scanner_connection::send(const protocol::Request &req) {
	auto msg_size = req.ByteSizeLong();

	std::ostream send_stream(&output_buffer);
	send_stream.write((const char*) &msg_size, sizeof(uint32_t));
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

		uint32_t size;
		asio::read(socket, input_buffer, asio::transfer_exactly(sizeof(uint32_t)));
		recv_stream.read((char*)&size, sizeof(uint32_t));
		asio::read(socket, input_buffer, asio::transfer_exactly(size));

		parse_response(recv_stream, resp);
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
		recv_stream_size.read((char*)&size, sizeof(uint32_t));
		asio::async_read(socket, input_buffer, asio::transfer_exactly(size), [this, size](const std::error_code& ec, std::size_t length) {
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
