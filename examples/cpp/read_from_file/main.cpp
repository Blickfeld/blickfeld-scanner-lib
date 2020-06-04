/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>
#include <blickfeld/exception.h>

int example(int argc, char* argv[]) {
	std::string dump_fn;
	if(argc > 1)
		dump_fn = argv[1];

	std::ifstream dump_file;
	dump_file.open (dump_fn, std::ios::in | std::ios::binary);
	if (!dump_file.good())
		throw blickfeld::str_exception("Failed to open file '%s'", dump_fn.c_str());

	auto stream = blickfeld::scanner::file_point_cloud_stream(&dump_file);
	printf("Metadata:\n%s", stream->get_metadata().DebugString().c_str());

	int max_n_frames = 3;
	while (!stream->end_of_stream() && max_n_frames--) {
		// Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
		// Protobuf API is described in https://developers.google.com/protocol-buffers/docs/cpptutorial
		const blickfeld::protocol::data::Frame frame = stream->recv_frame();

		time_t time_s = frame.start_time_ns() / 1e9;
		auto timepoint = localtime(&time_s);
		printf ("Frame:  scanlines %u (max %0.2f Hz - current %0.2f Hz) - timestamp %f - %s",
			frame.scanlines_size(),
			frame.scan_pattern().frame_rate().maximum(),
			frame.scan_pattern().frame_rate().target(),
			frame.start_time_ns() / 1e9,
			asctime(timepoint)
			);

		// Example for scanline and point iteration
		for (int s_ind = 0; s_ind < frame.scanlines_size(); s_ind++) {
			for (int p_ind = 0; p_ind < frame.scanlines(s_ind).points_size(); p_ind++) {
				auto& point = frame.scanlines(s_ind).points(p_ind);

				for (int r_ind = 0; r_ind < point.returns_size(); r_ind++) {
					auto& ret = point.returns(r_ind);

					if (p_ind < 10 && s_ind == 0)
						printf("Point %u -ret %u [x: %4.2f, y: %4.2f, z: %4.2f] - intensity: %u\n",
						       point.id(), ret.id(),
						       ret.cartesian(0), ret.cartesian(1), ret.cartesian(2),
						       ret.intensity());
				}
			}
		}

		//		uint64_t previous_timestamp = frame->has_start_time_ns() ? frame->start_time_ns() : 0;
		//		convert_point_cloud(data.frame(), *frame);
		//		if (previous_timestamp)
		//			usleep((frame->start_time_ns() - previous_timestamp) / 1e3);
	}

	return 0;
}

int main(int argc, char* argv[]) {
	try {
		return example(argc, argv);
	} catch(const std::exception& e) {
		fprintf(stderr, "main caught exception:\n%s\n", e.what());
	}
	return 1;
}
