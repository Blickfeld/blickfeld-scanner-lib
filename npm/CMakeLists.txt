##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
cmake_minimum_required (VERSION 3.9)
include(ExternalProject)

project(blickfeld-npm)

find_program(NODEJS_EXECUTABLE NAMES node)
find_program(NPM_EXECUTABLE NAMES npm HINTS /usr)
if (NODEJS_EXECUTABLE AND NPM_EXECUTABLE)
	message("Using local NPM ${NPM_EXECUTABLE}")
	add_custom_command(
		OUTPUT ${PROJECT_BINARY_DIR}/package-lock.json
		COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/package.json ${PROJECT_BINARY_DIR}
		COMMAND ${NPM_EXECUTABLE} install
		DEPENDS ${PROJECT_SOURCE_DIR}/package.json
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)
	
	set(NPM_PACKAGE ${PROJECT_BINARY_DIR}/blickfeld-scanner-${PROJECT_GIT_DESCRIBE}.tgz)
	add_custom_command(
		OUTPUT ${NPM_PACKAGE}
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROTOBUF_GENERATED_JS_DIR} ${PROJECT_BINARY_DIR}
		COMMAND ${NPM_EXECUTABLE} version --no-git-tag-version --allow-same-version ${PROJECT_GIT_DESCRIBE}
		COMMAND ${NPM_EXECUTABLE} run build
		COMMAND ${NPM_EXECUTABLE} pack
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		DEPENDS blickfeld-scanner-protocol-js
		DEPENDS ${PROTOBUF_GENERATED_JS}
		DEPENDS ${PROJECT_SOURCE_DIR}/index.ts
		DEPENDS ${PROJECT_SOURCE_DIR}/tsconfig.json
		DEPENDS ${PROJECT_BINARY_DIR}/package-lock.json
	)
	
	add_custom_target(blickfeld-scanner-npm ALL DEPENDS ${NPM_PACKAGE})
	
endif()
