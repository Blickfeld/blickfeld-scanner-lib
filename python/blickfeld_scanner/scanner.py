#!/usr/bin/python
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

from __future__ import print_function

import logging
import re
import socket
import struct
import ssl

from .protocol import connection_pb2
from .protocol import options_pb2
from . import stream


log = logging.getLogger(__name__)

class scanner(object):
    """
    Class to connect to a device, get a point cloud stream, get a status stream and set a scan pattern.

    :param hostname_or_ip: Device, to which the connection should be established.
    :type hostname_or_ip: str
    :param port: Port on which the device is reachable, default is 8000.
    :type port: int
    :param name: Name of the device. Used for string representation.
    :type name: str
    :param key_and_cert_file: Filename containing a private key and certificate for SSL connection
    :type key_and_cert_file: str
    """
    protocol_version = 1
    _default_port = 8000
    _default_ssl_port = 8800
    
    def __init__(self, hostname_or_ip="localhost", port=None, name=None, key_and_cert_file=None):
        self.hostname_or_ip = hostname_or_ip
        self.name = name if name is not None else self.hostname_or_ip
        self.api_path = "http://" + hostname_or_ip + "/api/v1/"
        self._key_and_cert_file = key_and_cert_file
        self.has_ssl = self._key_and_cert_file != None

        if port != None:
            self.port = port
        else:
            self.port = self._default_ssl_port if self.has_ssl else self._default_port
        
        self._connection = self.create_connection()
        
        req = connection_pb2.Request()
        req.hello.protocol_version = self.protocol_version
        self._connection.send_request(req)

    def __del__(self):
        if hasattr(self, "_connection"):
            self._connection.close()
            
    def __repr__(self):
        return "<Blickfeld Scanner '%s'>" % ( self.name )

    def get_status(self):
        """ Request status of device.

        :returns: Status object
        :rtype: Status, see :any:`protobuf_protocol` Status
        """
        req = connection_pb2.Request()
        req.status.SetInParent()
        return self._connection.send_request(req).status

    def get_status_stream(self):
        """ Request status stream of device.

        This returns a status stream, which only delivers updates when the device has changed.
        Do not use this in a synchronous blocking call. Use the `get_status` method instead.

        :returns: Status stream object
        :rtype: :py:class:`blickfeld_scanner.scanner.status_stream`
        """
        return status_stream(self.create_connection())

    def get_point_cloud_stream(self):
        """ Request point cloud stream of device.

        :returns: Point cloud stream object
        :rtype: :py:class:`blickfeld_scanner.scanner.point_cloud_stream`
        """
        return stream.point_cloud(self.create_connection())

    @staticmethod
    def file_point_cloud_stream(dump_filename):
        """ Request a point_cloud_stream, which streams of a .bfpc file.
        No device (and connection to a device) is needed for this operation.

        :param dump_filename: path to .bfpc file
        :return: :py:class:`blickfeld_scanner.scanner.point_cloud_stream`
        """
        return stream.point_cloud(from_file=dump_filename)
    
    def set_scan_pattern(self, config, persist = False):
        """ Function to set a new ScanPattern, see: :any:`protobuf_protocol`.
        First call :py:func:`blickfeld_scanner.scanner.scanner.fill_scan_pattern` with the ScanPattern you want to set and
        then use that returned ScanPattern as a config in this function.

        :param config: ScanPattern to be set
        :param persist: Persist scan pattern on device and reload it after a power-cycle
        :type config: ScanPattern, see: :any:`protobuf_protocol` ScanPattern
        :return: response ScanPattern, see :any:`protobuf_protocol` Connection
        :rtype: response ScanPattern, see :any:`protobuf_protocol` Connection
        """
        req = connection_pb2.Request()
        req.set_scan_pattern.config.MergeFrom(config)
        req.set_scan_pattern.persist = persist
        return self._connection.send_request(req).set_scan_pattern
    
    def fill_scan_pattern(self, config):
        """ Function to fill a ScanPattern, see: :any:`protobuf_protocol`. The device fill the missing fields with default values and return it. After it is filled the ScanPattern can be set with the :py:func:`blickfeld_scanner.scanner.scanner.set_scan_pattern` function.

        :param config: ScanPattern to be filled by the BSS
        :type config: ScanPattern, see: :any:`protobuf_protocol` ScanPattern
        :return: Filled ScanPattern, which can be set
        :rtype: ScanPattern, see :any:`protobuf_protocol` ScanPattern
        """
        req = connection_pb2.Request()
        req.fill_scan_pattern.config.MergeFrom(config)
        return self._connection.send_request(req).fill_scan_pattern.config
    
    def get_scan_pattern(self):
        """ Returns the current set ScanPattern, see: :any:`protobuf_protocol`.

        :return: current set ScanPattern
        :rtype: ScanPattern, see :any:`protobuf_protocol` ScanPattern
        """
        req = connection_pb2.Request()
        req.get_scan_pattern.SetInParent()
        return self._connection.send_request(req).get_scan_pattern.config
    
    def get_device_timestamp(self):
        """Returns the current device timestamp in seconds.

        :return: current device timestamp in seconds
        :rtype: float
        """
        req = connection_pb2.Request()
        req.hello.protocol_version = self.protocol_version
        return self._connection.send_request(req).timestamp_ns / 1e9
    
    def get_ntp_server(self):
        """Attention to use this function you need the requests library
        `https://requests.readthedocs.io/en/master/`

        Function to get a ntp server

        :return: returns the the ntp server ip adress
        :rtype: str
        """
        import requests
        req = requests.get(self.api_path + "network/ntp")
        req.raise_for_status()
        return req.json()['data']['server']
    
    def set_ntp_server(self, server):
        """Attention to use this function you need the requests library
        `https://requests.readthedocs.io/en/master/`

        Function to set a ntp server

        :param server: Server IP to set the ntp server to
        :type server: str
        """
        import requests
        import json
        cur_server = self.get_ntp_server()
        if cur_server != server:
            req = requests.put(self.api_path + "network/ntp/server", data=json.dumps({ "data": server }), headers={"Content-Type": "application/json"})
            req.raise_for_status()
            log.info("Configured NTP server to '%s'. Please power cycle the device to apply changes." % (self.get_ntp_server()))
        
    def create_connection(self, other_key_and_cert_file=None):
        """Function to create a new connection

        :param other_key_and_cert_file: Filename containing another private key and certificate for this SSL connection
        :type other_key_and_cert_file: str 
        :return: New created connection
        :rtype: :py:class:`blickfeld_scanner.scanner.connection`
        """
        return connection(self.hostname_or_ip, self.port, 
            key_and_cert_file=other_key_and_cert_file or self._key_and_cert_file)
 
    @staticmethod
    def sync(devices, scan_pattern = None, target_frame_rate = None, max_time_difference = 0.1):
        """ Sync two or more devices to record frames at the same time.

        :param server: list of scanners to sync
        :type server: list of :py:class:`blickfeld_scanner.scanner.scanner`
        :param scan_pattern: ScanPattern, which the devices should use, if None is given, it will select current ScanPattern
        :type scan_pattern: ScanPattern, see: :any:`protobuf_protocol`,
        :param target_frame_rate: Frame rate which the devices should run. Max frame rate is dependend on the ScanPattern. If None is given the max possible frame rate will be used.
        :type target_frame_rate: float
        :param max_time_difference: Max time difference which is allowed.
        :type max_time_difference: float
        """
        max_frame_rate = None
        for device in devices:
            sp_device = device.fill_scan_pattern(scan_pattern) if scan_pattern is not None else device.get_scan_pattern()
            if max_frame_rate is None or sp_device.frame_rate.maximum < max_frame_rate:
                max_frame_rate = sp_device.frame_rate.maximum
        
        if target_frame_rate and target_frame_rate < max_frame_rate:
            frame_rate = target_frame_rate
        else:
            frame_rate = max_frame_rate
        
        log.info("Syncing to frame rate %0.2f Hz (maximum %0.2f Hz)" % (frame_rate, max_frame_rate))
        
        for device in devices:
            sp_device = device.fill_scan_pattern(scan_pattern) if scan_pattern is not None else device.get_scan_pattern()
            sp_device.frame_rate.target = frame_rate
            device.set_scan_pattern(sp_device)
        log.info("Sync triggered")
        
        timestamps = []
        total_diff = 0
        for device in devices:
            timestamps.append(device.get_device_timestamp())
        for i, ts in enumerate(timestamps[1:], 1):
            diff = abs(timestamps[0] - ts)
            total_diff = total_diff + diff
            if diff > max_time_difference:
                raise Exception("Device timestamps are not synced (%s: %fs, %s: %fs => diff %fs)." 
                    % (devices[i].hostname_or_ip, ts, devices[0].hostname_or_ip, timestamps[0], diff))
        log.info("Total time difference with serial requests %0.3fs" % (total_diff))

