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
import threading
import time
from google.protobuf.internal.decoder import _DecodeVarint32
from google.protobuf.internal.encoder import _EncodeVarint

from ..protocol import connection_pb2
from ..protocol.file import general_pb2, point_cloud_pb2
from ..protocol.data import frame_pb2
from .. import version

frame_pb2.Frame.__str__ = lambda self: "<Blickfeld Frame %u: %u returns, %.1fx%.1f FoV, %u scanlines>" % (
        self.id, 
        self.total_number_of_returns, 
        self.scan_pattern.horizontal.fov * 180 / math.pi, 
        self.scan_pattern.vertical.fov * 180 / math.pi,
        len(self.scanlines)
    )

class point_cloud(object):
    """ Class to request a point cloud stream

    :param connection: connection to the device
    :type connection: :py:class:`blickfeld_scanner.scanner.connection`
    """
    def __init__(self, connection=None, from_file=None):
        self._metadata = point_cloud_pb2.PointCloud.Metadata()
        
        if connection and from_file:
            raise AttributeError("Either provide connection or from_file.")
        if not connection and not from_file:
            raise AttributeError("Neither connection nor from_file is provided.")
           
        self._ofile = False
        self._thread = None
        self._connection = None 
        self._stream_data = point_cloud_pb2.PointCloud.Data()
        if connection:
            self._connection = connection

            req = connection_pb2.Request()
            req.subscribe.point_cloud.SetInParent()
            self._metadata.header.device.CopyFrom(self._connection.send_request(req).event.point_cloud.header)
            
            self._metadata.header.client.library_version = version.__version__
            self._metadata.header.client.file_time_ns = int(time.time() * 1e9)
            self._metadata.header.client.language = general_pb2.Client.Language.PYTHON
        else:
            # Open file
            self._file_name = from_file
            self._file = gzip.open(self._file_name, 'rb')
            self._file.seek(0, 2)
            self._file_size = self._file.tell()
            self._file.seek(0, 0)
            
            # Read header
            buf_header = self._get_file_block()
            offset_data = self._file.tell()
            self._metadata.header.ParseFromString(buf_header)
            self._stream_buffered = False
            
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
            ("device '%s'" % (self._metadata.header.device.cube_serial_number)) if self._connection else ("file '%s'" % (self._file_name))
        )
        
    def _run(self):
        self._prev_scan_pattern_str = b""
    
        while not self._thread_stop:
            with self._thread_sync:
                self._thread_sync.wait(0.1)
                
                if self._thread_stop:
                    break
                
                # Check if main thread is still active.
                main_thread = None
                for i in threading.enumerate():
                    if i.name == "MainThread":
                        main_thread = i
                if not main_thread.is_alive():
                    break

                if self._stream_data.HasField("frame"):
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

    def close(self):
        """ Close point cloud stream and connection
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
        if self._file_size == self._file.tell():
            self._stream_buffered = False
            return False
        
        self._stream_data.Clear()
        self._stream_data.ParseFromString(self._get_file_block())
        self._stream_buffered = True
        return self._stream_buffered

    def get_metadata(self):
        """ Get metadata of point cloud stream

        :return: Point cloud metadata, see: :any:`protobuf_protocol` PointCloud.Metadata
        """
        return self._metadata

    def recv_frame(self):
        """ Receive point cloud frame

        :return: point cloud frame with all the data in it, see: :any:`protobuf_protocol` Frame
        """
        if self._connection:
            frame = self._connection.recv().event.point_cloud.frame
        else:
            if self.end_of_stream():
                raise Exception("Reached end of stream. Use end_of_stream() before calling recv_frame().")
            self._stream_buffered = False
                
            frame = self._stream_data.frame
        
        # Record frame to file
        if self._ofile:
            with self._thread_sync:
                self._stream_data.frame.CopyFrom(frame)
                self._thread_sync.notify()
        
        return frame
        
    def end_of_stream(self):
        """ Check whether stream has ended. This function is required for point cloud streams of files

        :return: Bool if end of stream is reached
        """
        if not self._stream_buffered:
            self._read_stream()
        return not self._stream_buffered or self._stream_data.HasField('footer')

    def record_to_file(self, file_name):
        """ Record point cloud stream to file

        :param file_name: Path to the file where it should be dumped
        """
        self._ofile = gzip.open(file_name, 'wb')
        _EncodeVarint(self._ofile.write, self._metadata.header.ByteSize())
        self._ofile.write(self._metadata.header.SerializeToString())

        self._thread = threading.Thread(target=self._run)
        self._thread_sync = threading.Condition()
        self._thread_stop = False
        self._thread.start()

    def stop_recording(self):
        """ Stop recording
        """
        if self._thread and self._thread.isAlive():
            self._thread_stop = True
            with self._thread_sync:
                self._thread_sync.notify()
            self._thread.join()

        if self._ofile:
            self._stream_data.footer.CopyFrom(self._metadata.footer)
            _EncodeVarint(self._ofile.write, self._stream_data.ByteSize())
            self._ofile.write(self._stream_data.SerializeToString())

            self._ofile.close()
            self._ofile = None
