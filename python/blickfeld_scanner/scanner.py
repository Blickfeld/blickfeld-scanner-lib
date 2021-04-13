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
import tempfile
import os
import warnings
import time
from distutils.version import LooseVersion, StrictVersion

from .protocol import connection_pb2
from .protocol.stream import connection_pb2 as stream_connection_pb2
from .protocol import options_pb2, common_pb2
from .protocol.config import algorithm_pb2
from .protocol.status import time_synchronization_pb2
from . import stream
from .version import __version__

log = logging.getLogger(__name__)

class scanner(object):
    """
    Scanner is a class to connect to a device, get a point cloud stream, get a status stream and set a scan pattern.

    :param hostname_or_ip: Device to which the connection should be established.
    :type hostname_or_ip: str
    :param port: Port on which the device is reachable the default is 8000.
    :type port: int
    :param name: Name of the device used for string representation.
    :type name: str
    :param key_and_cert_file: Filename containing a private key and certificate for SSL connection.
    :type key_and_cert_file: str
    :param key_and_cert: String containing a private key and certificate for SSL connection.
    :type key_and_cert: str
    """
    protocol_version = 1
    _default_port = 8000
    _default_ssl_port = 8800
    
    def __init__(self, hostname_or_ip="localhost", port=None, name=None, key_and_cert_file=None, key_and_cert=None):
        self.hostname_or_ip = hostname_or_ip
        self.name = name if name is not None else self.hostname_or_ip
        self.api_path = "http://" + hostname_or_ip + "/api/v1/"
        
        self._key_and_cert = None
        if key_and_cert_file:
            with open(key_and_cert_file,'r') as f:
                self._key_and_cert = f.read()
        elif key_and_cert:
            self._key_and_cert = key_and_cert
        self.has_ssl = self._key_and_cert != None

        if port != None:
            self.port = port
        else:
            self.port = self._default_ssl_port if self.has_ssl else self._default_port
        
        self._connection = self.create_connection()
        
        self._hello = self.hello()
        # Compare library version of server and client
        if __version__ != self._hello.library_version:
            def parse_version(version_str):
                return [int(x) for x in version_str.split(".")]
            
            local_version = None
            if self._hello.library_version and __version__:
                local_version = [int(x) if x.isdecimal() else None for x in __version__.split(".")]
                server_version = [int(x) if x.isdecimal() else None for x in self._hello.library_version.split(".")]
            # Only warn if major or minor version does not match
            if local_version is None or len(local_version) < 3 or len(server_version) < 3 or local_version[0] != server_version[0] or local_version[1] != server_version[1]:
                warnings.warn("Warning: The client BSL version does not match the server BSL version. Client has '%s', server has '%s'." % (__version__, self._hello.library_version), DeprecationWarning, stacklevel=2)

    def __del__(self):
        if hasattr(self, "_connection"):
            self._connection.close()
            
    def __repr__(self):
        return "<Blickfeld Scanner '%s'>" % ( self.name )

    def hello(self):
        """ Send hello to device and retrieve device, software, and hardware information

        :returns: Hello object, see :any:`protobuf_protocol` Response.Hello
        """
        req = connection_pb2.Request()
        req.hello.protocol_version = self.protocol_version
        req.hello.library_version = __version__
        req.hello.language = common_pb2.Language.PYTHON
        return self._connection.send_request(req).hello

    def serial_number(self):
        """ > Introduced in BSL v2.16 and firmware v1.17

        Retrieve unique serial number of the device.
        Consider using hello() to retrieve more information.

        :returns: Unique serial number of the device
        """
        hello = self.hello()
        return hello.serial_number if hello.HasField("serial_number") else hello.legacy_serial_number

    def product(self):
        """ > Introduced in BSL v2.16 and firmware v1.17

        Retrieve product type.
        Consider using hello() to retrieve more information.

        :returns: Product type of the device, see :any:`protobuf_protocol` config.Product
        """
        return self.hello().product

    def get_status(self):
        """ Request status of device

        :returns: Status object, see :any:`protobuf_protocol` Status
        """
        req = connection_pb2.Request()
        req.status.SetInParent()
        return self._connection.send_request(req).status

    def get_status_stream(self):
        """ Request status stream of device

        This returns a status stream, which only delivers updates when the device has changed.
        Do not use this in a synchronous blocking call. Use the `get_status` method instead.

        :returns: :py:class:`blickfeld_scanner.scanner.stream.status` object
        """
        return stream.status(self.create_connection())

    def get_point_cloud_stream(self, filter=None, reference_frame=None, point_filter=None, algorithms=None, as_numpy=False):
        """ Request point cloud stream of device

        :param filter: DEPRECATED

            > Introduced in BSL v2.10 and firmware v1.9

            Filter points and returns by point attributes during the post-processing on the device. Is replaced by 'point_filter'.
        :param reference_frame:
            > Introduced in BSL v2.10 and firmware v1.9

            Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
        :param point_filter:
            > Introduced in BSL v2.13 and firmware v1.9

            Filter points and returns by point attributes during the post-processing on the device. This replaces the 'filter' parameter
        :param algorithms:
            > Introduced in BSL v2.17 and firmware v1.16

            Add set of algorithms which post-process the point cloud data.
        :param as_numpy:
            > Introduced in BSL v2.16
    
            This enables numpy support of the point cloud stream. Use recv_frame_as_numpy() to fetch frames as numpy structured arrays.
            Specify the required attributes via the reference_frame attribute.
            
            Note: With devices with an firmware version older than v1.17, the performance is significantly worse than with up-to-date devices as the format needs to be converted on the client side. Consider updating the device.
    
        :returns: :py:class:`blickfeld_scanner.scanner.stream.point_cloud` object
        """
        if filter and point_filter:
            raise Exception("Either provide 'filter' or 'point_filter'.\nPlease use 'point_filter' instead of 'filter'. 'filter' is deprecated.")
        if filter:
            warnings.warn("Please use 'point_filter' instead of 'filter'. 'filter' is deprecated.", DeprecationWarning, stacklevel=2)
            point_filter = filter
        return stream.point_cloud(self.create_connection(), point_filter=point_filter, reference_frame=reference_frame, algorithms=algorithms, as_numpy=as_numpy)

    def get_raw_point_cloud_stream(self, point_filter=None, reference_frame=None):
        """ Request raw point cloud stream of device
        
        > Introduced in BSL v2.13 and firmware v1.13

        :param point_filter:
            > Introduced in BSL v2.10 and firmware v1.9

            Filter points and returns by point attributes during the post-processing on the device.
        :param reference_frame:
            > Introduced in BSL v2.10 and firmware v1.9

            Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
        :returns: :py:class:`blickfeld_scanner.scanner.stream.raw` object
        """
        req = stream_connection_pb2.Subscribe.RawFile()
        req.point_cloud.SetInParent()
        if point_filter:
            req.point_cloud.filter.CopyFrom(point_filter)
        if reference_frame:
            req.point_cloud.reference_frame.CopyFrom(reference_frame)

        return stream.raw(self.create_connection(), req)

    def record_point_cloud_stream(self, file_name, point_filter=None, reference_frame=None):
        """ Record point cloud stream to file
        
        > Introduced in BSL v2.13 and firmware v1.13
        
        :param file_name: Path to the file where it should be dumped

        :param point_filter:
            > Introduced in BSL v2.10 and firmware v1.9

            Filter points and returns by point attributes during the post-processing on the device.
        :param reference_frame:
            > Introduced in BSL v2.10 and firmware v1.9

            Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
        :returns: :py:class:`blickfeld_scanner.scanner.stream.raw` object
        """
        req = stream_connection_pb2.Subscribe.RawFile()
        req.point_cloud.SetInParent()
        if point_filter:
            req.point_cloud.filter.CopyFrom(point_filter)
        if reference_frame:
            req.point_cloud.reference_frame.CopyFrom(reference_frame)

        return stream.raw(self.create_connection(), req, file_name)
        
    @staticmethod
    def file_point_cloud_stream(dump_filename, as_numpy=False):
        """ Request a point_cloud_stream, which streams off a .bfpc file.
        No device (and connection to a device) is needed for this operation.

        :param dump_filename: path to .bfpc file
        :param as_numpy:
            > Introduced in BSL v2.16

            This enables numpy support of the point cloud stream. Use recv_frame_as_numpy() to fetch frames as numpy structured arrays.
            Specify the required attributes via the reference_frame attribute.

            Note: The performance of this option is significantly better with files that were recorded in the packed format. Packed recordings are available in firmware versions >= v1.17.
        :return: :py:class:`blickfeld_scanner.scanner.stream.point_cloud` object
        """
        return stream.point_cloud(from_file=dump_filename, as_numpy=as_numpy)

    def set_default_point_cloud_algorithms(self, persist=False, background_subtraction=False, neighbor_filter=False, static_transformation=False):
        """ Sets point cloud algorithms, which are then applied on all point cloud stream subscribe requests of clients.
        Clients can still actively request other algorithms or disabled them.

        Args:
            persist (bool, optional): Persist the configuration on the device and activates it after a power-cycle. Defaults to False.
            background_subtraction (bool, optional): Either enable the background subtraction with default parameters or supply a configuration object. Defaults to False.
            neighbor_filter (bool, optional): Either enable the neighbor filter with default parameters or supply a configuration object Defaults to False.
            static_transformation (bool, optional): Either enable the static transformation with default parameters or supply a configuration object. Defaults to False.

        Returns:
            Currently set advanced config, see :any:`protobuf_protocol` advanced config
        """
        advanced = self.get_advanced_config()
        del advanced.server.default_point_cloud_subscription.algorithms[:]
        
        algorithms = []
        if background_subtraction:
            cfg = algorithm_pb2.Algorithm()
            if type(background_subtraction) == bool:
                cfg.background_subtraction.SetInParent()
            else:
                cfg.background_subtraction.CopyFrom(background_subtraction)
            algorithms.append(cfg)
        if neighbor_filter:
            cfg = algorithm_pb2.Algorithm()
            if type(neighbor_filter) == bool:
                cfg.neighbor_filter.SetInParent()
            else:
                cfg.neighbor_filter.CopyFrom(neighbor_filter)
            algorithms.append(cfg)
        if static_transformation:
            cfg = algorithm_pb2.Algorithm()
            if type(static_transformation) == bool:
                cfg.static_transformation.SetInParent()
            else:
                cfg.static_transformation.CopyFrom(static_transformation)
            algorithms.append(cfg)
        
        advanced.server.default_point_cloud_subscription.algorithms.extend(algorithms)
        return self.set_advanced_config(advanced, persist=persist)
    
    def set_scan_pattern(self, config=None, name=None, persist = False):
        """ Function to set a new scan pattern, see: :any:`protobuf_protocol`.
        First call :py:func:`blickfeld_scanner.scanner.scanner.fill_scan_pattern` with the scan pattern you want to set and
        then use that returned scan pattern as a config in this function.

        > Changed in BSL v2.15 and firmware v1.16

        It is now possible to set a named scan pattern. Either a scan pattern config or a name of a named scan pattern can be provided.

        :param config: scan pattern to be set
        :param name: name of named scan pattern to be set
        :param persist: Persist scan pattern on device and reload it after a power-cycle, see: :any:`protobuf_protocol` scan pattern
        :return: response scan pattern, see :any:`protobuf_protocol` Connection, see :any:`protobuf_protocol` Connection
        """
        req = connection_pb2.Request()
        if name and config:
            raise ValueError("Provide either config or named scan pattern name.")
        if name:
            req.set_scan_pattern.name = name
        else:
            req.set_scan_pattern.config.MergeFrom(config)
        req.set_scan_pattern.persist = persist
        return self._connection.send_request(req).set_scan_pattern
    
    def fill_scan_pattern(self, config):
        """ Function to fill a scan pattern, see: :any:`protobuf_protocol`. The device fill the missing fields with default values and return it. After it is filled the scan pattern can be set with the :py:func:`blickfeld_scanner.scanner.scanner.set_scan_pattern` function.

        :param config: scan pattern to be filled by the BSS, see: :any:`protobuf_protocol` scan pattern
        :return: Filled scan pattern, which can be set, see :any:`protobuf_protocol` scan pattern
        """
        req = connection_pb2.Request()
        req.fill_scan_pattern.config.MergeFrom(config)
        return self._connection.send_request(req).fill_scan_pattern.config
    
    def get_scan_pattern(self):
        """ Returns the currently set scan pattern, see: :any:`protobuf_protocol`.

        :return: Currently set scan pattern, see :any:`protobuf_protocol` scan pattern
        """
        req = connection_pb2.Request()
        req.get_scan_pattern.SetInParent()
        return self._connection.send_request(req).get_scan_pattern.config
    
    def get_scan_pattern_constraints(self):
        """This request returns a list of constraints which are applied on scan patterns.
        
        > Introduced in BSL v2.14 and firmware v1.14
        
        The constraints define the constant and dynamic relationships between field values.
        The constraints are equal for a device type and firmware, but might vary for firmware releases and device variants.
        It is mainly used to visualize the constraints in the scan pattern configuration of the web gui.

        :return: List of active constraints, see :any:`protobuf_protocol` common
        """
        req = connection_pb2.Request()
        req.get_scan_pattern_constraints.SetInParent()
        return self._connection.send_request(req).get_scan_pattern_constraints.constraints
    
    def set_advanced_config(self, config, persist = False):
        """ Function to set advanced config, see: :any:`protobuf_protocol`.
        Expert parameters: It is not recommended to adapt this calibrated configuration without understanding the influences on the resulting point cloud quality.

        > Introduced in BSL v2.11 and firmware v1.11

        :param config: advanced config to be set
        :param persist: Persist advanced config on device and reload it after a power-cycle
        :return: response advanced config, see :any:`protobuf_protocol` advanced config
        """
        req = connection_pb2.Request()
        req.set_advanced_config.config.MergeFrom(config)
        req.set_advanced_config.persist = persist
        return self._connection.send_request(req).set_advanced_config
    
    def get_advanced_config(self):
        """ Returns the currently set advanced config, see: :any:`protobuf_protocol`.
        
        > Introduced in BSL v2.11 and firmware v1.11

        :return: Currently set advanced config, see :any:`protobuf_protocol` advanced config
        """
        req = connection_pb2.Request()
        req.get_advanced_config.SetInParent()
        return self._connection.send_request(req).get_advanced_config.config
    
    def get_device_timestamp(self):
        """Returns the current device timestamp in seconds.

        :return: Current device timestamp in seconds
        """
        req = connection_pb2.Request()
        req.hello.protocol_version = self.protocol_version
        return self._connection.send_request(req).timestamp_ns / 1e9

    def set_time_synchronization(self, ntp=False, ptp=False, persist=True, wait_for_sync=True, max_sync_duration=60):
        """> Introduced in BSL v2.18 and firmware v1.19
        
        Set the type of time synchronization and set basic parameters

        Either NTP or PTP has to be set (If both are set NTP will be choosen). It will overwrite the current time synchronization configuration

        :param ntp: Set this field to choose NTPv4 time synchronization, this can be a list of ntp servers or a bool if the standard config should be used
        :param ptp: Set this field to choose PTP time synchronization, this can be a list of ptp unicast destinations, this will also activate unicast mode and deactivate multicast mode or a bool if the standard config should be used
        :param persist: Persist time synchronization config on device and reload it after a power-cycle
        :param wait_for_sync: Wait until device is synchronized. Raises exception if device is not synchronized within max_sync_duration.
        :param max_sync_duration: Specify maximum time in seconds for synchronization.
        :return: None.
        """
        cfg = self.get_advanced_config()

        if ntp and ptp:
            raise AttributeError("Either provide ntp or ptp configuration. Not both.")
        if not ntp and not ptp:
            raise AttributeError("Neither ntp nor ptp is provided.")

        if ntp:
            if type(ntp) == bool:
                cfg.time_synchronization.ntp.SetInParent()
            elif type(ntp) == list:
                cfg.time_synchronization.ntp.servers[:] = ntp
            else:
                cfg.time_synchronization.ntp.CopyFrom(ntp)
        if ptp:
            if type(ptp) == bool:
                cfg.time_synchronization.ptp.SetInParent()
            elif type(ptp) == list:
                cfg.time_synchronization.ptp.unicast_destinations[:] = ptp
            else:
                cfg.time_synchronization.ptp.CopyFrom(ptp)

        ret = self.set_advanced_config(cfg, persist=persist)

        if wait_for_sync:
            while max_sync_duration > 0:
                status = self.get_status()
                if status.time_synchronization.state == time_synchronization_pb2.TimeSynchronization.State.SYNCED:
                    return
                time.sleep(1)
                max_sync_duration = max_sync_duration - 1
        
            raise Exception("Device failed to synchronize. Current state is %s." % (time_synchronization_pb2.TimeSynchronization.State.Name(status.time_synchronization.state)))
            
    def run_self_test(self):
        """> Introduced in BSL v2.10 and firmware v1.9
        
        Run self test on the device.
        See :any:`protobuf_protocol` Request.RunSelfTest.

        :return: Self test run response, see :any:`protobuf_protocol` Response.RunSelfTest
        """
        req = connection_pb2.Request()
        req.run_self_test.SetInParent()
        return self._connection.send_request(req).run_self_test
        
    def create_connection(self):
        """Function to create a new connection
        
        :return: Newly created :py:class:`blickfeld_scanner.scanner.connection`
        """
        return connection(self.hostname_or_ip, self.port, self._key_and_cert)
            
    def attempt_error_recovery(self):
        """> Introduced in BSL v2.13 and firmware v1.13
        
        This request can be used to attempt a re-initialization of the device if it is errored.
        A self test is automatically triggered after a successful re-initialization.
        """
        req = connection_pb2.Request()
        req.attempt_error_recovery.SetInParent()
        return self._connection.send_request(req).attempt_error_recovery
    
    def get_named_scan_patterns(self):
        """> Introduced in BSL v2.15 and firmware v1.16
        
        Get protobuf list of named scan patterns. There are two types of named scan patterns:
        1. Default scan patterns, which are not changeable.
        2. User defined named scan patterns, which are changeable.

        :return: List of named scan patterns, see :any:`protobuf_protocol` Response.GetNamedScanPatterns
        """
        req = connection_pb2.Request()
        req.get_named_scan_patterns.SetInParent()
        return self._connection.send_request(req).get_named_scan_patterns
    
    def store_named_scan_pattern(self, name, config):
        """> Introduced in BSL v2.15 and firmware v1.16
        
        Store a named scan patterns.
        The default scan patterns can't be overwritten.

        :param name: Name of the scan pattern
        :param config: Config of the scan pattern, see :any:`protobuf_protocol` ScanPattern
        """
        req = connection_pb2.Request()
        req.store_named_scan_pattern.name = name
        req.store_named_scan_pattern.config.CopyFrom(config)
        return self._connection.send_request(req).store_named_scan_pattern
    
    def delete_named_scan_pattern(self, name):
        """> Introduced in BSL v2.15 and firmware v1.16

        Delete a named scan patterns.
        The default scan patterns can't be deleted.

        :param name: Name of the scan pattern
        """
        req = connection_pb2.Request()
        req.delete_named_scan_pattern.name = name
        return self._connection.send_request(req).delete_named_scan_pattern

    def get_imu_stream(self, as_numpy=False):
        """> Introduced in BSL v2.18 and firmware v1.19
        
        Request IMU stream of device

        :returns: :py:class:`blickfeld_scanner.scanner.stream.imu` object
        """
        return stream.imu(self.create_connection(), as_numpy=as_numpy)
 
    @staticmethod
    def sync(devices, scan_pattern = None, target_frame_rate = None, max_time_difference = 0.1):
        """ Sync two or more devices to record frames at the same time.

        :param server: list of scanners to sync
        :type server: list of :py:class:`blickfeld_scanner.scanner.scanner`
        :param scan_pattern: scan pattern that the devices should use, if None is given, it will select the current scan pattern
        :type scan_pattern: scan pattern, see: :any:`protobuf_protocol`,
        :param target_frame_rate: Frame rate that the devices should run. Maximum frame rate is dependent on the scan pattern, if None is given the maximum possible frame rate will be used.
        :type target_frame_rate: float
        :param max_time_difference: Maximum time difference allowed.
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

    :param value: Exception that occured
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

        :return: Error number
        """
        return self.__field().number
    
    def name(self):
        """ Returns error name as in :any:`protobuf_protocol`

        :return: Error name
        """
        return self.__field().message_type.full_name
    
    def description(self):
        """ Returns error description as in :any:`protobuf_protocol`

        :return: Error description
        """
        return self.__field().message_type.GetOptions().Extensions[options_pb2.e_desc]
        
    def __str__(self):
        return self.__format()
   
