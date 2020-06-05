##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
cmake_minimum_required (VERSION 2.6)

include(CTest)

IF(NOT HAVE_GOOGLETEST)
  option(BUILD_SHARED_LIBS "" ON)
  add_subdirectory("googletest" EXCLUDE_FROM_ALL)
ENDIF()

if (BSL_RECORDING)
	set (BSL_RECORDING_TESTS
	  src/file.cpp
	  src/simulated_connection.cpp
	)
endif()		

add_executable(bf-scanner-lib-test
  src/main.cpp
  src/exceptions.cpp
  ${BSL_RECORDING_TESTS}
)
target_link_libraries(bf-scanner-lib-test gtest_main blickfeld-scanner-static)
add_test (
  NAME gtests
  COMMAND $<TARGET_FILE_NAME:bf-scanner-lib-test>
)

