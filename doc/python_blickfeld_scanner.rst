Python Package: blickfeld_scanner
=================================

.. warning:: 
    The Blickfeld conda package "blickfeld_scanner" needs Protobuf to communicate with the device.
    It will automatically install the Protobuf dependency in the environment, but it might not fetch the fast C++ extension.
    Therefore it could be necessary to install Protobuf of Pypi, especially if you want to parse Protobuf messages in real time.

    How to check the version of the Python Protobuf implementation:

    .. code-block:: python

        >>> from google.protobuf.internal import api_implementation
        >>> print(api_implementation._default_implementation_type)
        cpp

    If it prints out "cpp", the correct Python Protobuf implementation is installed.

For a download and installation guide see :ref:`BSL_Installation`.

The blickfeld_scanner python library is used for connecting to a device (:ref:`python_scanner`) and discovering devices in the network (:ref:`python_discover`).

.. toctree::
   :maxdepth: 3
   :caption: Content of the blickfeld_scanner Python library:

   python_scanner
   python_discover
   python_point_cloud_stream
   python_raw_stream
