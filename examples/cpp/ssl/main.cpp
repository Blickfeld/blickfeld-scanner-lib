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
	if (argc < 2) return 1;
	std::string cert_filename = argv[1];

	std::string scanner_ip_or_host = "localhost";
	if (argc > 2) scanner_ip_or_host = argv[2];

	std::string port = "8000";
	if (argc > 3) port = argv[3];

	std::string ssl_port = "8800";
	if (argc > 4) ssl_port = argv[4];

	std::shared_ptr<blickfeld::scanner> scanner = blickfeld::scanner::connect(scanner_ip_or_host+":"+port);
	std::cout << "Connected to " << scanner_ip_or_host << " on port " << port << std::endl;

	std::shared_ptr<blickfeld::scanner> ssl_scanner = blickfeld::scanner::connect(scanner_ip_or_host+":"+ssl_port, cert_filename);
	std::cout << "Connected with ssl to " << scanner_ip_or_host << " on port " << ssl_port << std::endl;

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
