/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include <mutex>
#include <functional>
#include "asio.h"

#include "blickfeld/connection.pb.h"
#include "blickfeld/scanner.h"

#ifdef HAVE_OPENSSL
#include "socket_wrapper.h"
#include "asio/ssl.hpp"
#endif

namespace blickfeld {

class connection {
	std::mutex request_mutex;

protected:
	using event_callback_t = std::function<void(const protocol::stream::Event& event)>;
	std::map<const protocol::stream::Event::DataCase, std::list<event_callback_t>> event_callbacks;

	connection() {
	};
public:
	virtual ~connection() {
	};

	virtual void recv(protocol::Response &resp) = 0;
	virtual void send(const protocol::Request &req) = 0;
	void send_request(const protocol::Request& req, protocol::Response& resp);

	virtual void async_run() = 0;
	virtual void subscribe(const protocol::stream::Event::DataCase event_id, event_callback_t cb) = 0;
};

}
