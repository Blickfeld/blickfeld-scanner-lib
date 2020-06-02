/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include "blickfeld/scanner-config.h"

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include <stdio.h>

#include "blickfeld/string_utils.h"

using namespace std;

namespace blickfeld {

void append_format_string(char** buffer, unsigned int* buffer_size, unsigned int* buffer_len, const char* format, ...) {
	char*& p = *buffer;
	unsigned int& size = *buffer_size;
	unsigned int offset = *buffer_len;
	unsigned int& len = *buffer_len;
	append_vsn_format(format, size, len, p, offset);
}

string format_string(const char* format, ...) {
	vsn_format(format, size, len, str);
	string ret(str, len);
	free(str);
	return ret;
}

vector<string> split_string(string input, string by, unsigned int max_split) {
	vector<string> out;

	string::size_type s = 0;
	while(s < input.size()) {
		string::size_type p = input.find(by, s);
		if(p == string::npos) {
			out.push_back(input.substr(s));
			break;
		}
		out.push_back(input.substr(s, p - s));
		s = p + by.size();

		if(max_split > 0 && out.size() == max_split) {
			out.push_back(input.substr(s));
			break;
		}
	}
	return out;
}

string strip(string input, string white) {
	string::size_type start = input.find_first_not_of(white);
	if(start == string::npos)
		return "";
	string::size_type end = input.find_last_not_of(white);
	return input.substr(start, end - start + 1);
}

} // namespace blickfeld
