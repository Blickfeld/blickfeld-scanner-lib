/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "blickfeld/imu_stream.h"
#include "blickfeld/utils.h"

#ifndef BSL_STANDALONE

#include "connection.h"
#include <iomanip>

using namespace std;

namespace blickfeld {

imu_stream::imu_stream(std::shared_ptr<connection> conn, const protocol::stream::Subscribe_IMU* extend_subscription) :
	conn(conn),
	resp(new protocol::Response())
{
	protocol::Request req;
	protocol::Response resp;
	req.mutable_subscribe()->mutable_imu();
	if (extend_subscription)
		req.mutable_subscribe()->mutable_imu()->MergeFrom(*extend_subscription);
	conn->send_request(req, resp);
}

imu_stream::~imu_stream() {
	if (resp)
		delete resp;
}

const protocol::data::IMU& imu_stream::recv_burst() {
	// Receive and extract data
	conn->recv(*resp);
	return (static_cast<protocol::Response&>(*resp)).event().imu();
}

} // namespace blickfeld

std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::IMU& data) {
	if (!data.samples_size())
		return strm << "<Blickfeld IMU Burst: Empty>";
	return strm << "<Blickfeld IMU Burst: " << data.samples_size() << " samples, acc: ["
		    << setprecision(3) << fixed
		    << data.samples(0).acceleration(0) << ", "
		    << data.samples(0).acceleration(1) << ", "
		    << data.samples(0).acceleration(2) << "], gyro: ["
		    << data.samples(0).angular_velocity(0) << ", "
		    << data.samples(0).angular_velocity(1) << ", "
		    << data.samples(0).angular_velocity(2) << "], "
		    << blickfeld::os::get_time_string(data.start_time_ns() / 1e9) << ">";
}

#endif
