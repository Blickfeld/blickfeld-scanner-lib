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

#include "blickfeld/logged_object.h"
#include "blickfeld/connection.pb.h"
#include "blickfeld/point_cloud_stream.h"

namespace asio {
class io_context;

class executor;
template <typename Protocol, typename Executor>
class basic_socket_acceptor;
namespace ip {
class tcp;
}
}

namespace blickfeld {

namespace network {

class simulated_connection;
class mockup_session;

/**
 * @brief This server mocks the blickfeld-scanner-server, which accepts the BSL connections on the Blickfeld devices.
 *
 * The implementation is currently **incomplete**.
 * It does not yet follow the correct handshake patterns and a lot of API requests are only implemented partially or not at all.
 * The core functionality of passing point-clouds is available.
 * It currently loops through a provided dump file and passes the frames using the delay in the metadata.
 * A pointcloud_stream_processor_t can be used to perform processing operations on the frames.
 */
class mockup_server : public logged_object {
public:
	static const int PROTOCOL_VERSION = 1;

protected:
	std::string replay_dump_fn;
	point_cloud_stream<protocol::data::Frame>::processor_t pc_processor;
	unsigned int port;

	asio::io_context* io_context;
	asio::basic_socket_acceptor<asio::ip::tcp, asio::executor>* acceptor;
	std::vector<std::weak_ptr<mockup_session> > sessions;

	void do_accept();

public:
	/**
	 * @brief Construct a new mockup server object
	 *
	 * @param replay_dump_fn Provide filename of a bfpc dump file
	 * @param pc_processor Provide point cloud stream processor, which can be used to alter the read point cloud.
	 * @param listen_port Listen port of the server, defaults to 8000.
	 */
	mockup_server(const std::string replay_dump_fn,  point_cloud_stream<protocol::data::Frame>::processor_t pc_processor = nullptr, unsigned int listen_port = 8000);
	virtual ~mockup_server();

	/**
	 * @brief Starts the server and blocks until its terminated.
	 *
	 * @return int Return 0 if it has been exited normally.
	 */
	int serve_forever();

	/**
	 * @brief Shutdown server
	 *
	 * This can be called asynchronously to shutdown the server.
	 */
	void shutdown();
};

}  // namespace network

}  // namespace blickfeld

#endif