##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
project(bf-external)

include(ExternalProject)

ExternalProject_Add(zlib
    GIT_REPOSITORY     https://github.com/madler/zlib.git
    CMAKE_ARGS         "-DBUILD_SHARED_LIBS=ON" 
                       "-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}/install"
                       "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install"
)

ExternalProject_Add(protobuf
    GIT_REPOSITORY     https://github.com/protocolbuffers/protobuf.git
    SOURCE_SUBDIR      "cmake/"
    CMAKE_ARGS         "-Dprotobuf_BUILD_TESTS=OFF" 
                       "-Dprotobuf_BUILD_EXAMPLES=OFF" 
                       "-Dprotobuf_PROTOC_BINARIES=ON" 
                       "-Dprotobuf_MSVC_STATIC_RUNTIME=OFF"
                       "-DBUILD_SHARED_LIBS=ON" 
                       "-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}/install"
                       "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install"
    DEPENDS zlib
)

ExternalProject_Add(bsl
    SOURCE_DIR         ${PROJECT_SOURCE_DIR}/../..
    CMAKE_ARGS         "-DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}/install"
                       "-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/install"
    BUILD_ALWAYS	   ON
    DEPENDS protobuf
)
