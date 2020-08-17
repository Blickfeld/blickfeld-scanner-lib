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
#include <csignal>

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>

std::shared_ptr<blickfeld::scanner::point_cloud_stream<blickfeld::protocol::data::Frame> > stream;

void sigint_handler(int signal) {
	stream = nullptr;
	exit(signal);
}

int example(int argc, char* argv[]) {
	// Install a sigint handler
	std::signal(SIGINT, sigint_handler);

	std::string scanner_ip_or_host = "localhost";
	std::string dump_fn;
	if(argc > 1)
		// First argument is the IP adress or hostname of the devie.
		scanner_ip_or_host = argv[1];
	if(argc > 2)
		// Second argument is the path or filenname to which the pointcloud will be streamed. If you want to dump, provide this argument.
		dump_fn = argv[2];

	// Create a connection to the device.
	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	std::cout << "Connected." << std::endl;

	// Create a pointcloud stream object to receive pointclouds
	stream = scanner->get_point_cloud_stream();

	std::ofstream dump_stream;
#ifdef BSL_RECORDING
	if (dump_fn != "") {
		printf("Recording to %s.bfpc ..\n", dump_fn.c_str());
		dump_stream.open(dump_fn + ".bfpc", std::ios::out | std::ios::trunc | std::ios::binary);
		stream->record_to_stream(&dump_stream);
	}
#endif

	while (true) {
		// Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
		// Protobuf API is described in https://developers.google.com/protocol-buffers/docs/cpptutorial
		const blickfeld::protocol::data::Frame frame = stream->recv_frame();

		/*
		 * Print information about this frame
		 *
		 * Reference implementation of stream operator to clarify available attribute methods:
		 * 	std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Frame& frame) {
		 * 		return strm << "<Blickfeld Frame " << frame.id() << ": " << frame.total_number_of_returns() << " returns, "
		 * 			<< setprecision(1) << fixed << frame.scan_pattern().horizontal().fov() * 180.0f / M_PI << "x" << frame.scan_pattern().vertical().fov() * 180.0f / M_PI << " FoV, "
		 * 			<< setprecision(0) << fixed << frame.scanlines_size() << " scanlines>";
		 * 	}
		 */
		std::cout << frame << std::endl;

		// Specify number of points printed per frame
		int print_n_points = 3;

		// Example for scanline and point iteration
		// Iterate through all the scanlines in a frame
		for (int s_ind = 0; s_ind < frame.scanlines_size(); s_ind++) {
			// Iterate through all the points in a scanline
			for (int p_ind = 0; p_ind < frame.scanlines(s_ind).points_size(); p_ind++) {
				auto& point = frame.scanlines(s_ind).points(p_ind);

				/*
				 * Print information about the first 3 points of the first scanline
				 *
				 * Reference implementation of stream operator to clarify available attribute methods:
				 *
				 * 	std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Point& point) {
				 *		return strm << "<Blickfeld Point " << point.id() << ": " << point.returns_size() << (point.returns_size() == 1 ? " return>" : " returns>");
				 *	}
				 */
				if (p_ind < print_n_points && s_ind == 0)
					std::cout << point << (point.returns_size() ? ": " : "");

				// Iterate through all the returns for each points
				for (int r_ind = 0; r_ind < point.returns_size(); r_ind++) {
					auto& ret = point.returns(r_ind);

					/*
					 * Print return information about the first 3 points of the first scanline
					 *
					 * Reference implementation of stream operator to clarify available attribute methods:
					 * 	std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Point::Return& ret) {
					 *		return strm << "<Blickfeld Point::Return " << ret.id() << ": "
					 *				<< setprecision(2) << fixed
					 *				<< "(x: " << ret.cartesian(0)
					 *				<< ", y: " << ret.cartesian(1)
					 *				<< ", z: " << ret.cartesian(0)
					 *				<< ") [m], "
					 *				<< setprecision(0) << "intensity of " << ret.intensity() << ">";
					 *	}
					 */
					if (p_ind < print_n_points && s_ind == 0)
						std::cout << (ret.id() > 0 ? ", " : "") << ret;
				}

				if (p_ind < print_n_points && s_ind == 0)
					std::cout << std::endl;
			}
		}
	}

	stream = nullptr;
	dump_stream.close();

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
