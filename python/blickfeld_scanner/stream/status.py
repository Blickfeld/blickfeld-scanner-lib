#!/usr/bin/python
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

from __future__ import print_function
import warnings

from ..protocol import connection_pb2


class status(object):
    """Class to request a status stream

    :param connection: connection to the device
    :type connection: :py:class:`blickfeld_scanner.scanner.connection`
    """
    def __init__(self, connection):
        self._connection = connection

        req = connection_pb2.Request()
        req.subscribe.status.SetInParent()
        ret = self._connection.send_request(req)

    def __del__(self):
        self.stop()

    def close(self):
        """ Close stream and connection

        .. deprecated:: 2.15.0
            Since BSL v2.15.0 this function is deprecated, please use scanner.stream.status.status.stop to stop a stream.
        """
        warnings.warn("This function is deprecated, please use scanner.stream.status.status.stop to stop a stream.", DeprecationWarning, stacklevel=2)
        
        self.stop()

    def recv_status(self):
        """ Receive status update

        :return: Status messages of the device, see: :any:`protobuf_protocol`
        """
        return self._connection.recv().event.status

    def stop(self):
        """Unsubscribe of the status stream and close connection. Always call this function before deleting a stream.status object.
        """
        if self._connection.socket.fileno() == -1:
            return

        req = connection_pb2.Request()
        req.unsubscribe.status.SetInParent()
        self._connection.send(req)

        self._connection.close()
