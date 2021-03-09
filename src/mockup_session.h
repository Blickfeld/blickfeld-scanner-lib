/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include "blickfeld/scanner-config.h"

#if defined (BSL_RECORDING) && !defined (BSL_STANDALONE)

#include "blickfeld/mockup_server.h"
#include "asio.h"

namespace blickfeld {

namespace network {

class mockup_session : public std::enable_shared_from_this<mockup_session>, public logged_object {
protected:
	simulated_connection* const conn;

	std::shared_ptr<asio::io_context> io_context;
	std::shared_ptr<asio::ip::tcp::socket> socket;
	std::shared_ptr<asio::thread> context_thread;
	std::string client_ident;

	std::shared_ptr<asio::thread> send_thread;

	asio::streambuf input_buffer;
	asio::streambuf output_buffer;

	void do_read();
	void do_send();

public:
	mockup_session(const std::string replay_dump_fn, point_cloud_stream<protocol::data::Frame>::processor_t pc_processor, std::shared_ptr<asio::io_context> io_context, std::shared_ptr<asio::ip::tcp::socket> socket, std::shared_ptr<asio::thread> context_thread);
	virtual ~mockup_session();

	void start();
	void stop();
};

}  // namespace network

}  // namespace blickfeld

#endif