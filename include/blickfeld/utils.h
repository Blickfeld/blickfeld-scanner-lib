/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#ifndef BLICKFELD_UTILS_H
#define BLICKFELD_UTILS_H

#include "blickfeld/scanner-config.h"

#ifdef HAVE_WINDOWS_H
#  include <winsock2.h>
#  include <ws2tcpip.h>
#endif

#if defined(HAVE_TIME_H)
#  include <time.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif

#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif

#ifdef HAVE_NETINET_IP_H
#  include <netinet/ip.h>
#endif

#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif

#ifdef HAVE_NETDB_H
#  include <netdb.h>
#endif

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif

#ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
#endif
#include <string>

#define _USE_MATH_DEFINES
#include <cmath>

namespace blickfeld {

namespace os {

double get_time();
uint64_t get_time_ns();
std::string get_time_string(double ts);
std::string read_file(std::string filename);
void write_file(std::string filename, std::string data);
float be32tofloat(uint32_t be);
uint32_t floattobe32(float val);

}

namespace protocol {
namespace data {
class Frame;

int get_total_number_of_scanlines(const Frame& frame);
int number_of_points_per_scanline(const Frame& frame);
int get_scanline_id_by_point_id(const Frame& frame, const uint32_t point_id);

} // namespace data
} // namespace protocol

}

#endif // BLICKFELD_UTILS_H