class connection(object):
    """ Class to create a connection to the Blickfeld LiDAR device

    :param hostname_or_ip: hostname or IP to which the connection should be established
    :type hostname_or_ip: str
    :param port: port on which the connection should be established
    :type port: int
    :param key_and_cert: String containing a private key and certificate for SSL connection
    :type key_and_cert: str
    :param ssl_protocol: The protocol used for an SSL connection
    :type ssl_protocol: see ssl lib: `ssl.PROTOCOL_...`
    """
    def __init__(self, hostname_or_ip, port, key_and_cert=None, ssl_protocol=ssl.PROTOCOL_TLS):
        self._key_and_cert = key_and_cert
        self.has_ssl = self._key_and_cert != None
        if self.has_ssl:
            self._ssl_context = ssl.SSLContext(ssl_protocol)
            
            # Write cert in temporary file
            fd, key_and_cert_file = tempfile.mkstemp()
            with open(fd, 'w') as f:
                f.write(self._key_and_cert)
            try:
                self._ssl_context.load_cert_chain(key_and_cert_file)
                self._ssl_context.load_verify_locations(key_and_cert_file)
            except:
                os.unlink(key_and_cert_file)
                raise
            os.unlink(key_and_cert_file)
            
            self._ssl_context.verify_mode = ssl.CERT_NONE
            self.socket = self._ssl_context.wrap_socket(socket.create_connection((hostname_or_ip, port)), server_hostname=hostname_or_ip)
        else:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((hostname_or_ip, port))
         
    def __del__(self):
        self.close()

    def clone(self, other_hostname_or_ip=None, other_port=None, other_key_and_cert=None):
        """ Copies connection object.
        
        :param other_hostname_or_ip: a different hostname or IP to use for the clone
        :type other_hostname_or_ip: str
        :param other port: a different port to use for the clone
        :type other port: int
        :param other_key_and_cert: a different string containing a private key and certificate for the clone
        :type other_key_and_cert: str
        """
        (old_hostname_or_ip, old_port) = self.socket.getpeername()

        return connection(other_hostname_or_ip or old_hostname_or_ip, 
                          other_port or old_port,
                          other_key_and_cert or self._key_and_cert)
        
    def send_request(self, req):
        """ Send request to the device

        :param req: Request to the device
        :type req: Request, see: :any:`protobuf_protocol`
        :return: Response of the device, see: :any:`protobuf_protocol`
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

        :return: Response of the device, see: :any:`protobuf_protocol`
        """
        len = struct.unpack('<I', self.__recv_all(4))[0]
        data = self.__recv_all(len)
        recv = connection_pb2.Response()
        recv.ParseFromString(data)

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
