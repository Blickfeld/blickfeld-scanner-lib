#!/usr/bin/python
#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#

from __future__ import print_function

import math
import gzip
import time
import warnings
from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint

from ..protocol import connection_pb2
from ..protocol.file import general_pb2, point_cloud_pb2
from ..protocol.data import frame_pb2
from .. import version

import signal
import sys


terminate = False

def signal_handler(sig, frame):
    global terminate
    terminate = True

# Catch CTRL-C and terminate and close the point cloud stream
signal.signal(signal.SIGINT, signal_handler)


frame_pb2.Frame.__repr__ = frame_pb2.Frame.__str__ = lambda \
    self: "<Blickfeld Frame %u: %u returns, %.1fx%.1f FoV, %u scanlines>" % (
    self.id,
    self.total_number_of_returns,
    self.scan_pattern.horizontal.fov * 180 / math.pi,
    self.scan_pattern.vertical.fov * 180 / math.pi,
    len(self.scanlines)
)

""" Reference frame: XYZ coordinates """
REF_FRAME_XYZ = frame_pb2.Frame()
REF_FRAME_XYZ.scan_pattern.SetInParent()
REF_FRAME_XYZ.total_number_of_points = 0
REF_FRAME_XYZ.total_number_of_returns = 0
REF_FRAME_XYZ.scanlines.add().points.add().returns.add().cartesian.append(0)

""" Reference frame: XYZ coordinates, intensity """
REF_FRAME_XYZ_I = frame_pb2.Frame()
REF_FRAME_XYZ_I.CopyFrom(REF_FRAME_XYZ)
REF_FRAME_XYZ_I.scanlines[0].points[0].returns[0].intensity = 0

""" Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id """
REF_FRAME_XYZ_I_ID = frame_pb2.Frame()
REF_FRAME_XYZ_I_ID.CopyFrom(REF_FRAME_XYZ_I)
REF_FRAME_XYZ_I_ID.id = 0
REF_FRAME_XYZ_I_ID.scanlines[0].id = 0
REF_FRAME_XYZ_I_ID.scanlines[0].points[0].id = 0
REF_FRAME_XYZ_I_ID.scanlines[0].points[0].returns[0].id = 0

""" Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id, timestamps """
REF_FRAME_XYZ_I_ID_TS = frame_pb2.Frame()
REF_FRAME_XYZ_I_ID_TS.CopyFrom(REF_FRAME_XYZ_I_ID)
REF_FRAME_XYZ_I_ID_TS.start_time_ns = 0
REF_FRAME_XYZ_I_ID_TS.scanlines[0].start_offset_ns = 0
REF_FRAME_XYZ_I_ID_TS.scanlines[0].points[0].start_offset_ns = 0

""" Reference frame: ambient_light_level, intensity, range, frame id, scanline id, point id """
REF_FRAME_DEPTH_MAP = frame_pb2.Frame()
REF_FRAME_DEPTH_MAP.scan_pattern.SetInParent()
REF_FRAME_DEPTH_MAP.total_number_of_points = 0
REF_FRAME_DEPTH_MAP.total_number_of_returns = 0
REF_FRAME_DEPTH_MAP.id = 0
REF_FRAME_DEPTH_MAP.scanlines.add().points.add().returns.add()
REF_FRAME_DEPTH_MAP.scanlines[0].id = 0
REF_FRAME_DEPTH_MAP.scanlines[0].points[0].id = 0
REF_FRAME_DEPTH_MAP.scanlines[0].points[0].ambient_light_level = 0
REF_FRAME_DEPTH_MAP.scanlines[0].points[0].returns[0].intensity = 0
REF_FRAME_DEPTH_MAP.scanlines[0].points[0].returns[0].range = 0


