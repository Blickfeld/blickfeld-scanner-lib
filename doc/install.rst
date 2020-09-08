.. _BSL_Installation:

============
Installation
============

This guide describes the available pre-compiled library packages and instructions for manual builds from source.
The BSL can be compiled cross-platform and has been tested for Linux GCC compilers and Windows MSVC 201x compilers.

The source code is published on `GitHub <https://github.com/Blickfeld/blickfeld-scanner-lib>`_ .

.. _BSL Installation Dependencies:

Dependencies
============

The BSL uses `Protocol Buffers <https://developers.google.com/protocol-buffers>`_ as an encoding protocol for network communication, configuration requests, and file recordings.
The protocol allows cross-platform, versioned by design, structured, and optimized data transfers.

It is recommended to install a Protobuf version 3.6.1+ on the build system, as the BSL provides configuration and stream methods directly as Protobuf messages.
If the Protobuf dependency conflicts with the existing development environment, the standalone mode (use the CMake flag `-DBF_USE_SYSTEM_PROTOBUF=OFF`) of the BSL can be enabled.
It offers the core functionality, such as streaming simple point clouds, but lacks configuration methods and metadata information.

Pre-compiled packages
=====================

Linux
-----

Debian packages with full Protobuf support are available for Ubuntu 19+ and Debian 10+.
Debian packages with BSL standalone mode are available for older systems.

Ubuntu 19+ and Debian 10
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    sudo apt update
    sudo apt install -y wget libprotobuf-dev libprotobuf17
    wget https://github.com/Blickfeld/blickfeld-scanner-lib/releases/latest/download/blickfeld-scanner-lib-dev-Linux.deb
    sudo dpkg -i blickfeld-scanner-lib-dev-Linux.deb

Latest debian-based systems
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    sudo apt update
    sudo apt install -y wget libprotobuf-dev libprotobuf23
    wget https://github.com/Blickfeld/blickfeld-scanner-lib/releases/latest/download/blickfeld-scanner-lib-dev-testing-Linux.deb
    sudo dpkg -i blickfeld-scanner-lib-dev-testing-Linux.deb

Older debian-based systems
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    sudo apt update
    sudo apt install -y wget
    wget https://github.com/Blickfeld/blickfeld-scanner-lib/releases/latest/download/blickfeld-scanner-lib-dev-standalone-Linux.deb
    sudo dpkg -i blickfeld-scanner-lib-dev-standalone-Linux.deb

Follow the :ref:`BSL_Getting_started` guide to write your first application.

Python (cross-platform)
-----------------------

The BSL is also available as a Python package for fast prototyping.
For high-performance environments it is recommended to use the C++ implementation.

.. code-block:: bash

    pip install blickfeld_scanner

Follow the :ref:`BSL_Getting_started` guide to write your first application.

Windows
-------

No pre-compiled binaries are currently available. Please build it from source.

Building from source
====================

Linux
-----

Dependencies
~~~~~~~~~~~~

CMake is required as a build system.

.. code-block:: bash

    curl https://cmake.org/files/v3.15/cmake-3.15.3-Linux-x86_64.sh -o /tmp/curl-install.sh \
        && chmod u+x /tmp/curl-install.sh \
        && sudo mkdir /usr/bin/cmake \
        && sudo /tmp/curl-install.sh --skip-license --prefix=/usr/bin/cmake \
        && rm /tmp/curl-install.sh

Install Protocol Buffers from source or install available packages.

Packages
********

.. code-block:: bash

    sudo apt update
    sudo apt install -y git build-essential libprotobuf-dev libprotoc-dev protobuf-compiler
    
From source
***********

.. code-block:: bash

    sudo apt update
    sudo apt install -y git build-essential autoconf automake libtool curl unzip
    git clone --recursive https://github.com/protocolbuffers/protobuf.git
    cd protobuf/cmake
    cmake . -Dprotobuf_BUILD_SHARED_LIBS=ON
    make -j8
    sudo make install

Build library with installed dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    git clone --recursive https://github.com/Blickfeld/blickfeld-scanner-lib.git
    mkdir blickfeld-scanner-lib/build && cd blickfeld-scanner-lib/build
    cmake ..
    make -j8
    sudo make install

Follow the :ref:`BSL_Getting_started` guide to write your first application.

