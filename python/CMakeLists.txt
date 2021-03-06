##
## Copyright (c) 2020 Blickfeld GmbH.
## All rights reserved.
##
## This source code is licensed under the BSD-style license found in the
## LICENSE.md file in the root directory of this source tree.
##
# Look for python interpreter, preferably version 3, but using python2 as fallback
find_package(Python COMPONENTS Interpreter)

# Note that PYTHON_VERSION is not the python version but the version of the package blickfeld_scanner
string(REGEX REPLACE "([0-9.]*)-" "\\1\+" BF_BSL_PYTHON_VERSION_TMP "${PROJECT_GIT_DESCRIBE}")
string(REGEX REPLACE "\\+([0-9]*)\\+" ".dev0\+\\1\." BF_BSL_PYTHON_VERSION "${BF_BSL_PYTHON_VERSION_TMP}")

get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(BF_BSL_PYTHON_VERSION ${BF_BSL_PYTHON_VERSION} PARENT_SCOPE)
endif()

if(Python_Interpreter_FOUND)
    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
    set(SETUP_PY "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    
    file(GLOB_RECURSE PYTHON_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
    set(DEPS
      blickfeld-scanner
      ${PYTHON_SOURCE_FILES}
      ${PROTOBUF_GENERATED_PY}
    )
    set(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/dist/blickfeld_scanner.tar.gz")

    file(RELATIVE_PATH CMAKE_CURRENT_SOURCE_DIR_REL "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    configure_file(${SETUP_PY_IN} ${SETUP_PY})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/meta.yaml "${CMAKE_CURRENT_BINARY_DIR}/meta.yaml")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/bld.bat "${CMAKE_CURRENT_BINARY_DIR}/bld.bat")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build.sh "${CMAKE_CURRENT_BINARY_DIR}/build.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../README.md "${CMAKE_CURRENT_BINARY_DIR}/README.md" COPYONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.py.in ${CMAKE_CURRENT_BINARY_DIR}/blickfeld_scanner/version.py)

    add_custom_command( 
	    OUTPUT ${OUTPUT}
	    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/blickfeld_scanner ${CMAKE_CURRENT_BINARY_DIR}/blickfeld_scanner
	    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROTOBUF_GENERATED_PY_DIR}/blickfeld ${CMAKE_CURRENT_BINARY_DIR}/blickfeld_scanner/protocol
	    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/tests ${CMAKE_CURRENT_BINARY_DIR}/tests
	    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/fix_protobuf_imports.py ${CMAKE_CURRENT_BINARY_DIR}/blickfeld_scanner/protocol
	    COMMAND ${Python_EXECUTABLE} ${SETUP_PY} build
	    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/dist
  	  COMMAND ${Python_EXECUTABLE} ${SETUP_PY} sdist
      COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/dist/blickfeld_scanner-${BF_BSL_PYTHON_VERSION}.tar.gz ${OUTPUT}
	    DEPENDS ${DEPS}
    )

    add_custom_target(blickfeld-scanner-python ALL DEPENDS ${OUTPUT})

    install(CODE "execute_process(WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}\" COMMAND ${Python_EXECUTABLE} ${SETUP_PY} install)")
endif()
