##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
cmake_minimum_required (VERSION 2.8.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT TARGET blickfeld-scanner)
    find_package(blickfeld-scanner REQUIRED full)
endif()

add_executable(bf-named_scan_pattern main.cpp)
target_link_libraries(bf-named_scan_pattern blickfeld-scanner)
