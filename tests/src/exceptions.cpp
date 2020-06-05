/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include "gtest/gtest.h"

#include "blickfeld/utils.h"
#include "blickfeld/exception.h"

#include <sstream>
using namespace std;

void throw_long_exceptions() {
	stringstream ss;
	for(unsigned int i = 0; i < 500; i++) {
		try {
			ss << "much much more test here. this is a big text that we will copy a few times to test reallocation stuff\n";
			throw blickfeld::str_exception_tb("this is a test exception!\ndata: %s", ss.str().c_str());
		}
		catch(const std::exception& e) {
			// printf("main caught test exception: %d\n", (unsigned int)strlen(e.what()));
		}
	}
}

TEST(exceptions, long_messages) {
#ifndef HAVE_WINDOWS_H
	ASSERT_NO_THROW(({ throw_long_exceptions(); } ));
#endif
}

void throw_eagain() {
	errno = EAGAIN;
	throw blickfeld::errno_exception_tb("something did set errno");
}

TEST(exceptions, errno_text) {
#ifndef HAVE_WINDOWS_H
	ASSERT_ANY_THROW(({ throw_eagain(); }));
#endif
}
