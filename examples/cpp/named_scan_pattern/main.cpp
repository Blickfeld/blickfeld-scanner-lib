/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include <iostream>

#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>

int example(int argc, char* argv[]) {
	std::string scanner_ip_or_host = "localhost";
	if (argc > 1) scanner_ip_or_host = argv[1];

	// Connect to the device
	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	std::cout << "Connected to " << scanner_ip_or_host << std::endl;

	// Get named scan patterns
	auto named_scan_patterns = scanner->get_named_scan_patterns();

	// Print if the scan pattern is a default or custom named scan pattern
	for (auto const& scan_pattern : named_scan_patterns) {
		if (scan_pattern.read_only()) {
			std::cout << "'" << scan_pattern.name() << "' is a default named scan pattern." << std::endl;
		} else {
			std::cout << "'" <<  scan_pattern.name() << "' is a custom named scan pattern."<< std::endl;
		}
	}

	// Set default scan pattern by name
	scanner->set_scan_pattern("Default");

	// Create new scan pattern
	blickfeld::protocol::config::ScanPattern new_scan_pattern;
	new_scan_pattern.mutable_vertical()->set_scanlines_up(100);
	new_scan_pattern.mutable_vertical()->set_scanlines_down(100);
	new_scan_pattern.mutable_pulse()->set_frame_mode(
		blickfeld::protocol::config::ScanPattern::Pulse::FrameMode::ScanPattern_Pulse_FrameMode_COMBINE_UP_DOWN);

	// Fill scan pattern to add missing parameter (optional)
	new_scan_pattern = scanner->fill_scan_pattern(new_scan_pattern);

	// Store scan pattern as "python test"
	// This method persists the scan pattern on the device. Use 'scanner->delete_named_scan_pattern("cpp-test")' to delete it again.
	scanner->store_named_scan_pattern("cpp-test", new_scan_pattern);

	// Check if the stored scan pattern was correctly set (optional, just for visualisation)
	named_scan_patterns = scanner->get_named_scan_patterns();
	for (auto const& scan_pattern : named_scan_patterns) {
		if (scan_pattern.name() == "cpp-test") {
			std::cout << "'" << scan_pattern.name() << "' succesfully stored.\n";
		}
	}

	// Set scan pattern by name
	scanner->set_scan_pattern("cpp-test");

	// Delete scan pattern by name, comment in the next line to try it out
	// scanner->delete_named_scan_pattern("cpp-test");

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
