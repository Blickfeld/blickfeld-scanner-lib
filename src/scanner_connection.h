/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include "connection.h"

#include <chrono>

namespace blickfeld {

class scanner_connection: public connection {
protected:
	asio::io_context& io_context;
	const std::string hostname;
	const std::chrono::duration<long int> timeout;
#ifdef HAVE_OPENSSL
	socket_wrapper socket;
#else
	asio::ip::tcp::socket socket;
#endif
	asio::streambuf input_buffer;
	asio::streambuf output_buffer;

	std::mutex receive_mutex;
	protocol::Response async_resp, async_received;
	bool async_read_active = false;

	void connect(const std::string full_hostname, const uint32_t default_port);
	void parse_response(std::istream &recv_stream, protocol::Response &resp);
	void async_recv();

	scanner_connection();
public:
	static const uint32_t protocol_version;
	static const uint32_t default_server_port;
#ifdef HAVE_OPENSSL
	static const uint32_t default_server_ssl_port;
#endif

	scanner_connection(asio::io_context& io_context, std::string hostname, std::chrono::duration<long int> timeout = std::chrono::seconds(30));
#ifdef HAVE_OPENSSL
	scanner_connection(asio::io_context& io_context, std::string hostname, asio::ssl::context& ssl_context, std::chrono::duration<long int> timeout = std::chrono::seconds(30));
#endif

	void recv(protocol::Response &resp) override;
	void send(const protocol::Request &req) override;

	void async_run() override;
	void subscribe(const protocol::stream::Event::DataCase event_id, event_callback_t cb) override;
};

}
