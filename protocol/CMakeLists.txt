##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
cmake_minimum_required (VERSION 3.9)
include(ExternalProject)

project(blickfeld-protocol)

# NPM
find_program(NODEJS_EXECUTABLE NAMES node)
find_program(NPM_EXECUTABLE NAMES npm HINTS /usr)
if (BF_BUILD_NPM AND NODEJS_EXECUTABLE AND NPM_EXECUTABLE)
	if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
		set(PBJS_PATH ${CMAKE_CURRENT_BINARY_DIR}/npm-install/pbjs.cmd)
		set(PBTS_PATH ${CMAKE_CURRENT_BINARY_DIR}/npm-install/pbts.cmd)
	else()
		set(PBJS_PATH ${CMAKE_CURRENT_BINARY_DIR}/npm-install/node_modules/.bin/pbjs)
		set(PBTS_PATH ${CMAKE_CURRENT_BINARY_DIR}/npm-install/node_modules/.bin/pbts)
	endif()
		
	add_custom_command(
		OUTPUT ${PBJS_PATH}
		OUTPUT ${PBTS_PATH}
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/npm-install
		COMMAND ${NPM_EXECUTABLE} install protobufjs --prefix ${PROJECT_BINARY_DIR}/npm-install
	)
	add_custom_target(blickfeld-protocol-npm DEPENDS ${PBJS_PATH} DEPENDS ${PBTS_PATH})
endif()
	
## Protocol Buffers
if(NOT BF_USE_SYSTEM_PROTOBUF)
	message("Protobuf compiler is compiled from source")
	ExternalProject_Add(
	  blickfeld-protobuf
	  SOURCE_DIR ${PROTOBUF_SRC_DIR}
	  SOURCE_SUBDIR cmake
	  CMAKE_ARGS
	    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
	  	"-Dprotobuf_BUILD_TESTS=OFF"
	  	"-Dprotobuf_BUILD_EXAMPLES=OFF"
	  	"-Dprotobuf_BUILD_PROTOC_BINARIES=ON"
	  	"-Dprotobuf_MSVC_STATIC_RUNTIME=OFF"
	  	"-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/protobuf"
	)
	set(Protobuf_PROTOC_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/protobuf/bin/protoc)
	set(BF_PROTOBUF_TARGET blickfeld-protobuf)
else()
	message("Existing protobuf compiler is used: ${Protobuf_PROTOC_EXECUTABLE}")
endif()
set(Protobuf_PROTOC_EXECUTABLE ${Protobuf_PROTOC_EXECUTABLE} PARENT_SCOPE)

# plugin for protobuf compilation
if(BF_BUILD_PROTOBUF_PLUGIN)
	find_program(BF_PROTOCOL_PLUGIN NAMES blickfeld-protocol-plugin)
	if(BF_PROTOCOL_PLUGIN)
		set(BF_PROTOCOL_PLUGIN_PATH ${BF_PROTOCOL_PLUGIN})
	else()
		message("blickfeld-protocol-plugin is compiled from source")
		set(BF_PROTOCOL_PLUGIN blickfeld-protocol-plugin)
		ExternalProject_Add(
			blickfeld-protocol-plugin
			SOURCE_DIR ${PROJECT_SOURCE_DIR}/plugin
			DEPENDS ${BF_PROTOBUF_TARGET}
			CMAKE_ARGS
				"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
				"-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/protobuf"
		)
		if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
				set(BF_PROTOCOL_PLUGIN_PATH "${CMAKE_CURRENT_BINARY_DIR}/protobuf/bin/blickfeld-protocol-plugin.exe")
		else()
				set(BF_PROTOCOL_PLUGIN_PATH "${CMAKE_CURRENT_BINARY_DIR}/protobuf/bin/blickfeld-protocol-plugin")
		endif()
	endif()
	message("Using blickfeld-protocol-plugin: ${BF_PROTOCOL_PLUGIN}")
endif()

