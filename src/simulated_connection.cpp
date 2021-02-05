/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "simulated_connection.h"

#if defined (BSL_RECORDING) && !defined (BSL_STANDALONE)

namespace blickfeld {

namespace network {

using namespace blickfeld::protocol;

simulated_connection::simulated_connection(std::string replay_dump_fn, point_cloud_stream<protocol::data::Frame>::processor_t pc_processor, bool loop_recording)
	: pc_processor(pc_processor),
	loop_recording(loop_recording) {
	// Open dump file
	dump_file.open(replay_dump_fn, std::ios::in | std::ios::binary);

	// Open stream
	dump_active = true;
	dump_stream = std::make_shared<scanner::point_cloud_stream<protocol::data::Frame> >(&dump_file);
}

simulated_connection::~simulated_connection() {
	dump_active = false;
	dump_thread->join();
}

void simulated_connection::recv(protocol::Response& resp) {
	std::unique_lock<std::mutex> lck(response_mutex);

	if (!response_buffer.size()) {
		response_condition.wait(lck);
	}

	resp.CopyFrom(response_buffer.front());
	response_buffer.pop_front();
}

void simulated_connection::send(const protocol::Request& req) {
	std::unique_lock<std::mutex> lck(response_mutex);
	protocol::Response resp;

	// TODO Send hello and feedback that this is a mockup
	if (req.has_hello()) {
		resp.mutable_hello();
	}

	if (req.has_proxy_hello()) {
		resp.mutable_proxy_hello();
	}

	// TODO Send proper status
	if (req.subscribe().has_status()) {
		resp.mutable_event()->mutable_status()->mutable_scanner()->set_state(status::Scanner_State_READY);
	}

	// TODO Handle unsubscribe
	if (req.subscribe().has_point_cloud()) {
		resp.mutable_event()->mutable_point_cloud()->mutable_header()->CopyFrom(dump_stream->get_metadata().header().device());

		dump_thread = std::make_shared<asio::thread>([this, req] {
					std::function<void (protocol::data::Frame&)> on_frame;
					if (pc_processor) {
						on_frame = pc_processor(req.subscribe().point_cloud());
					}

					uint64_t last_ts = 0;
					while (dump_active) {
						if (dump_stream->end_of_stream()) {
							if (!loop_recording)
								break;
							dump_stream->jump_to_first_frame();
							last_ts = 0;
						}

						std::unique_lock<std::mutex> lck(response_mutex);

						auto frame = dump_stream->recv_frame();

						if (on_frame)
							on_frame(frame);

						protocol::Response resp;
						resp.mutable_event()->mutable_point_cloud()->mutable_frame()->CopyFrom(frame);
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

}  // namespace network

}  // namespace blickfeld

#endif