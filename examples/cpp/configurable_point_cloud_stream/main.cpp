/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 *
 * > Introduced in BSL v2.10 and firmware v1.9
 *
 */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <csignal>

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>

std::shared_ptr<blickfeld::scanner::point_cloud_stream<blickfeld::protocol::data::Frame> > stream;

int example(int argc, char* argv[]) {
	std::string scanner_ip_or_host = "localhost";
	if(argc > 1)
		// First argument is the IP adress or hostname of the devie.
		scanner_ip_or_host = argv[1];

	// Create a connection to the device.
	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	std::cout << "Connected." << std::endl;

	// Reference Frame with X, Y, Z coordinates, intensity and point IDs
	// The frame represents the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
	auto reference_frame = scanner->REF_FRAME_XYZ_I_ID;

	// Create filter to filter points and returns by point attributes during the post-processing on the device.
	blickfeld::protocol::config::ScanPattern::Filter filter;
	filter.mutable_range()->set_minimum(5); // Minimum range for a point to be sent out is 5m
	filter.mutable_range()->set_maximum(50); // Maximum range for a point to be sent out is 50m
	filter.set_max_number_of_returns_per_point(2); // Set max number of returns to 2. The default value is 1.

	// Create a point cloud stream object and format filter and reference frame to device
	stream = scanner->get_point_cloud_stream(filter, reference_frame);

	while (true) {
		// Request frame. It has a reduced data size, as not all fields are requested. This also speeds up the Protobuf encoding & decoding.
		// Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
		// Protobuf API is described in https://developers.google.com/protocol-buffers/docs/cpptutorial
		const blickfeld::protocol::data::Frame frame = stream->recv_frame();

		// Print information about this frame
		std::cout << frame << std::endl;
	}

	stream = nullptr;

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