# list .proto files here
set(PROTOBUF_DEFS_DIR ${PROJECT_SOURCE_DIR})
set(PROTOBUF_SOURCES	
	blickfeld/config/algorithm.proto
	blickfeld/config/advanced.proto
	blickfeld/config/product.proto
	blickfeld/config/generate.proto
	blickfeld/config/scan_pattern.proto
	blickfeld/config/secure.proto
	blickfeld/config/named_scan_pattern.proto
	
	blickfeld/status/main.proto
	blickfeld/status/scanner.proto
	blickfeld/status/temperature.proto
	blickfeld/status/server.proto
	
	blickfeld/data/algorithm.proto
	blickfeld/data/frame.proto
	blickfeld/data/point_cloud.proto
	blickfeld/data/point.proto
	blickfeld/data/scanline.proto
	
	blickfeld/file/general.proto
	blickfeld/file/point_cloud.proto
	
	blickfeld/stream/connection.proto
	blickfeld/stream/event.proto

	blickfeld/update/hardware.proto
	blickfeld/update/manifest.proto
	
	blickfeld/common.proto
	blickfeld/options.proto
	blickfeld/error.proto
	blickfeld/connection.proto
)

set(PROTOBUF_SOURCES_CUSTOM
	blickfeld/error.proto
)

set(PROTOBUF_GENERATED_PY_DIR ${PROJECT_BINARY_DIR}/python)

IF(WIN32)
	set(DLL_EXPORT "dllexport_decl=BF_DLLEXPORT:")
ENDIF()

set(PROTOBUF_GENERATED_CPP)
set(PROTOBUF_GENERATED_HEADER)
set(PROTOBUF_GENERATED_PY)
set(PROTOBUF_GENERATED_JS)

set(PROTOBUF_SOURCES_WITH_ABSOLUTE_PATH "")

foreach(FILE ${PROTOBUF_SOURCES})
	list(APPEND PROTOBUF_SOURCES_WITH_ABSOLUTE_PATH "${PROTOBUF_DEFS_DIR}/${FILE}")

	string(REPLACE ".proto" ".pb.cc" FILE_CC ${FILE})
	string(REPLACE ".proto" ".pb.h" FILE_H ${FILE})
	
	# Cpp Build
	add_custom_command(
		OUTPUT ${PROJECT_BINARY_DIR}/${FILE_CC}
		OUTPUT ${LIBINCLUDEDIR_TOP}/${FILE_H}
		
		#COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}
		COMMAND ${Protobuf_PROTOC_EXECUTABLE} ${PROTOBUF_DEFS_DIR}/${FILE} --cpp_out=${DLL_EXPORT}${PROJECT_BINARY_DIR} --proto_path=${PROTOBUF_DEFS_DIR}
		COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${FILE_H} ${LIBINCLUDEDIR_TOP}/${FILE_H}
		COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_BINARY_DIR}/${FILE_H}
		
		DEPENDS $<TARGET_NAME_IF_EXISTS:blickfeld-protobuf>
		DEPENDS ${FILE}
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)
	list(APPEND PROTOBUF_GENERATED_CPP ${PROJECT_BINARY_DIR}/${FILE_CC})
	list(APPEND PROTOBUF_GENERATED_HEADER ${LIBINCLUDEDIR_TOP}/${FILE_H})
	
	# Python Build
	if (BF_BUILD_PYTHON)
		string(REPLACE ".proto" "_pb2.py" FILE_PY ${FILE})
		add_custom_command(
			OUTPUT ${PROTOBUF_GENERATED_PY_DIR}/${FILE_PY}
		
			COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/python
			COMMAND ${Protobuf_PROTOC_EXECUTABLE} ${PROTOBUF_DEFS_DIR}/${FILE} --python_out=${PROTOBUF_GENERATED_PY_DIR} --proto_path=${PROTOBUF_DEFS_DIR}
		
			DEPENDS $<TARGET_NAME_IF_EXISTS:blickfeld-protobuf>
			DEPENDS ${FILE}
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		)
	endif()
	list(APPEND PROTOBUF_GENERATED_PY ${PROTOBUF_GENERATED_PY_DIR}/${FILE_PY})
endforeach()

# NPM Build (moved out of the loop as pbjs and pbts accepts all .proto files at once.)
if (BF_BUILD_NPM AND NODEJS_EXECUTABLE AND NPM_EXECUTABLE)
	add_custom_command(
		OUTPUT ${PROJECT_BINARY_DIR}/npm/protobuf-definitions.js
		OUTPUT ${PROJECT_BINARY_DIR}/npm/protobuf-definitions.d.ts
		
		COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/npm
		COMMAND ${PBJS_PATH} -t static-module -w commonjs --force-number -o ${PROJECT_BINARY_DIR}/npm/protobuf-definitions.js ${PROTOBUF_SOURCES_WITH_ABSOLUTE_PATH}
		COMMAND ${PBTS_PATH} -o ${PROJECT_BINARY_DIR}/npm/protobuf-definitions.d.ts ${PROJECT_BINARY_DIR}/npm/protobuf-definitions.js
		
		DEPENDS $<TARGET_NAME_IF_EXISTS:blickfeld-protobuf>
		DEPENDS blickfeld-protocol-npm
		DEPENDS ${PROTOBUF_SOURCES}
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)
	list(APPEND PROTOBUF_GENERATED_JS ${PROJECT_BINARY_DIR}/npm/protobuf-definitions.js)
