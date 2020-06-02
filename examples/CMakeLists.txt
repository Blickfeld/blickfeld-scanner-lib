##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
add_subdirectory(cpp_standalone)
if(NOT BSL_STANDALONE)
	add_subdirectory(cpp)
endif()