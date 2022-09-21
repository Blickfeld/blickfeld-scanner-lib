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
#include <iomanip>

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>

bool keep_alive = true;
void sigint_handler(int signal) {
	if (keep_alive) {
		std::cout << "Received SIGINT. Stopping stream." << std::endl;
		keep_alive = false;
	} else {
		exit(signal);
	}
}

int example(int argc, char* argv[]) {
	// Install a sigint handler
	std::signal(SIGINT, sigint_handler);

	std::string scanner_ip_or_host = "localhost";
	if(argc > 1)
		// First argument is the IP adress or hostname of the devie.
		scanner_ip_or_host = argv[1];

	// Create a connection to the device.
	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	std::cout << "Connected." << std::endl;

	// Create a pointcloud stream object to receive pointclouds
	auto stream = scanner->get_imu_stream();

	while (keep_alive) {
		// Format of IMU data is described in protocol documentation:
		// https://docs.blickfeld.com/cube/latest/external/blickfeld-scanner-lib/protobuf_protocol.html
		//
		// Protobuf API is described in https://developers.google.com/protocol-buffers/docs/cpptutorial
		const blickfeld::protocol::data::IMU data = stream->recv_burst();

		// Print burst package
		std::cout << data << std::endl;

		// Extract samples and print
		int print_n_samples = 3;
		for (auto sample : data.samples()) {
			std::cout << "- acc: ["
				  << std::setprecision(3) << std::fixed
				  << sample.acceleration(0) << ", "
				  << sample.acceleration(1) << ", "
				  << sample.acceleration(2) << "], gyro: ["
				  << sample.angular_velocity(0) << ", "
				  << sample.angular_velocity(1) << ", "
				  << sample.angular_velocity(2) << "]>"
				  << std::endl;
			if (!--print_n_samples)
				break;
		}
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
