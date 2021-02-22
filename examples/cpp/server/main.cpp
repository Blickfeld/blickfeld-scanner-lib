/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include <blickfeld/exception.h>
#include <blickfeld/mockup_server.h>

int example(int argc, char* argv[]) {
	std::string dump_fn;
	if (argc > 1)
		dump_fn = argv[1];
	else
		throw blickfeld::str_exception("Please provide filename of recording");

	blickfeld::network::mockup_server server(dump_fn, [](const blickfeld::protocol::stream::Subscribe::PointCloud subscribe) {
	                                         // Handler is called for each client point cloud subscription.
	                                         // Variables can be initialized in this step.

	                                         // Return function, which as called for every frame provided to the client.
						 return [](blickfeld::protocol::data::Frame& frame) {
	                                         // Process & mutate client frame
			};
		});
	return server.serve_forever();
}

int main(int argc, char* argv[]) {
	try {
		return example(argc, argv);
	} catch (const std::exception& e) {
		fprintf(stderr, "main caught exception:\n%s\n", e.what());
	}
	return 1;
}
