/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>

#include <blickfeld/discover.h>
#include <blickfeld/string_utils.h>

int example(int argc, char* argv[]) {
	auto results = blickfeld::discover::search(0.5);

	// Print table
	std::cout << std::string(42, '-') << std::endl;
	std::cout << "| " << std::setw(20) << std::left << "Hostname" << " | " << std::setw(15) << std::left << "IP" << " |" << std::endl;
	std::cout << std::string(42, '-') << std::endl;
	for (auto& result : results) {
		std::cout << "| " << std::setw(20) << std::left << result.hostname << " | " << std::setw(15) << std::left << blickfeld::format_string("%u.%u.%u.%u", result.ipv4[0], result.ipv4[1], result.ipv4[2], result.ipv4[3]) << " |" << std::endl;
	}
	if (!results.size())
		std::cout << "| " << std::setw(38) << std::left <<  "<No devices discovered>" << " |" << std::endl;
	std::cout << std::string(42, '-') << std::endl;

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
