# see https://cmake.org/cmake/help/git-master/manual/cmake-packages.7.html#creating-packages
include(CMakeFindDependencyMacro)

set(BSL_STANDALONE @BSL_STANDALONE@)
set(HAVE_OPENSSL @HAVE_OPENSSL@)

if (NOT BSL_STANDALONE)
 	find_dependency(Protobuf @Protobuf_VERSION@ REQUIRED)
  	if(NOT PROTOBUF_FOUND)
    	set(blickfeld-scanner_FOUND False)
    	set(blickfeld-scanner_NOT_FOUND_MESSAGE "BSL is configured with Protobuf support. Could not find suitable Protobuf on this system (libprotobuf-dev).")
  	endif()
endif()

if (HAVE_OPENSSL)
 	find_dependency(OpenSSL REQUIRED)
  	if(NOT OPENSSL_FOUND)
    	set(blickfeld-scanner_FOUND False)
    	set(blickfeld-scanner_NOT_FOUND_MESSAGE "BSL is configured with OpenSSL support. Could not find suitable OpenSSL on this system (openssl).")
  	endif()
endif()

foreach(_comp ${blickfeld-scanner_FIND_COMPONENTS})
  if("full" STREQUAL "${_comp}")
    if(BSL_STANDALONE)
  	  set(blickfeld-scanner_FOUND False)
	  set(blickfeld-scanner_NOT_FOUND_MESSAGE "Installed BSL is configured in standalone mode. Full mode with system Protobuf is required.")
	endif()
  elseif("ssl" STREQUAL "${_comp}")
    if(NOT HAVE_OPENSSL)
  	  set(blickfeld-scanner_FOUND False)
	  set(blickfeld-scanner_NOT_FOUND_MESSAGE "Installed BSL is configured without SSL support. Full mode with system OpenSSL is required.")
	endif()
  else()
    set(blickfeld-scanner_FOUND False)
    set(blickfeld-scanner_NOT_FOUND_MESSAGE "Unsupported component: ${_comp}")  
  endif()
endforeach()

include("${CMAKE_CURRENT_LIST_DIR}/blickfeld-scanner.cmake")
set_property(TARGET blickfeld-scanner 
        APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${PROTOBUF_INCLUDE_DIR}"
        APPEND PROPERTY INTERFACE_LINK_LIBRARIES "${Protobuf_LIBRARIES}"
)
set(blickfeld-scanner_PROTOCOL "${CMAKE_CURRENT_LIST_DIR}/protocol")
