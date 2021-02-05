/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "blickfeld/mockup_server.h"

#if defined (BSL_RECORDING) && !defined (BSL_STANDALONE)

#include "simulated_connection.h"
#include "mockup_session.h"
namespace blickfeld {

namespace network {

mockup_server::mockup_server(const std::string replay_dump_fn, point_cloud_stream<protocol::data::Frame>::processor_t pc_processor, unsigned int listen_port)
	: logged_object("mockup_server"),
	replay_dump_fn(replay_dump_fn),
	pc_processor(pc_processor),
	port(listen_port),
	io_context(new asio::io_context()),
	acceptor(new asio::ip::tcp::acceptor(*io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))) {
	do_accept();
	log_info("mockup-server ready on port %d\n", port);
}

mockup_server::~mockup_server() {
	if (acceptor)
		delete acceptor;
	if (io_context)
		delete io_context;
}

int mockup_server::serve_forever() {
	log_info("started...\n");
	io_context->run();
	log_info("shutting down...\n");
	return 0;
}

void mockup_server::do_accept() {
	auto client_io_context = std::make_shared<asio::io_context>();
	auto guard = asio::make_work_guard(*client_io_context);
	auto socket = std::make_shared<asio::ip::tcp::socket>(*client_io_context);
	auto thread = std::make_shared<asio::thread>([this, client_io_context] {
				try {
					client_io_context->run();
				} catch (std::exception& ex) {
					log_warning("Client thread threw exception.\n%s\n", ex.what());
				}
			});

	acceptor->async_accept(*socket, [this, client_io_context, guard, socket, thread](const std::error_code& error) {
				auto client = std::make_shared<network::mockup_session>(replay_dump_fn, pc_processor, client_io_context, socket, thread);

				client->start();

				if (acceptor->is_open())
					do_accept();
			});
}

}  // namespace network

}  // namespace blickfeld

#endif