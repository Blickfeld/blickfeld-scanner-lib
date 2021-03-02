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


class imu(object):
    """Class to request an IMU stream

    :param connection: connection to the device
    :type connection: :py:class:`blickfeld_scanner.scanner.connection`
    """
    def __init__(self, connection, as_numpy=False):
        self._connection = connection

        if as_numpy:
            import numpy
        self.as_numpy = as_numpy

        req = connection_pb2.Request()
        req.subscribe.imu.packed_format = as_numpy
        ret = self._connection.send_request(req)

    def __del__(self):
        self.stop()

    def recv_burst(self):
        """ Receive IMU burst package

        :return: IMU data of the device, see: :any:`protobuf_protocol`
        """
        if self.as_numpy:
            raise AttributeError("The recv_burst() can only be used in combination with the 'as_numpy=False' attribute in the stream constructor.")
        
        return self._connection.recv().event.imu

    def recv_burst_as_numpy(self):
        """ Receive IMU burst package

        :return: IMU data of the device, see: :any:`protobuf_protocol`
        """
        if not self.as_numpy:
            raise AttributeError("The recv_burst_as_numpy() can only be used in combination with the 'as_numpy=True' attribute in the stream constructor.")
        
        import numpy as np

        dtype = []
        dtype.append( ('start_offset_ns', '>u8') )
        dtype.append( ('acceleration', [ ('x', '>f4'), ('y', '>f4'), ('z', '>f4') ] ) )
        dtype.append( ('angular_velocity', [ ('x', '>f4'), ('y', '>f4'), ('z', '>f4') ] ) )

        data = self._connection.recv().event.imu

         # Cast buffers to the correct format
        res = np.zeros(data.packed.length, dtype=dtype)
        res['start_offset_ns'] = np.frombuffer(data.packed.start_offset_ns, dtype='>u8')
        res['acceleration'] = np.frombuffer(data.packed.acceleration, dtype={'names': ['x', 'y', 'z'], 'formats': ['>f4', '>f4', '>f4']})
        res['angular_velocity'] = np.frombuffer(data.packed.angular_velocity, dtype={'names': ['x', 'y', 'z'], 'formats': ['>f4', '>f4', '>f4']})

        data.ClearField('packed')
        
        return data, res

    def stop(self):
        """Unsubscribe of the IMU stream and close connection. Always call this function before deleting a stream.imu object.
        """
        if self._connection.socket.fileno() == -1:
            return

        req = connection_pb2.Request()
        req.unsubscribe.imu.SetInParent()
        self._connection.send(req)

        self._connection.close()

    @staticmethod
    def convert_numpy_burst_to_rotations(data):
        import numpy as np

        acc = data['acceleration']
        res = np.zeros(len(data), dtype=[('pitch', '>f4'), ('roll', '>f4'), ('yaw', '>f4')])
        res['pitch'] = 180 * np.arctan(acc['x'] / np.sqrt(acc['y'] * acc['y'] + acc['z'] * acc['z'])) / np.pi
        res['roll'] = 180 * np.arctan(acc['y'] / np.sqrt(acc['x'] * acc['x'] + acc['z'] * acc['z'])) / np.pi
        res['yaw'] = 180 * np.arctan(acc['z'] / np.sqrt(acc['x'] * acc['x'] + acc['z'] * acc['z'])) / np.pi
        return res
