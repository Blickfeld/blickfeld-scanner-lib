##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
cmake_minimum_required (VERSION 2.8.12)

include(CTest)

IF(NOT HAVE_GOOGLETEST)
  option(BUILD_SHARED_LIBS "" ON)
  add_subdirectory("googletest" EXCLUDE_FROM_ALL)
ENDIF()

if (NOT BSL_STANDALONE AND BSL_RECORDING)
	set (BSL_RECORDING_TESTS
	  src/file.cpp
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

if (CMAKE_BUILD_TYPE STREQUAL "TestCoverage")
  add_custom_target(gcovr
    COMMAND mkdir -p coverage/html coverage/cobertura coverage/sonarqube
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  add_custom_command(TARGET gcovr
    COMMAND echo "== executing tests - .."
    COMMAND ./tests/bf-scanner-lib-test || true
    COMMAND echo "== executing tests - OK"
    COMMAND echo "== collecting coverage information - .."
    COMMAND gcovr --config ../tests/gcovr.cfg
    COMMAND echo "== collecting coverage information - OK"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  )
  add_dependencies(gcovr bf-scanner-lib-test)
endif()