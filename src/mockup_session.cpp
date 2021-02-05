/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "mockup_session.h"

#if defined (BSL_RECORDING) && !defined (BSL_STANDALONE)

#include "simulated_connection.h"
#include "asio.h"

using asio::ip::tcp;

namespace blickfeld {

namespace network {

mockup_session::mockup_session(const std::string replay_dump_fn, point_cloud_stream<protocol::data::Frame>::processor_t pc_processor, std::shared_ptr<asio::io_context> io_context, std::shared_ptr<asio::ip::tcp::socket> socket, std::shared_ptr<asio::thread> context_thread)
	: logged_object("mockup_server:session"),
	conn(new simulated_connection(replay_dump_fn, pc_processor)),
	io_context(io_context),
	socket(socket),
	context_thread(context_thread),
	client_ident(socket->remote_endpoint().address().to_string() + ":" + std::to_string(socket->remote_endpoint().port())) {
}

mockup_session::~mockup_session() {
	log_info("[%s] Closed client connection\n", client_ident.c_str());

	if (conn)
		delete conn;
}

void mockup_session::start() {
	log_info("[%s] Received client connection\n", client_ident.c_str());
	do_read();

	do_send();
}

void mockup_session::do_read() {
	auto self(shared_from_this());

	asio::async_read(*socket, input_buffer, asio::transfer_exactly(sizeof(uint32_t)), [this, self](const std::error_code& ec, std::size_t length) {
				if (ec) {
					log_debug("[%s] Client read length failed. Error-code %u.\n", client_ident.c_str(), ec);
					socket->close();
					return;
				}
				std::istream recv_stream_size(&input_buffer);

				uint32_t size;
				recv_stream_size.read((char*)&size, sizeof(uint32_t));
				asio::async_read(*socket, input_buffer, asio::transfer_exactly(size), [this, self, size](const std::error_code& ec, std::size_t length) {
					if (ec) {
						log_debug("[%s] Client read message failed. Error-code %u.\n", client_ident.c_str(), ec);
						socket->close();
						return;
					}

					std::istream recv_stream(&input_buffer);

					// Parse the message.
					protocol::Request req;
					if (!req.ParseFromIstream(&recv_stream)) {
						log_error("[%s] Client message parse failed.\n", client_ident.c_str());
						return;
					}

					log_info("Received request %s\n", req.DebugString().c_str());

					// Continue reading
					do_read();

					// Handle request
					conn->send(req);
				});
			});
}

void mockup_session::do_send() {
	auto weak_self(weak_from_this());

	send_thread = std::make_shared<asio::thread>([this, weak_self] {
				while (auto self = weak_self.lock()) {
				        // Pass response
					protocol::Response resp;
					conn->recv(resp);

					auto data_field = protocol::Response::descriptor()->FindFieldByNumber(resp.data_case());
					log_info("Sending response %s\n", data_field->full_name().c_str());

					uint32_t msg_size = resp.ByteSizeLong();
					std::ostream send_stream(&output_buffer);
					send_stream.write((const char*)&msg_size, sizeof(uint32_t));
					resp.SerializeToOstream(&send_stream);

					asio::error_code write_ec;
					asio::write(*socket, output_buffer, write_ec);
					if (write_ec) {
						log_debug("[%s] Client write failed.\n", client_ident.c_str());
						socket->close();
						return;
					}
				}
			});
}

}  // namespace network

}  // namespace blickfeld

#endif