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

	auto stream = scanner->get_simple_point_cloud_stream();

	while (true) {
		const blickfeld::protocol::data::frame_t frame = stream->recv_frame();

		time_t time_s = frame.start_time_ns / 1e9;
		auto timepoint = localtime(&time_s);
		printf ("Frame: points %lu - %0.2f Hz - timestamp %f - %s",
			(unsigned long)frame.points.size(),
			frame.frame_rate,
			frame.start_time_ns / 1e9,
			asctime(timepoint)
			);

		// Example for point iteration
		for (unsigned int i = 0; i < frame.points.size(); i++) {
			auto& point = frame.points[i];

			if (i < 10)
				printf("Point %u [x: %4.2f, y: %4.2f, z: %4.2f] - intensity: %u\n",
				       point.id,
				       point.x, point.y, point.z,
				       point.intensity);
		}
	}
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