endif()

# Custom plugin protobuf build
set(PROTOBUF_GENERATED_CUSTOM)
foreach(FILE ${PROTOBUF_SOURCES_CUSTOM})
	string(REPLACE ".proto" ".custom.pb.h" FILE_H ${FILE})

	if(BF_BUILD_PROTOBUF_PLUGIN)
		add_custom_command(
			OUTPUT ${LIBINCLUDEDIR_TOP}/${FILE_H}
			COMMAND ${Protobuf_PROTOC_EXECUTABLE} ${PROTOBUF_DEFS_DIR}/${FILE} --plugin=protoc-gen-custom=${BF_PROTOCOL_PLUGIN_PATH} --custom_out=${DLL_EXPORT}${PROJECT_BINARY_DIR} --proto_path=${PROTOBUF_DEFS_DIR}
			COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/${FILE_H} ${LIBINCLUDEDIR_TOP}/${FILE_H}
			COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_BINARY_DIR}/${FILE_H}

			DEPENDS $<TARGET_NAME_IF_EXISTS:blickfeld-protobuf>
			DEPENDS ${BF_PROTOCOL_PLUGIN}
			DEPENDS ${FILE}
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		)
	else()  # if plugin build is deactivated the header files will just be copied
		add_custom_command(
			OUTPUT ${LIBINCLUDEDIR_TOP}/${FILE_H}
			COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/plugin/${FILE_H} ${LIBINCLUDEDIR_TOP}/${FILE_H}
			DEPENDS ${FILE}
			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		)
	endif()
	list(APPEND PROTOBUF_GENERATED_CUSTOM ${LIBINCLUDEDIR_TOP}/${FILE_H})
endforeach()

add_library(blickfeld-scanner-protocol-static OBJECT ${PROTOBUF_GENERATED_CUSTOM} ${PROTOBUF_GENERATED_CPP})
target_include_directories(blickfeld-scanner-protocol-static PRIVATE ${LIBINCLUDEDIR_TOP})
target_link_libraries(blickfeld-scanner-protocol-static PUBLIC ${Protobuf_LIBRARY})
set_property(TARGET blickfeld-scanner-protocol-static PROPERTY POSITION_INDEPENDENT_CODE ON)

if(WIN32)
	if(NOT BSL_STANDALONE)
		target_compile_definitions(blickfeld-scanner-protocol-static PRIVATE "BF_DLLEXPORT=__declspec(dllexport)" "PROTOBUF_USE_DLLS")
	else()
		target_compile_definitions(blickfeld-scanner-protocol-static PRIVATE "BF_DLLEXPORT=__declspec(dllexport)")
	endif()
	
	# https://github.com/protocolbuffers/protobuf/blob/master/cmake/README.md#notes-on-compiler-warnings
	target_compile_options(blickfeld-scanner-protocol-static PRIVATE "/wd4251" "/wd4244" "/wd4267")
endif()

set(PROTOBUF_GENERATED_CPP ${PROTOBUF_GENERATED_CPP} PARENT_SCOPE)
set(PROTOBUF_GENERATED_PY_DIR ${PROTOBUF_GENERATED_PY_DIR} PARENT_SCOPE)
set(PROTOBUF_GENERATED_PY ${PROTOBUF_GENERATED_PY} PARENT_SCOPE)
set(PROTOBUF_GENERATED_JS ${PROTOBUF_GENERATED_JS} PARENT_SCOPE)
set(PROTOBUF_GENERATED_JS_DIR ${PROJECT_BINARY_DIR}/npm PARENT_SCOPE)

add_custom_target(blickfeld-scanner-protocol 
	DEPENDS blickfeld-scanner-protocol-static
	DEPENDS ${PROTOBUF_GENERATED_CPP}
	DEPENDS ${PROTOBUF_GENERATED_PY})

add_custom_target(blickfeld-scanner-protocol-js
	DEPENDS ${PROTOBUF_GENERATED_JS})
