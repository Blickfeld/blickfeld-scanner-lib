Python Package: blickfeld_scanner
=================================

.. warning:: 
    The Blickfeld conda package "blickfeld_scanner" needs protobuf to communicate with the device.
    It will automatically install the protobuf dependency into the environment, but it might not fetch the fast c++ extension.
    Therefore it could be necessary to install protobuf of pip, especially if you want to parse protobuf messages in real time.

    How to check the version of the python protobuf implementation:

    .. code-block:: python

        >>> from google.protobuf.internal import api_implementation
        >>> print(api_implementation._default_implementation_type)
        cpp

    If it prints out "cpp", the right python protobuf implementation is installed.

For a download and installation guide see :ref:`BSL_Installation`.

The blickfeld_scanner python library is used for connecting to a device (:ref:`python_scanner`) and discovering devices in the network (:ref:`python_discover`).

.. toctree::
   :maxdepth: 3
   :caption: Content of the blickfeld_scanner python library:

   python_scanner
   python_discover
   python_point_cloud_stream
