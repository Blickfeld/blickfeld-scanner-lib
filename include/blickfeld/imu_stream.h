/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include "blickfeld/scanner-config.h"

#ifndef BSL_STANDALONE

#include <memory>
#include <iostream>

namespace blickfeld {

namespace protocol {
class Response;
namespace data {
class IMU;
}
namespace stream {
class Subscribe_IMU;
}
}

class connection;

/**
 * IMU Stream class for requesting IMU streams.
 *
 * Use scanner::get_imu_stream for live streams.
 */
class imu_stream {
private:
	std::shared_ptr<connection> conn;
	protocol::Response* resp = nullptr;

public:
	/**
	 * Internal use. Use scanner::get_imu_stream.
	 *
	 * @param conn Scanner connection which should be used for stream.
	 * @param extend_subscription Merge subscription request.
	 */
	imu_stream(std::shared_ptr<connection> conn, const protocol::stream::Subscribe_IMU* extend_subscription = nullptr);

	virtual ~imu_stream();

	/**
	 * Receive burst of IMU data.
	 * Blocks until next burst is available.
	 *
	 * @return IMU data.
	 */
	const protocol::data::IMU& recv_burst();
};

}

std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::IMU& data);

#endif