class protocol_exception(Exception):
    """ Protocol exception class for errors, see errors in :any:`protobuf_protocol`

    :param value: Exception which occured
    :type value: Protocol Exception, see: :any:`protobuf_protocol`
    """
    def __init__(self, value):
        self.error = value

    def __format(self):
        e_desc = self.description()
        for match in re.finditer(re.compile(r"(\{([a-z_]+)\:(\%\d*\.*\d*[fsdu])\})"), e_desc):
            template, key, fmt = match.groups()
            if self.__contained_error().HasField(key):
                e_desc = e_desc.replace(template, str(fmt % getattr(self.__contained_error(), key)))
        return self.name() + str(": (errno: {})").format(self.errno()) + "\n\t" + e_desc
        
    def __field(self):
        return self.error.DESCRIPTOR.fields_by_name[str(self.error.WhichOneof("error"))]

    def __contained_error(self):
        return getattr(self.error, self.error.WhichOneof("error"))
        
    def errno(self):
        """ Returns error number

        :return: error number
        :rtype: int
        """
        return self.__field().number
    
    def name(self):
        """ Returns error name as in :any:`protobuf_protocol`

        :return: error name
        :rtype: str
        """
        return self.__field().message_type.full_name
    
    def description(self):
        """ Returns error description as in :any:`protobuf_protocol`

        :return: error description
        :rtype: str
        """
        return self.__field().message_type.GetOptions().Extensions[options_pb2.e_desc]
        
    def __str__(self):
        return self.__format()
   
