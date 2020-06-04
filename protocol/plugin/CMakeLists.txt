##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
cmake_minimum_required (VERSION 3.9)

project(blickfeld-protocol-plugin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(SOURCES 
	bf_protobuf_plugin.cpp
)

find_package(Protobuf PATHS "${CMAKE_INSTALL_PREFIX}/cmake" NO_DEFAULT_PATH)
if(NOT TARGET protobuf::libprotoc)
	find_package(Protobuf REQUIRED)
	include_directories(${PROTOBUF_INCLUDE_DIR})
else()
	set(Protobuf_LIBRARIES protobuf::libprotobuf)
	set(Protobuf_PROTOC_LIBRARIES protobuf::libprotoc)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(blickfeld-protocol-plugin ${SOURCES})

option(BF_PLUGIN_USE_OLD_CXX11_ABI "Compile with old C++11 ABI for Petalinux compatibility" OFF)
if(BF_PLUGIN_USE_OLD_CXX11_ABI)
	target_compile_options(blickfeld-protocol-plugin PRIVATE "SHELL:-D _GLIBCXX_USE_CXX11_ABI=0")
endif()

target_link_libraries(blickfeld-protocol-plugin PRIVATE ${Protobuf_PROTOC_LIBRARIES} ${Protobuf_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS blickfeld-protocol-plugin DESTINATION bin)
