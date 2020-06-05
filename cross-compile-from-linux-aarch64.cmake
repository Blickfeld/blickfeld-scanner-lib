# this is a toolchain file.
# if you want to crosscompile you should call cmake like this:
#   cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to.../this/file

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
set(SDK_BASE "/opt/Xilinx/SDK/2019.1" CACHE PATH "path to your petalinux installation")
set(cross_prefix ${SDK_BASE}/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-)

set(CMAKE_C_COMPILER   ${cross_prefix}gcc)
set(CMAKE_CXX_COMPILER ${cross_prefix}g++)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_COMPILER_IS_GNUCC TRUE)

