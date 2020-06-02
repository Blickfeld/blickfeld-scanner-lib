/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "blickfeld/discover.h"
#include "blickfeld/string_utils.h"
#include "blickfeld/protocol_exception.h"
#include "discover_impl.hpp"

namespace blickfeld {

std::list<discover::result_t> discover::search(float duration) {
	impl::discover impl;
	impl.run_for(duration);

	return impl.results;
}

std::list<std::shared_ptr<scanner>> discover::search_and_connect(float duration) {
	auto results = search(duration);
	std::list<std::shared_ptr<scanner>> scanners;
	for (auto& result : results) {
		try {
			auto scanner = scanner::connect(format_string("%u.%u.%u.%u", result.ipv4[0], result.ipv4[1], result.ipv4[2], result.ipv4[3]));
			scanners.push_back(scanner);
		} catch(...) {
		}
	}
	return scanners;
}

discover::result_t discover::search_one(float timeout) {
	auto results = search(timeout);
	if (!results.size())
		throw protocol_exception<protocol::Error::NoDeviceDiscovered>();

	return results.front();
}

std::shared_ptr<scanner> discover::search_one_and_connect(float timeout) {
	auto result = search_one(timeout);
	return scanner::connect(format_string("%u.%u.%u.%u", result.ipv4[0], result.ipv4[1], result.ipv4[2], result.ipv4[3]));
}

}
