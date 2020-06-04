/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include <blickfeld/logged_object.h>

class test_class : public blickfeld::logged_object {
public:
	test_class() : blickfeld::logged_object("test_class") {
		log_debug("debug!\n");
		log_verbose("verbose!\n");
		log_info("info!\n");
		log_warning("warning!\n");
		log_error("error!\n");
	}

};

int main(int argc, char* argv[]) {
	test_class tst;
	return 0;
}
