/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include <stdio.h>

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>

int example(int argc, char* argv[]) {
	std::string scanner_ip_or_host = "localhost";
	std::string dump_fn;
	if(argc > 1)
		scanner_ip_or_host = argv[1];
	if(argc > 2)
		dump_fn = argv[2];

	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	printf("Connected.\n");

	// Synchronous request for scan_pattern
	printf("ScanPattern: %s", scanner->get_scan_pattern().DebugString().c_str());

	// Asynchronous request for status
	scanner->subscribe([](const blickfeld::protocol::Status& status) {
		printf("Got scanner status (async): %s", status.scanner().DebugString().c_str());
	});

	// Synchronous request for status
	printf("Got status (sync): %s", scanner->get_status().DebugString().c_str());

	std::shared_ptr<std::thread> scanner_thread = scanner->async_run_thread();

	// TODO Example for application logic
	auto stream = scanner->get_point_cloud_stream();
	stream->subscribe([scanner](const blickfeld::protocol::data::Frame& frame) {
		// Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
		// Protobuf API is described in https://developers.google.com/protocol-buffers/docs/cpptutorial
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
//				auto& point = frame.scanlines(s_ind).points(p_ind);
//
//				printf("Point %u [x: %4.2f, y: %4.2f, z: %4.2f] - intensity: %u\n",
//					point.id(),
//					point.returns(0).cartesian(0), point.returns(0).cartesian(1), point.returns(0).cartesian(2),
//					point.returns(0).intensity());
			}
		}
	});

	// Join thread of scanner
	scanner_thread->join();

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
     
