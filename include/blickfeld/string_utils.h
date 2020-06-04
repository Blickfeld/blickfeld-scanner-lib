/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#ifndef BLICKFELD_STRING_UTILS_H
#define BLICKFELD_STRING_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string>
#include <vector>

#include "blickfeld/exception.h"

#define append_vsn_format(format, size, len, p, offset)                         \
	va_list ap;                                                             \
	int lenret;                                                             \
	while(true) {                                                           \
		va_start(ap, format); /* needed for gcc 4.8.2 on x86_64 */      \
		lenret = vsnprintf(p + offset, size - offset, format, ap);      \
		va_end(ap);                                                     \
		if (lenret > -1 && static_cast<unsigned>(lenret) < (size - offset)) {        \
			len = static_cast<unsigned>(lenret);                                           \
			break;                                                  \
		}                                                               \
		if (lenret > -1)    /* glibc 2.1 */                             \
			size = offset + static_cast<unsigned>(lenret) + 1;                             \
		else           /* glibc 2.0 */                                  \
			size *= 2;  /* twice the old size */                    \
		char* np;                                                       \
		if ((np = static_cast<char *>(realloc(p, size))) == NULL) {                   \
			free(p);                                                \
			throw std::bad_alloc();                                 \
		}                                                               \
		p = np;                                                         \
	}                                                                       \
	len = static_cast<unsigned>(lenret) + offset

#define vsn_format(format, size, len, p)                                        \
	unsigned int size = static_cast<unsigned>(strlen(format));                                     \
	if(size < 256)                                                          \
		size = 256;                                                     \
                                                                                \
	char* p;                                                                \
	if ((p = static_cast<char*>(malloc(size))) == NULL)                                  \
		throw std::bad_alloc();                                         \
                                                                                \
	unsigned int len = 0;                                                   \
	append_vsn_format(format, size, len, p, 0)

namespace blickfeld {

void append_format_string(char** buffer, unsigned int* buffer_size, unsigned int* buffer_len, const char* format, ...) FORMAT_CHECK_ATTR(4, 5);
std::string format_string(const char* format, ...) FORMAT_CHECK_ATTR(1, 2);
std::vector<std::string> split_string(std::string input, std::string by, unsigned int max_split=0);
std::string strip(std::string input, std::string white);

} // namespace blickfeld

#endif // BLICKFELD_STRING_UTILS_H
