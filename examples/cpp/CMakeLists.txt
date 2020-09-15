##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
add_subdirectory(async_fetch_clouds)
add_subdirectory(reduced_clouds)
add_subdirectory(fetch_clouds)
add_subdirectory(set_scan_pattern)
add_subdirectory(error_handling)

if (HAVE_OPENSSL)
    add_subdirectory(ssl)
endif()
if (BSL_RECORDING)
	add_subdirectory(read_from_file)
endif()
