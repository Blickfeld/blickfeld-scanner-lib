/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "blickfeld/utils.h"
#include "blickfeld/protocol_exception.h"
#include "blickfeld/error.custom.pb.h"
#include "connection.h"

namespace blickfeld {

void connection::send_request(const protocol::Request& req, protocol::Response& resp) {
	std::lock_guard<std::mutex> guard(request_mutex);

	try {
		try {
			send(req);
			recv(resp);
		} catch(const protocol_exception_base &) {
			throw;
		} catch(const std::exception &e) {
			throw protocol::CreateErrorUnknown(std::string(e.what()));
		}
	} catch(const protocol_exception_base &e) {
#ifndef BSL_STANDALONE
		throw;
#else
		throw str_exception("%s", e.what());
#endif
	}
}

}
