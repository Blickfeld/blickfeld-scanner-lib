/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>

namespace blickfeld {

class discover {
public:
	/**
	 * Search result for discovered device.
	 */
	struct result_t {
		std::string hostname; // Hostname in network
		std::string service; // mDNS service
		uint8_t ipv4[4]; // IPv4 address of the device
		uint16_t ipv6[8]; // IPv6 address of the device
		uint16_t port; // BSL port of the device
	};

	/**
	 * Searches for Blickfeld devices in the network.
	 * @param duration Search duration in seconds.
	 * @return List of found devices as \ref result_t structs.
	 */
	static std::list<result_t> search(float duration = 0.5);

	/**
	 * Searches for Blickfeld devices in the network and connects to them.
	 * @param duration Search duration in seconds.
	 * @return List of found devices as \ref scanner instances.
	 */
	static std::list<std::shared_ptr<scanner> > search_and_connect(float duration = 0.5);

	/**
	 * Searches for one Blickfeld device in the network.
	 * If multiple devices are available, the first discovered device will be selected.
	 * @param timeout Search duration in seconds.
	 * @return Found device as \ref result_t struct.
	 * @throws protocol_exception<protocol::Error::NoDeviceDiscovered>
	 */
	static result_t search_one(float timeout = 0.5);

	/**
	 * Searches for one Blickfeld device in the network and connects to it.
	 * If multiple devices are available, the first discovered device will be selected.
	 * @param timeout Search duration in seconds.
	 * @return Found device as \ref scanner instance.
	 */
	static std::shared_ptr<scanner> search_one_and_connect(float timeout = 0.5);
};

}
