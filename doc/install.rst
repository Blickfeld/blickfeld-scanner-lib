.. _BSL_Installation:

============
Installation
============

This guide describes the available pre-compiled library packages and instructions for manual builds from source.
The BSL can be compiled cross-platform and is tested for Linux GCC compilers and Windows MSVC 201x compilers.

.. _BSL Installation Dependencies:

Dependencies
============

The BSL uses `Protocol Buffers <https://developers.google.com/protocol-buffers>`_ as encoding protocol for the network communication, configuration requests, and file recordings.
The protocol allows cross-platform, versioned by design, structured, and optimized data transfers.

It is recommended to install a Protobuf Version 3.6.1+ on the build system, as the BSL provides configuration and stream methods directly as Protobuf messages.
If the Protobuf dependency conflicts with the existing development environment, the standalone mode (use the CMake flag `-DBF_USE_SYSTEM_PROTOBUF=OFF`) of the BSL can be enabled.
It offers the core functionality, like streaming simple point clouds, but lacks configuration methods and metadata information.

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
    sudo apt install -y wget libprotobuf-dev libprotobuf22
    wget https://github.com/Blickfeld/blickfeld-scanner-lib/releases/latest/download/blickfeld-scanner-lib-dev-testing-Linux.deb
    sudo dpkg -i blickfeld-scanner-lib-dev-testing-Linux.deb

Older debian-based systems
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: bash

    sudo apt update
    sudo apt install -y wget
    wget https://github.com/Blickfeld/blickfeld-scanner-lib/releases/latest/download/blickfeld-scanner-lib-dev-standalone-Linux.deb
    sudo dpkg -i blickfeld-scanner-lib-dev-standalone-Linux.deb

Follow the :doc:`getting_started.rst` guide to write your first application.

Python (cross-platform)
-----------------------

The BSL is also available as python package for fast prototyping.
For high performance environments, it is recommended to use the C++ implementation.

.. code-block:: bash

    pip install blickfeld_scanner

Follow the :doc:`getting_started.rst` guide to write your first application.

Windows
-------

No pre-compiled binaries are currently available. Please build it from source.

Build from source
=================

Linux
-----

Dependencies
~~~~~~~~~~~~

CMake is required as build system.

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

Build library
~~~~~~~~~~~~~

.. code-block:: bash

    git clone --recursive https://github.com/Blickfeld/blickfeld-scanner-lib.git
    mkdir blickfeld-scanner-lib/build && cd blickfeld-scanner-lib/build
    cmake ..
    make -j8
    sudo make install

Follow the :ref:`BSL Getting started` guide to write your first application.

Windows
-------

Dependencies
~~~~~~~~~~~~

Building on Windows is tested with MSVC++ and Microsoft nmake.
The minimum requirement is a Microsoft Visual Studio 10 compiler.
VC++ build tool can be found here: https://visualstudio.microsoft.com/de/downloads/.
Install an up-to-date CMake from https://cmake.org/download/.

To activate full support, Protocol Buffers is required. It is recommended to use `CMake External Projects <https://cmake.org/cmake/help/latest/module/ExternalProject.html>`_.
An example will be available soon.

Build library
~~~~~~~~~~~~~

.. code-block:: bash

    git clone --recursive https://github.com/Blickfeld/blickfeld-scanner-lib.git
    mkdir blickfeld-scanner-lib/build && cd blickfeld-scanner-lib/build
    cmake .. -DBF_USE_SYSTEM_PROTOBUF=OFF
    cmake --build . --target ALL_BUILD --config Release
    
.. note:: The `BF_USE_SYSTEM_PROTOBUF=OFF` flag builds the BSL with reduced functionality. Read :ref:`BSL Installation Dependencies` for more information.

Follow the :doc:`getting_started.rst` guide to write your first application.

Compile options
---------------

.. note:: Detailed information about the CMake compile options will be available soon.

