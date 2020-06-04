/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#ifndef BLICKFELD_EXCEPTION_H
#define BLICKFELD_EXCEPTION_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <exception>
#include <string>

#ifdef __GNUC__
#define FORMAT_CHECK_ATTR(fmt_idx, first_check_idx) __attribute__((format(printf, fmt_idx, first_check_idx)))
#else
#define FORMAT_CHECK_ATTR(fmt_idx, first_check_idx)
#endif

#define str_exception_tb(format, ...) str_exception("%s:%d %s()\n   " format, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#define errno_exception_tb(format, ...) errno_exception("%s:%d %s()\n   " format, __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#define retval_exception_tb(retval, format, ...) errno_exception(retval, "%s:%d %s()\n   " format, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#define retval_exception(retval, format, ...) errno_exception(retval, format, ## __VA_ARGS__)

#define check_retval(func, ...) do {                                                                    \
		int ret = func(__VA_ARGS__);                                                            \
		if(ret != 0)                                                                            \
			throw retval_exception_tb(ret, #func "()");                                     \
} while(0)

namespace blickfeld {

class str_exception : public std::exception {
protected:
	std::string msg;
	str_exception() {
	}

public:
	str_exception(const char* format, ...) FORMAT_CHECK_ATTR(2, 3);
	~str_exception() noexcept{
	}

	virtual const char* what() const noexcept {
		return msg.c_str();
	}

};

class errno_exception : public str_exception {
public:
	errno_exception(const char* format, ...) FORMAT_CHECK_ATTR(2, 3);
	errno_exception(int retval, const char* format, ...) FORMAT_CHECK_ATTR(3, 4);
};

} // namespace blickfeld

#endif // BLICKFELD_EXCEPTION_H
