/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "blickfeld/protocol_exception.h"
#include "connection.h"
#include "asio.h"

namespace blickfeld {

class simulated_connection: public connection {

	std::ifstream dump_file;
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > dump_stream;
	std::shared_ptr<asio::thread> dump_thread;

	std::list<protocol::Response> response_buffer;
	std::mutex response_mutex;
	std::condition_variable response_condition;

public:
	simulated_connection(std::string replay_dump_fn);
	virtual ~simulated_connection();

	void recv(protocol::Response &resp) override;
	void send(const protocol::Request &req) override;

	void async_run() {
		throw protocol_exception<protocol::Error::NotImplemented>();
	}

	void subscribe(const protocol::stream::Event::DataCase event_id, event_callback_t cb) {
		throw protocol_exception<protocol::Error::NotImplemented>();
	}

};

}
