/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include <iostream>
#include <math.h>
#include <blickfeld/scanner.h>
#include <blickfeld/utils.h>
#include <blickfeld/protocol_exception.h>

int example(int argc, char* argv[]) {
	std::string scanner_ip_or_host = "localhost";
	if (argc > 1) scanner_ip_or_host = argv[1];

	std::shared_ptr<blickfeld::scanner> scanner;

	// connect to scanner
	// -- catch all possible exceptions
	try {
		scanner = blickfeld::scanner::connect(scanner_ip_or_host);
	} catch (const std::exception& e) {
		fprintf(stderr, "connect failed: %s\n", e.what());
		return -1;
	}

	// generate impossible scan pattern to scanner (scanlines-up := 16 @ vertical-fov := 30[deg])
	// -- catch only exception of interest
	blickfeld::protocol::config::ScanPattern scan_pattern;
	scan_pattern.mutable_horizontal()->set_fov(10.f / 180.f * 3.1415f);
	scan_pattern.mutable_vertical()->set_fov(30.f / 180.f * 3.1415f);
	scan_pattern.mutable_vertical()->set_scanlines_up(10);
	scan_pattern.mutable_vertical()->set_scanlines_down(30);
	scan_pattern.mutable_pulse()->set_frame_mode(blickfeld::protocol::config::ScanPattern::Pulse::FrameMode::ScanPattern_Pulse_FrameMode_COMBINE_UP_DOWN);

	// -- this will generate a NotInRange exception with additional information
	try {
		scan_pattern = scanner->fill_scan_pattern(scan_pattern);
	} catch (const blickfeld::protocol_exception<blickfeld::protocol::Error_NotInRange>& e) {
		fprintf(stderr, "%s\n", e.what());
	}

	// -- since we try to send an incomplete message this will generate an InvalidRequest exception
	try {
		scanner->set_scan_pattern(scan_pattern);
	} catch (const blickfeld::protocol_exception<blickfeld::protocol::Error_InvalidRequest>& e) {
		fprintf(stderr, "%s\n", e.what());
	}

	return 0;
}

int main(int argc, char** argv) {
	try {
		return example(argc, argv);
	} catch (const std::exception& e) {
		fprintf(stderr, "main caught exception: %s\n", e.what());
	}
	return 1;
}
