#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
# > Introduced in BSL v2.10 and firmware v1.9
#
from __future__ import print_function

import argparse
import blickfeld_scanner
from blickfeld_scanner.stream import point_cloud
from blickfeld_scanner.protocol.config import scan_pattern_pb2


def configure_point_cloud_stream(args):
    """Fetch a basic point cloud. Only get specific fields of each point: X, Y, Z, intensity and ID.
    Use a filter to filter out points which are closer than 5m or further away than 50m.

    :param args: arguments to parse out the hostname or IP address of the device and an optional file path to record to.
    """
    device = blickfeld_scanner.scanner(args.target)  # Connect to the device
    
    # Reference Frame with X, Y, Z coordinates, intensity and point IDs
    # The frame represents the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
    reference_frame = point_cloud.REF_FRAME_XYZ_I_ID  

    # Create filter to filter points and returns by point attributes during the post-processing on the device.
    point_filter = scan_pattern_pb2.ScanPattern().Filter()
    point_filter.range.minimum = 5  # minimum range for a point to be sent out is 5m
    point_filter.range.maximum = 50  # maximum range for a point to be sent out is 50m
    point_filter.max_number_of_returns_per_point = 2 # Set max number of returns to 2. The default value is 1.

    stream = device.get_point_cloud_stream(filter=point_filter, reference_frame=reference_frame)  # Create a point cloud stream object
    
    while True:
        # Receive frame. It has a reduced data size, as not all fields are requested. This also speeds up the Protobuf encoding & decoding. 
        frame = stream.recv_frame()
        # Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
        # Protobuf API is described in https://developers.google.com/protocol-buffers/docs/pythontutorial
        print(f"Got {frame}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("target", help="hostname or IP of scanner to connect to")  # host name or IP address of the device

    args = parser.parse_args()  # Parse command line arguments
    configure_point_cloud_stream(args)  # Start example
    