class point_cloud(object):
    """ Class to request a point cloud stream

    :param connection: connection to the device
    :type connection: :py:class:`blickfeld_scanner.scanner.connection`
    :param from_file: path to file, of which to stream a point cloud, if this is set, no connection should be given
    :param filter: DEPRECATED

        > Introduced in BSL v2.10 and firmware v1.9

        Filter points and returns by point attributes during the post-processing on the device. Is replaced by 'point_filter'.
    :param reference_frame:
        > Introduced in BSL v2.10 and firmware v1.9

        Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
    :param point_filter:
        > Introduced in BSL v2.10 and firmware v1.9

        Filter points and returns by point attributes during the post-processing on the device. This replaces the 'filter' parameter
        
    :param extend_subscribe_request:
        > Introduced in BSL v2.13 and firmware v1.13

        Extend point cloud subscription request with additional parameters. This is mainly used internally.
    """

    REF_FRAME_XYZ = REF_FRAME_XYZ  #: Reference frame: XYZ coordinates
    REF_FRAME_XYZ_I = REF_FRAME_XYZ_I  #: Reference frame: XYZ coordinates, intensity
    REF_FRAME_XYZ_I_ID = REF_FRAME_XYZ_I_ID  #: Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id
    REF_FRAME_XYZ_I_ID_TS = REF_FRAME_XYZ_I_ID_TS  #: Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id, timestamps
    REF_FRAME_DEPTH_MAP = REF_FRAME_DEPTH_MAP  #: Reference frame: ambient_light_level, intensity, range, frame id, scanline id, point id

    def __init__(self, connection=None, from_file=None, filter=None, reference_frame=None, point_filter=None, extend_subscribe_request=None):
        self._metadata = point_cloud_pb2.PointCloud.Metadata()

        if connection and from_file:
            raise AttributeError("Either provide connection or from_file.")
        if not connection and not from_file:
            raise AttributeError("Neither connection nor from_file is provided.")

        self._thread = None
        self._connection = None

        self._last_frame_id = None

        self._ofile = None
        self._stream_data = point_cloud_pb2.PointCloud.Data()
        self._prev_scan_pattern_str = b""

        if connection:
            self._connection = connection

            req = connection_pb2.Request()
            req.subscribe.point_cloud.SetInParent()
            if filter and point_filter:
                raise Exception("Either provide 'filter' or 'point_filter'.\nPlease use 'point_filter' instead of 'filter'. 'filter' is deprecated.")
            if point_filter:
                req.subscribe.point_cloud.filter.CopyFrom(point_filter)
            if filter:
                warnings.warn("Please use 'point_filter' instead of 'filter'. 'filter' is deprecated.", DeprecationWarning, stacklevel=2)
                req.subscribe.point_cloud.filter.CopyFrom(filter)
            if reference_frame:
                req.subscribe.point_cloud.reference_frame.CopyFrom(reference_frame)
            if extend_subscribe_request:
                req.subscribe.point_cloud.MergeFrom(extend_subscribe_request)

            self._metadata.header.device.CopyFrom(self._connection.send_request(req).event.point_cloud.header)

            self._metadata.header.client.library_version = version.__version__
            self._metadata.header.client.file_time_ns = int(time.time() * 1e9)
            self._metadata.header.client.language = general_pb2.Client.Language.PYTHON
        else:
            # Open file
            self._file_name = from_file
            self._file = gzip.open(self._file_name, 'rb')
            try:
                self._file.seek(0, 2)
                self._file_size = self._file.tell()
            except EOFError as e:
                self._file_size = None
            self._file.seek(0, 0)

            # Read header
            buf_header = self._get_file_block()
            offset_data = self._file.tell()
            self._metadata.header.ParseFromString(buf_header)
            self._stream_buffered = False

            if self._file_size:
                # Find footer offset
                while self._file_size > self._file.tell():
                    offset_footer = self._file.tell()
                    self._get_file_block()
    
                # Read footer
                self._file.seek(offset_footer)
                if self._read_stream():
                    self._metadata.footer.CopyFrom(self._stream_data.footer)
                    self._stream_buffered = False
            self._file.seek(offset_data)

    def __del__(self):
        self.close()

    def __str__(self):
        return "<Blickfeld Point Cloud Stream: %u frames, %u returns, %s>" % (
            self._metadata.footer.stats.counter.frames,
            self._metadata.footer.stats.counter.returns,
            ("device '%s'" % (self._metadata.header.device.cube_serial_number)) if self._connection else (
                        "file '%s'" % (self._file_name))
        )

    def close(self):
        """ Close point cloud stream and connection. Always call this function before deleting a point_cloud_stream object.
        """
        self.stop_recording()

        if self._connection:
            self._connection.close()
        else:
            self._file.close()

    def _get_file_block(self):
        buf = self._file.read(4)
        len, offset = _DecodeVarint32(buf, 0)
        buf = buf[offset:] + self._file.read(len - 4 + offset)
        return buf

    def _read_stream(self):
        if self._file_size and self._file_size == self._file.tell():
            self._stream_buffered = False
            return False

        self._stream_data.Clear()
        try:
            self._stream_data.ParseFromString(self._get_file_block())
            self._stream_buffered = True
        except EOFError as e:
            self._stream_buffered = False
        return self._stream_buffered

    def get_metadata(self):
        """ Get metadata of point cloud stream

        :return: Point cloud metadata, see: :any:`protobuf_protocol` PointCloud.Metadata
        """
        return self._metadata

    def recv_frame(self, fail_on_lost_frames=False):
        """ Receive point cloud frame

        :param fail_on_lost_frames: An exception will be raised when a frame is lost. Please check performance of client or network. If file recording is enabled the compressions level of gzip can be reduced and therefore less CPU power is needed. See :py:func:`record_to_file`

        :return: point cloud frame with all the data in it, see: :any:`protobuf_protocol` Frame
        """

        # If stream is closed with CTRL-C it will still write the footer and save the file
        if terminate:
            self.close()
            raise Exception("Detected SIGINT during recv_frame, point cloud stream closed ungracefully.")

        if self._connection:
            frame = self._connection.recv().event.point_cloud.frame
        else:
            if self.end_of_stream():
                raise ConnectionAbortedError("Reached end of stream. Use end_of_stream() before calling recv_frame().")
            self._stream_buffered = False

            frame = self._stream_data.frame

        if fail_on_lost_frames:
            if self._last_frame_id and self._last_frame_id + 1 != frame.id:
                tmp_last_frame_id = self._last_frame_id
                self._last_frame_id = frame.id
                ex_str = "Lost a point cloud frame. Client or network performance is not sufficient. " \
                         "Last received frame id: %d, current frame id: %d." % (tmp_last_frame_id, frame.id)
                if self._ofile:
                    ex_str += "\nReducing the compressions level in the record_to_file function could prevent frame losses."

                raise RuntimeError(ex_str)
            self._last_frame_id = frame.id

        # Record frame to file
        if self._ofile:
            self._save_frame_to_file(frame)

        return frame

    def end_of_stream(self):
        """ Check whether stream has ended. This function is required for point cloud streams of files

        :return: Bool if end of stream is reached
        """
        if not self._stream_buffered:
            self._read_stream()
        return not self._stream_buffered or self._stream_data.HasField('footer')

    def _save_frame_to_file(self, frame):
        """ Save a pointcloud frame to a file. Write Metadata into the footer
        """
        self._stream_data.frame.CopyFrom(frame)
        # Add events for changed scan pattern to footer
        scan_pattern_str = self._stream_data.frame.scan_pattern.SerializeToString()
        if self._prev_scan_pattern_str != scan_pattern_str:
            event = self._metadata.footer.events.add()
            event.from_frame_id = self._stream_data.frame.id
            event.scan_pattern.CopyFrom(self._stream_data.frame.scan_pattern)

            self._prev_scan_pattern_str = scan_pattern_str

        # Update stats in footer
        counter = self._metadata.footer.stats.counter
        counter.frames = counter.frames + 1
        counter.points = counter.points + self._stream_data.frame.total_number_of_points
        counter.returns = counter.returns + self._stream_data.frame.total_number_of_returns

        _EncodeVarint(self._ofile.write, self._stream_data.ByteSize())
        self._ofile.write(self._stream_data.SerializeToString())

    def record_to_file(self, file_name, compresslevel=1):
        """ Record point cloud stream to file

        :param file_name: Path to the file where it should be dumped
        :param compresslevel: The compresslevel argument is an integer from 0 to 9 controlling the level of compression; 1 is fastest and produces the least compression, and 9 is slowest and produces the most compression. 0 is no compression. The default is 1. If frames are lost during the recording decrease the compression level.

        .. deprecated:: 2.13.0
            Since BSL v2.13.0 this function is deprecated, because of performance issues. Please use :py:func:`blickfeld_scanner.scanner.scanner.record_point_cloud_stream` for a performance improved recording.
        """
        warnings.warn("This function is deprecated, because of performance issues. Please use scanner.record_point_cloud_stream for a performance improved recording.", DeprecationWarning, stacklevel=2)

        if self._ofile:
            raise Exception("The output file has already been opened. To open another file, please call 'stop_recording' first to close the current output file.")
        self._prev_scan_pattern_str = b""
        self._metadata.ClearField("footer")
        self._ofile = gzip.open(file_name, 'wb', compresslevel=compresslevel)
        _EncodeVarint(self._ofile.write, self._metadata.header.ByteSize())
        self._ofile.write(self._metadata.header.SerializeToString())

    def stop_recording(self):
        """ Stop recording. Always call this function or the :py:func:`blickfeld_scanner.stream.point_cloud.point_cloud.close` function before deleting a point_cloud object.
        """
        if self._ofile:
            self._stream_data.footer.CopyFrom(self._metadata.footer)
            _EncodeVarint(self._ofile.write, self._stream_data.ByteSize())
            self._ofile.write(self._stream_data.SerializeToString())

            self._ofile.close()
            self._ofile = None

    def stop(self):
        """ Stop and unsubscribe of the stream
        """
        self.stop_recording()
        
        if self._connection:
            req = connection_pb2.Request()
            req.unsubscribe.point_cloud.SetInParent()
            self._connection.send(req)
        else:
            raise NotImplementedError("File stream not supported.")
