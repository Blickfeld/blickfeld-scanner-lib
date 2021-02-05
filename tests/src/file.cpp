/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include <stdio.h>
#include <fstream>

#include "blickfeld_test.h"
#include <blickfeld/scanner.h>

#include "simulated_connection.h"

class file_simulate_record_and_read : public ::testing::TestWithParam<string> {
};

TEST_P(file_simulate_record_and_read, simulate_record_and_read) {
	{
		auto connection = std::make_shared<network::simulated_connection>(GetParam(), nullptr, false);

		// Open dump file
		std::ofstream dump_file;
		dump_file.open("dump_simulate.bf", std::ios::out | std::ios::trunc | std::ios::binary);

		// Start stream and enable recording
		auto stream = std::make_shared<scanner::point_cloud_stream<protocol::data::frame_t> >(connection);
		stream->record_to_stream(&dump_file);

		// Receive & record frames
		int n_frames = 7;
		while (n_frames--)
			std::cout << "Received and dumped " << stream->recv_frame() << std::endl;

		// Stop stream and close file afterwards
		stream = nullptr;
		dump_file.close();
	}

	{
		// Open dump file
		std::ifstream dump_file;
		dump_file.open ("dump_simulate.bf", std::ios::in | std::ios::binary);

		// Open stream
		auto stream = blickfeld::scanner::simple_file_point_cloud_stream(&dump_file);

		// Read & parse frames
		while (!stream->end_of_stream())
			std::cout << "Read and parsed " << stream->recv_frame() << std::endl;

		// Stop stream and close file afterwards
		stream->stop_recording();
		dump_file.close();
	}
}

INSTANTIATE_TEST_CASE_P(
	simulate_record_and_read,
	file_simulate_record_and_read,
	::testing::Values(
		"../tests/assets/dump.bfpc",
		"../tests/assets/packed.bfpc"
		));
