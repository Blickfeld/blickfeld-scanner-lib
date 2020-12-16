/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "simulated_connection.h"

namespace blickfeld {

simulated_connection::simulated_connection(std::string replay_dump_fn) {
	// Open dump file
	dump_file.open(replay_dump_fn, std::ios::in | std::ios::binary);

	// Open stream
	dump_stream = std::make_shared<scanner::point_cloud_stream<protocol::data::Frame> >(&dump_file);
}

simulated_connection::~simulated_connection() {
	dump_thread->join();
}

void simulated_connection::recv(protocol::Response &resp) {
	std::unique_lock<std::mutex> lck(response_mutex);

	if (!response_buffer.size()) {
		response_condition.wait(lck);
	}

	resp.CopyFrom(response_buffer.front());
	response_buffer.pop_front();
}

void simulated_connection::send(const protocol::Request &req) {
	std::unique_lock<std::mutex> lck(response_mutex);
	protocol::Response resp;
	if (req.subscribe().has_point_cloud()) {
		resp.mutable_event()->mutable_point_cloud()->mutable_header();

		dump_thread = std::make_shared<asio::thread>([this] {
				uint64_t last_ts = 0;
				while (!dump_stream->end_of_stream()) {
					std::unique_lock<std::mutex> lck(response_mutex);

					protocol::Response resp;
					resp.mutable_event()->mutable_point_cloud()->mutable_frame()->CopyFrom(dump_stream->recv_frame());
					if (last_ts > 0) {
						uint64_t wait_ns = resp.event().point_cloud().frame().start_time_ns() - last_ts;
						std::this_thread::sleep_for(std::chrono::milliseconds((uint64_t)(wait_ns / 1e6)));
					}
					last_ts = resp.event().point_cloud().frame().start_time_ns();
					response_buffer.push_back(resp);
					response_condition.notify_all();
				}
			});
	}

	if (resp.data_case() == protocol::Response::DATA_NOT_SET)
		resp.mutable_error()->mutable_not_implemented();

	response_buffer.push_back(resp);
	response_condition.notify_all();
}

}
