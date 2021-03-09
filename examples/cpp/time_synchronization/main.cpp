/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include <iostream>
#include <chrono>
#include <thread>

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>

int example(int argc, char* argv[]) {
	std::string scanner_ip_or_host = "localhost";
	std::string time_sync = "ntp";
	if (argc > 1) {
		scanner_ip_or_host = argv[1];
	} else {
		std::cout << "No hostname or IP provided, please provide hostname or ip and NTP or PTP" << std::endl;
		return 0;
	}
	if (argc > 2) {
		time_sync = argv[2];
	} else {
		std::cout << "No second argument provided, please provide hostname or ip and NTP or PTP" << std::endl;
		return 0;
	}

	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	std::cout << "Connected to " << scanner_ip_or_host << std::endl;

	if (time_sync == "ntp") {
		std::cout << "NTP synchronization selected. Starting time synchronization.." << std::endl;
		scanner->set_ntp_time_synchronization();
	} else if (time_sync == "ptp") {
		std::cout << "PTP synchronization selected. Starting time synchronization.." << std::endl;
		scanner->set_ptp_time_synchronization();
	} else {
		std::cout << "Couldn't parse second input, please provide either NTP or PTP" << std::endl;
		return 0;
	}

	std::cout << "Synchronization status:" << std::endl;
	std::cout << scanner->get_status().time_synchronization().DebugString() << std::endl;

	return 0;
}

int main(int argc, char** argv) {
	try {
		return example(argc, argv);
	} catch (const std::exception& e) {
		fprintf(stderr, "main caught exception:\n%s\n", e.what());
	}
	return 1;
}