Build library without installed dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The BSL can also be build without installing the dependencies, it will fetch the needed dependencies via git and build them, before building the BSL.
An example how to build the BSL with full support can be found in cmake/external.
For this example git is required, to download the sources from protobuf and zlib.

1. Build BSL with all dependencies

   .. code-block:: bash

       git clone --recursive https://github.com/Blickfeld/blickfeld-scanner-lib.git
       mkdir blickfeld-scanner-lib/build && cd blickfeld-scanner-lib/build
       cmake ../cmake/external
       make

   After building the BSL, all dependencies are in the build/install folder (Protobuf, zlib and the BSL).
   For building the example and linking own projects, the absolute path to the install folder is required, please note it down somewhere.

   For example this could be the following path: `/mnt/e/develop/blickfeld-scanner-lib/build/install`.

2. Build example and link it to the previously build BSL. 

   As an example, the reduced_clouds example is build. The approach is working analogously for the other examples or custom applications.

   .. code-block:: bash

       mkdir build_example && cd build_example
       cmake -DCMAKE_PREFIX_PATH=<path_to_bsl_install_dir> ../examples/cpp/reduced_clouds/
       make

   .. note:: The <path_to_bsl_install_dir> is the absolute path to the build/install folder of the BSL build, which we noted down in the step above.

3. Start the example

   .. code-block:: bash

       ./bf-reduced_clouds <hostname>

   Use the device's hostname or ip adress as `<hostname>`.

Follow the :ref:`BSL_Getting_started` guide to write your first application.

Windows
-------

Dependencies
~~~~~~~~~~~~

Building on Windows has been tested with MSVC++ and Microsoft nmake.
The minimum requirement is a Microsoft Visual Studio 10 compiler.
The VC++ build tool can be found here: https://visualstudio.microsoft.com/de/downloads/.
Install an up-to-date CMake from: https://cmake.org/download/.

To activate full support, Protocol Buffers are required. It is recommended to use `CMake External Projects <https://cmake.org/cmake/help/latest/module/ExternalProject.html>`_.

Build library
~~~~~~~~~~~~~

An example how to build the BSL with full support can be found in cmake/external.
For this example git is required, to download the sources from protobuf and zlib

1. Build BSL with all dependencies

   .. code-block:: bash

	   git clone --recursive https://github.com/Blickfeld/blickfeld-scanner-lib.git
	   mkdir blickfeld-scanner-lib/build && cd blickfeld-scanner-lib/build
	   cmake ../cmake/external -DCMAKE_BUILD_TYPE=Release
	   cmake --build . --target ALL_BUILD

   After building the BSL, all dependencies are in the build/install folder (Protobuf, zlib and the BSL).
   For building the example and linking own projects, the absolute path to the install folder is required, please note it down somewhere.

   For example this could be the following path: `C:\\develop\\blickfeld-scanner-lib\\build\\install`.

2. Build example and link it to the previously build BSL. 

   As an example, the reduced_clouds example is build. The approach is working analogously for the other examples or custom applications.

   .. code-block:: bash

	   mkdir build_example && cd build_example
	   cmake -DCMAKE_PREFIX_PATH=<path_to_bsl_install_dir> -DCMAKE_BUILD_TYPE=Release ../examples/cpp/reduced_clouds/
	   cmake --build .

   .. note:: The <path_to_bsl_install_dir> is the absolute path to the build/install folder of the BSL build, which we noted down in the step above.

3. Start the example

   .. code-block:: bash

	   ./Release/bf-reduced_clouds.exe <hostname>

   Use the device's hostname or ip adress as `<hostname>`.


Follow the :ref:`BSL_Getting_started` guide to write your first application.

Build standalone library
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    git clone --recursive https://github.com/Blickfeld/blickfeld-scanner-lib.git
    mkdir blickfeld-scanner-lib/build && cd blickfeld-scanner-lib/build
    cmake .. -DBF_USE_SYSTEM_PROTOBUF=OFF -DCMAKE_BUILD_TYPE=Release
    cmake --build . --target ALL_BUILD
    
.. note:: The `BF_USE_SYSTEM_PROTOBUF=OFF` flag builds the BSL with reduced functionality. Read :ref:`BSL Installation Dependencies` for more information.

Follow the :ref:`BSL_Getting_started` guide to write your first application.

Compile options
---------------

.. note:: Detailed information about the CMake compile options will be available soon.
