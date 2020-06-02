/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include "blickfeld/utils.h"
#include "blickfeld/scanner-config.h"
#include "blickfeld/exception.h"

#include <stdio.h>
#include <sstream>
#include <fstream>

#define FS_BUFFER_SIZE 1024

using namespace std;

namespace blickfeld {
namespace os {

#define NS_PER_SEC (1000 * US_PER_SEC)
#define US_PER_SEC (1000 * MS_PER_SEC)
#define MS_PER_SEC 1000

double get_time() {
#if defined(HAVE_CLOCK_GETTIME)
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;

#elif defined(HAVE_WINDOWS_H)
	SYSTEMTIME t;
	GetSystemTime(&t);
	time_t time_offset = time(NULL);
	return (double)time_offset + (double)t.wMilliseconds / 1e3;

#else
	return time(NULL);
#endif
}

uint64_t get_time_ns() {
#if defined(HAVE_CLOCK_GETTIME)
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (uint64_t)ts.tv_sec * NS_PER_SEC + (uint64_t)ts.tv_nsec;

#elif defined(HAVE_WINDOWS_H)
	SYSTEMTIME t;
	GetSystemTime(&t);
	time_t time_offset = time(NULL);
	return (uint64_t)time_offset * NS_PER_SEC + (uint64_t)t.wMilliseconds * 1000000;
#else
	return time(NULL) * NS_PER_SEC;
#endif
}

string get_time_string(double ts) {
	time_t seconds = (time_t)ts;
	unsigned int milliseconds = (unsigned int)((ts - seconds) * 1e3);
	if(milliseconds >= 1000) {
		seconds++;
		milliseconds -= 1000;
	}
	struct tm* btime = localtime(&seconds);
	char tsstr[32];
	strftime(tsstr, 32, "%Y-%m-%d %H:%M:%S.", btime); // 20 bytes
	snprintf(tsstr + 20, 12, "%03d", milliseconds);
	return string(tsstr, 23);
}

string read_file(string filename) {
	FILE* fp = fopen(filename.c_str(), "rb");
	if(!fp)
		throw errno_exception_tb("could not read file %s", filename.c_str());
	std::stringstream ss;
	while(true) {
		char buffer[FS_BUFFER_SIZE];
		int n = fread(buffer, 1, FS_BUFFER_SIZE, fp);
		if(n == -1)
			throw errno_exception_tb("read_file(%s): error reading!", filename.c_str());
		if(n == 0)
			break;
		ss.write(buffer, n);
	}
	fclose(fp);
	return ss.str();
}

void write_file(string filename, string data) {
	fstream file(filename, ios::out | ios::binary);
	file.exceptions(ifstream::failbit | ifstream::badbit);
	file.write(data.c_str(), data.size());
}

} // namespace os
} // namespace blickfeld