class connection(object):
    """ Class to create a connection to the Blickfeld LiDAR device

    :param hostname_or_ip: hostname or ip to which the connection should be established
    :type hostname_or_ip: str
    :param port: port on which the connection should be established
    :type port: int
    :param key_and_cert_file: Filename containing a private key and certificate for SSL connection
    :type key_and_cert_file: str
    :param ssl_protocol: The protocol used for an SSL connection
    :type ssl_protocol: see ssl lib: `ssl.PROTOCOL_...`
    """
    def __init__(self, hostname_or_ip, port, key_and_cert_file=None, ssl_protocol=ssl.PROTOCOL_TLS):
        self._key_and_cert_file = key_and_cert_file
        self.has_ssl = self._key_and_cert_file != None
        if self.has_ssl:
            self._ssl_context = ssl.SSLContext(ssl_protocol)
            self._ssl_context.load_cert_chain(key_and_cert_file)
            self._ssl_context.load_verify_locations(key_and_cert_file)
            self._ssl_context.verify_mode = ssl.CERT_REQUIRED
            self.socket = self._ssl_context.wrap_socket(socket.create_connection((hostname_or_ip, port)), server_hostname=hostname_or_ip)
        else:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((hostname_or_ip, port))
         
    def __del__(self):
        self.close()

    def clone(self, other_hostname_or_ip=None, other_port=None, other_key_and_cert_file=None):
        """ Copies connection object.
        
        :param other_hostname_or_ip: a different hostname or ip to use for the clone
        :type other_hostname_or_ip: str
        :param other port: a different port to use for the clone
        :type other port: int
        :param other_key_and_cert_file: a different filename containing a private key and certificate for the clone
        :type other_key_and_cert_file: str
        """
        (old_hostname_or_ip, old_port) = self.socket.getpeername()

        return connection(other_hostname_or_ip or old_hostname_or_ip, 
                          other_port or old_port,
                          other_key_and_cert_file or self._key_and_cert_file)
        
    def send_request(self, req):
        """ Send request to the device

        :param req: Request to the device
        :type req: Request, see: :any:`protobuf_protocol`
        :return: Response of the device
        :rtype: Response, see: :any:`protobuf_protocol`
        """
        self.send(req)
        return self.recv()

    def send(self, req):
        """ Send request to the device

        :param req: Request to the device
        :type req: Request, see: :any:`protobuf_protocol`
        """
        data = req.SerializeToString()

        self.socket.send(struct.pack('<I', len(data)))
        self.socket.send(data)

    def recv(self):
        """ Receive response of the device

        :return: response of the device
        :rtype: Response, see: :any:`protobuf_protocol`
        """
        len = struct.unpack('<I', self.__recv_all(4))[0]
        recv = connection_pb2.Response()
        recv.ParseFromString(self.__recv_all(len))

        if recv.HasField('error'):
            raise protocol_exception(recv.error)

        return recv

    def __recv_all(self, n):
        data = b''
        while len(data) < n:
            packet = self.socket.recv(n - len(data))
            if not packet:
                return None
            data += packet
        return data

    def close(self):
        """ Close connection to the device
        """
        if hasattr(self, "socket"):
            self.socket.close()



class status_stream(object):
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
        self.close()

    def close(self):
        """ Close stream stream and connection
        """
        self._connection.close()

    def recv_status(self):
        """ Receive status update

        :return: status messages of the device
        :rtype: Status, see: :any:`protobuf_protocol`
        """
        return self._connection.recv().event.status
