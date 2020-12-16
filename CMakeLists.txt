##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
cmake_minimum_required (VERSION 3.12)
#cmake_policy(SET CMP0091 NEW)

project (blickfeld-scanner-lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
set(CMAKE_DEBUG_POSTFIX d)

# Options
option(BF_USE_SYSTEM_PROTOBUF "Use system wide protobuf" ON)
option(BF_BUILD_EXAMPLES "Build ${CMAKE_PROJECT_NAME} examples" ON)  # Build cpp examples, defaults is ON
option(BF_BUILD_TESTS "Build ${CMAKE_PROJECT_NAME} tests" ON)  # Build tests for the BSL, defaults is ON
option(BF_BUILD_PYTHON "Build ${CMAKE_PROJECT_NAME} python package" ON)  # Build python package blickfeld_scanner, defaults is ON
option(BF_ENABLE_SSL "Enable ${CMAKE_PROJECT_NAME} SSL support (internal feature)" OFF)  # Enable SSL support for internal features, defaults is OFF (not needed for open-source BSL compilation)
option(BF_INSTALL_EXAMPLES "Install ${CMAKE_PROJECT_NAME} C++ examples" ON)  # Install cpp examples like bf_discovery on system, defaults is ON
option(BF_BUILD_PROTOBUF_PLUGIN "Build ${CMAKE_PROJECT_NAME} protobuf plugin" OFF)  # Build protobuf plugin for develop mode. The binary is also provided in the repository.

# NPM is not yet released. Folder may be missing.
if (EXISTS ${PROJECT_SOURCE_DIR}/npm)
    option(BF_BUILD_NPM "Build ${CMAKE_PROJECT_NAME} NPM package" ON)  # Build npm package, defaults is ON
else()
    set(BF_BUILD_NPM OFF)
endif()

# Get version of BSL
if(EXISTS "${PROJECT_SOURCE_DIR}/version.txt")
    file (READ "${PROJECT_SOURCE_DIR}/version.txt" PROJECT_VERSION)  
    file (READ "${PROJECT_SOURCE_DIR}/version.txt" PROJECT_GIT_DESCRIBE)    
else()
  execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE PROJECT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  execute_process(
    COMMAND git describe --tags --abbrev=8
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE PROJECT_GIT_DESCRIBE
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # Remove first letter 'g' after version
  string(REGEX REPLACE "(v[0-9.-]+-)g" "\\1" PROJECT_GIT_DESCRIBE "${PROJECT_GIT_DESCRIBE}")

  IF(NOT PROJECT_VERSION)
      file (READ "${PROJECT_SOURCE_DIR}/version.txt" PROJECT_VERSION)  
      file (READ "${PROJECT_SOURCE_DIR}/version.txt" PROJECT_GIT_DESCRIBE)    
  ENDIF()
endif()

string(REGEX REPLACE "^v([0-9.]+).*" "\\1" PROJECT_VERSION "${PROJECT_VERSION}")
string(REGEX REPLACE "^v" "" PROJECT_GIT_DESCRIBE "${PROJECT_GIT_DESCRIBE}")
string(REPLACE "." ";" VERSION_LIST ${PROJECT_VERSION})
list(GET VERSION_LIST 0 BSL_VERSION_MAJOR)
list(GET VERSION_LIST 1 BSL_VERSION_MINOR)
list(GET VERSION_LIST 2 BSL_VERSION_PATCH)

message("BSL Version ${PROJECT_GIT_DESCRIBE}")
project (blickfeld-scanner-lib 
	VERSION ${PROJECT_VERSION} 
	DESCRIPTION "Blickfeld Scanner Library for controlling LiDAR devices of the Blickfeld GmbH"
	HOMEPAGE_URL https://www.blickfeld.com
)
include(GNUInstallDirs)

set(SERVER_PORT 8000)

include (CheckIncludeFiles)

# Set a default build type for single-configuration
# CMake generators if no build type is set.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)	
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

check_include_files (windows.h HAVE_WINDOWS_H)
check_include_files (sys/socket.h HAVE_SYS_TYPES_H)
check_include_files (sys/socket.h HAVE_SYS_SOCKET_H)
check_include_files (netinet/in.h HAVE_NETINET_IN_H)
check_include_files (netinet/ip.h HAVE_NETINET_IP_H)
check_include_files (arpa/inet.h HAVE_ARPA_INET_H)
check_include_files (netdb.h HAVE_NETDB_H)
check_include_files (unistd.h HAVE_UNISTD_H)
check_include_files (syslog.h HAVE_SYSLOG_H)
check_include_files (time.h HAVE_TIME_H)
check_include_files (inttypes.h HAVE_INTTYPES_H)
check_include_files (winsock.h HAVE_WINSOCK_H)

# Check for Windows Build
if(MSVC)
	# Reduce warning level to hide protobuf warnings
    add_compile_options(/EHsc)
endif(MSVC)
if(WIN32)
	macro(get_WIN32_WINNT version)
	    if (WIN32 AND CMAKE_SYSTEM_VERSION)
	        set(ver ${CMAKE_SYSTEM_VERSION})
	        string(REPLACE "." "" ver ${ver})
	        string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})
	
	        set(${version} "0x${ver}")
	    endif()
	endmacro()
    get_WIN32_WINNT(ver)
	add_definitions(-D_WIN32_WINNT=${ver})
