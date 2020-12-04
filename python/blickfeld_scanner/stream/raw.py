#!/usr/bin/python
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

from __future__ import print_function

from ..protocol import connection_pb2

import signal
import sys

terminate = False
def signal_handler(sig, frame):
    global terminate
    terminate = True

# Catch CTRL-C and terminate and close the raw file stream
signal.signal(signal.SIGINT, signal_handler)

class raw(object):
    """ Class to request a point cloud stream
    
    > Introduced in BSL v2.13 and firmware v1.13

    :param connection: connection to the device
    :type connection: :py:class:`blickfeld_scanner.scanner.connection`
    :param req: request for the raw stream, for example a raw point cloud stream
    :param file_name: Path to the file where it should be dumped
    """

    def __init__(self, connection, req, file_name = None):

        if not connection:
            raise AttributeError("Connection is not provided.")

        if file_name:
            self._ofile = open(file_name, 'wb')
        else:
            self._ofile = None

        self._req = req
        req = connection_pb2.Request()
        req.subscribe.raw_file.CopyFrom(self._req)

        self._connection = connection        
        self._connection.send_request(req)

    def __del__(self):
        self.stop()

    def recv_bytes(self):
        """ Receive raw bytes

        When a file during creating of the raw object is given, this function will dump the bytes to the file.

        Will throw a `ConnectionError` when the end_of_stream field is set in the received event.

        :return: bytes
        """
        
        # If stream is closed with CTRL-C it will still write the footer and save the file
        global terminate
        if terminate:
            terminate = False
            self.stop()
            raise Exception("Detected SIGINT during recv_bytes, stream and file were closed gracefully.")

        resp = self._connection.recv().event

        if resp.HasField("end_of_stream"):
            raise ConnectionError("Stream was closed by the server.")

        # Record frame to file
        if self._ofile:
            self._ofile.write(resp.raw_file)

        return resp.raw_file

    def stop(self):
        """ Stop and unsuscribe of the stream.

        Receive all remaining packets and the footer.
        Will receive packets until the `ConnectionError` is raised, when the server sends the end_of_stream event.

        :return: all remaining bytes accumalated
        """
        if self._connection.socket._closed:
            return b""

        req = connection_pb2.Request()
        req.unsubscribe.raw_file.CopyFrom(self._req)
        self._connection.send(req)

        raw_file = b""

        while True:
            try:
                raw_file += self.recv_bytes()
            except ConnectionError:
                break

        if self._ofile:
            self._ofile.close()
            self._ofile = None

        self._connection.close()

        return raw_file
