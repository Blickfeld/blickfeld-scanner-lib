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

	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	std::cout << "Connected to " << scanner_ip_or_host << std::endl;

	blickfeld::protocol::config::ScanPattern scan_pattern;
	scan_pattern.mutable_vertical()->set_fov(30.f / 180.f * 3.1415f);
	scan_pattern.mutable_vertical()->set_scanlines_up(40);
	scan_pattern.mutable_vertical()->set_scanlines_down(30);
	scan_pattern.mutable_pulse()->set_frame_mode(
		blickfeld::protocol::config::ScanPattern::Pulse::FrameMode::ScanPattern_Pulse_FrameMode_COMBINE_UP_DOWN);

	scan_pattern = scanner->fill_scan_pattern(scan_pattern);
	scanner->set_scan_pattern(scan_pattern);

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