endif(WIN32)

# Check for GNUCC Cmake compiler
if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++11 -static-libgcc -static-libstdc++")
  # generate coverage information in debug builds
  set(CMAKE_CXX_FLAGS_TESTCOVERAGE " -fprofile-arcs -ftest-coverage -O0 -g3")
endif(CMAKE_COMPILER_IS_GNUCC)

include (CheckSymbolExists)
check_symbol_exists(clock_gettime time.h HAVE_CLOCK_GETTIME)
check_symbol_exists(MSG_NOSIGNAL "sys/types.h;sys/socket.h" HAVE_MSG_NOSIGNAL)

set(DEFAULT_LOG_MSGS_KEPT_IN_MEM 50)

# Set lib include directories
set (LIBINCLUDEDIR "include/blickfeld")
set (LIBINCLUDEDIR_TOP "${PROJECT_BINARY_DIR}/include")

# Protobuf
set(PROTOBUF_SRC_DIR "${PROJECT_SOURCE_DIR}/thirdparty/protobuf")
if(BF_USE_SYSTEM_PROTOBUF)
  find_package(Protobuf REQUIRED)

  # Check version of protobuf
  if(PROTOBUF_FOUND AND Protobuf_PROTOC_EXECUTABLE)
		message ("Found protobuf compiler: ${Protobuf_PROTOC_EXECUTABLE}")
		execute_process(COMMAND ${Protobuf_PROTOC_EXECUTABLE} --version OUTPUT_VARIABLE Protobuf_Compiler_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
		string(REGEX REPLACE "^libprotoc " "" Protobuf_Compiler_VERSION "${Protobuf_Compiler_VERSION}")

		if(NOT Protobuf_VERSION STREQUAL Protobuf_Compiler_VERSION)
			set(PROTOBUF_FOUND FALSE)
		endif()
  endif()

  # Only pass if we found protobuf with its executable and library
  if(NOT (PROTOBUF_FOUND AND Protobuf_PROTOC_EXECUTABLE AND Protobuf_LIBRARY))
    set(PROTOBUF_FOUND FALSE)
  endif()
  
  if(PROTOBUF_FOUND)
    message("Full protobuf support active.")
    set(BSL_STANDALONE OFF)  # Set BSL standalone build on off. This will activate more features.
  else()
    message(FATAL_ERROR "Could not find suitable protobuf on this system.")
  endif()
else()  # protobuf not on system and has to be compiled from source
  message("Protobuf is compiled from source. Functionality is reduced. Install protobuf in the system to activate full support.")
  option(protobuf_BUILD_TESTS OFF)
  option(protobuf_BUILD_EXAMPLES OFF)
  option(protobuf_BUILD_PROTOC_BINARIES ON)
  option(protobuf_BUILD_SHARED_LIBS OFF)
  option(protobuf_MSVC_STATIC_RUNTIME OFF)
  add_subdirectory("${PROTOBUF_SRC_DIR}/cmake" EXCLUDE_FROM_ALL)
  set(Protobuf_LIBRARY libprotobuf libprotobuf-lite)
  set(Protobuf_INCLUDE_DIRS ${PROTOBUF_SRC_DIR}/src)
  set_property(TARGET ${Protobuf_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE ON)
  set(BSL_STANDALONE ON)
endif()

# ZLIB
find_package(ZLIB)
if (ZLIB_FOUND)
	set(BSL_RECORDING ON)
	message("Found ZLIB. Recording and reading is enabled.")
else()
	set(BSL_RECORDING OFF)
	message("Did not find ZLIB. Recording and reading is disabled.")
endif()

# OpenSSL
if(BF_ENABLE_SSL)
  set(HAVE_OPENSSL ON)
  find_package(OpenSSL REQUIRED)
else()
  set(HAVE_OPENSSL OFF)
endif()

configure_file("${PROJECT_SOURCE_DIR}/include/blickfeld/scanner-config.h.in" "${PROJECT_BINARY_DIR}/include/blickfeld/scanner-config.h")

# Sources and includes
set(BLICKFELD_SCANNER_SOURCES
  src/string_utils.cpp
  src/exception.cpp
  src/scanner.cpp
  src/utils.cpp
  src/logged_object.cpp
  src/scanner_connection.cpp
  src/connection.cpp
  src/discover.cpp
  src/point_cloud_record.cpp
  src/point_cloud_stream.cpp
)
set(BLICKFELD_SCANNER_INCLUDES
  ${Protobuf_INCLUDE_DIRS}
  include
  src
  thirdparty/portable_endian
  thirdparty/asio/asio/include
  ${PROJECT_BINARY_DIR}/include
)

include_directories(${BLICKFELD_SCANNER_INCLUDES})

# Protocols
add_subdirectory(protocol)

# NPM
if(BF_BUILD_NPM)
  add_subdirectory(npm)
endif()

# Define library
add_library (blickfeld-scanner SHARED ${BLICKFELD_SCANNER_SOURCES} $<TARGET_OBJECTS:blickfeld-scanner-protocol-static>)
add_library (blickfeld-scanner-static STATIC EXCLUDE_FROM_ALL ${BLICKFELD_SCANNER_SOURCES} $<TARGET_OBJECTS:blickfeld-scanner-protocol-static>)
target_include_directories(blickfeld-scanner-static PUBLIC ${BLICKFELD_SCANNER_INCLUDES})

if(WIN32)
  	# https://msdn.microsoft.com/en-us/library/windows/desktop/ms738520(v=vs.85).aspx
  	
	target_link_libraries(blickfeld-scanner PUBLIC ws2_32 wsock32)
	if(NOT BSL_STANDALONE)
		target_compile_definitions(blickfeld-scanner PRIVATE "BF_DLLEXPORT=__declspec(dllexport)" "PROTOBUF_USE_DLLS")
		target_compile_definitions(blickfeld-scanner-static PUBLIC "BF_DLLEXPORT=__declspec(dllexport)" "PROTOBUF_USE_DLLS")
		target_compile_definitions(blickfeld-scanner INTERFACE "BF_DLLEXPORT=__declspec(dllimport)" "PROTOBUF_USE_DLLS")
	else()
		target_compile_definitions(blickfeld-scanner PRIVATE "BF_DLLEXPORT=__declspec(dllexport)")
		target_compile_definitions(blickfeld-scanner-static PUBLIC "BF_DLLEXPORT=__declspec(dllexport)")
	endif()
		
  # https://github.com/protocolbuffers/protobuf/blob/master/cmake/README.md#notes-on-compiler-warnings
  target_compile_options(blickfeld-scanner PUBLIC "/wd4251")
  target_compile_options(blickfeld-scanner-static PUBLIC "/wd4251" "/wd4244" "/wd4267")
endif(WIN32)

add_dependencies(blickfeld-scanner blickfeld-scanner-protocol)
add_dependencies(blickfeld-scanner-static blickfeld-scanner-protocol)

if(NOT BSL_STANDALONE)
	target_link_libraries( blickfeld-scanner PUBLIC $<BUILD_INTERFACE:${Protobuf_LIBRARY}>)
	target_include_directories( blickfeld-scanner PUBLIC $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIRS}>)
else()
	target_link_libraries(blickfeld-scanner PRIVATE ${Protobuf_LIBRARY})
	target_include_directories(blickfeld-scanner PRIVATE ${Protobuf_INCLUDE_DIRS})
endif()
target_link_libraries(blickfeld-scanner-static PUBLIC ${Protobuf_LIBRARY})
target_include_directories(blickfeld-scanner-static PUBLIC ${Protobuf_INCLUDE_DIRS})

if(HAVE_OPENSSL)
  target_link_libraries(blickfeld-scanner PUBLIC OpenSSL::SSL OpenSSL::Crypto)
  target_link_libraries(blickfeld-scanner-static PUBLIC OpenSSL::SSL OpenSSL::Crypto)
endif()

# Add thread support for asio
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(blickfeld-scanner INTERFACE ${CMAKE_THREAD_LIBS_INIT})

# Set version to lib
set_target_properties(blickfeld-scanner
    PROPERTIES VERSION ${BSL_VERSION_MAJOR}.${BSL_VERSION_MINOR})

target_include_directories(blickfeld-scanner INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# Install BSL
install(TARGETS blickfeld-scanner EXPORT blickfeld-scanner
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Main headers
install(
  DIRECTORY ${LIBINCLUDEDIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blickfeld
  FILES_MATCHING PATTERN "*.h"
)
# Install license
install(
  FILES LICENSE.md
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/blickfeld
)
# Generated headers
install(
  DIRECTORY ${PROJECT_BINARY_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
)
# Protocol files
install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/protocol/blickfeld/
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/blickfeld-scanner/protocol/blickfeld
  FILES_MATCHING PATTERN "*.proto"
)

# Misc
if(BF_BUILD_TESTS)
  add_subdirectory(tests)
endif()
if(BF_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
if(BF_BUILD_PYTHON)
  add_subdirectory(python)
endif()

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
	set(Protobuf_PROTOC_EXECUTABLE ${Protobuf_PROTOC_EXECUTABLE} PARENT_SCOPE)
	set(Protobuf_LIBRARY ${Protobuf_LIBRARY} PARENT_SCOPE)
	set(BF_BSL_PYTHON_VERSION ${BF_BSL_PYTHON_VERSION} PARENT_SCOPE)
	
	set(blickfeld-scanner_INCLUDE_DIRS ${BLICKFELD_SCANNER_INCLUDES} PARENT_SCOPE)
	set(blickfeld-scanner_PROTOCOL "${PROJECT_SOURCE_DIR}/protocol" PARENT_SCOPE)
endif()

# CMake Package
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/blickfeld-scanner/blickfeld-scanner-config-version.cmake"
  VERSION ${blickfeld-scanner-lib_VERSION}
  COMPATIBILITY AnyNewerVersion
)
export(EXPORT blickfeld-scanner
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/blickfeld-scanner/blickfeld-scanner.cmake"
)
configure_file(cmake/blickfeld-scanner-config.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/blickfeld-scanner/blickfeld-scanner-config.cmake"
  @ONLY
)
file(COPY "${PROJECT_SOURCE_DIR}/protocol/blickfeld" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cmake/blickfeld-scanner/protocol")

set(ConfigPackageLocation lib/cmake/blickfeld-scanner)
install(EXPORT blickfeld-scanner
  FILE
    blickfeld-scanner.cmake
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/blickfeld-scanner/blickfeld-scanner-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/blickfeld-scanner/blickfeld-scanner-config-version.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

# Debian package
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VENDOR "Blickfeld GmbH")
set(CPACK_PACKAGE_DESCRIPTION "Debian package to communicate with LiDAR devices of the Blickfeld GmbH.")
set(CPACK_PACKAGE_VERSION "${PROJECT_GIT_DESCRIBE}")

if(BF_USE_SYSTEM_PROTOBUF)
	set(CPACK_PACKAGE_NAME "${PROJECT_NAME}-dev")
	if(CPACK_DEBIAN_PACKAGE_DEPENDS)
		set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS},")
	endif()
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}libprotobuf-dev (>= ${Protobuf_VERSION})")
	if(${Protobuf_VERSION} VERSION_GREATER_EQUAL "3.11.4")
		set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS},libprotobuf23")
		set(CPACK_PACKAGE_NAME "${PROJECT_NAME}-dev-testing")
	elseif(${Protobuf_VERSION} VERSION_GREATER_EQUAL "3.6.1")
		set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS},libprotobuf17")
	endif()
else()
	set(CPACK_PACKAGE_NAME "${PROJECT_NAME}-dev-standalone")
endif()

if(HAVE_OPENSSL)
	if(CPACK_DEBIAN_PACKAGE_DEPENDS)
		set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS},")
	endif()
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}libssl-dev (>= ${OPENSSL_VERSION})")
endif()
message("Debian package dependencies: ${CPACK_DEBIAN_PACKAGE_DEPENDS}")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Blickfeld GmbH")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}")
include(CPack)
