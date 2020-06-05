#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
from __future__ import print_function

import argparse
import blickfeld_scanner


def fetch_point_cloud(args):
    """Fetch the point cloud of a device and print the frame ID and number of returns in the received frame.

    If the record_to_file parameter is set with a path and a filename the stream will be recorded to this file.

    Stop this example with CTRL+C. The file should be closed in a correct way.

    :param args: arguments to parse out the host name or IP address of the device and optional file path to record to.
    """
    device = blickfeld_scanner.scanner(args.target)  # Connect to the device
    
    stream = device.get_point_cloud_stream()  # Create a point cloud stream object
    
    if args.record_to_file:
        stream.record_to_file(args.record_to_file + ".bfpc")  # Record the frames to a file
    while True:
        frame = stream.recv_frame()  # Receive a frame. This frame will also be dumped into the file
        # Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
        # Protobuf API is described in https://developers.google.com/protocol-buffers/docs/pythontutorial
        print(f"Got {frame}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("--target", "-t", help="hostname or IP of scanner to connect to")  # host name or IP address of the device
    parser.add_argument("--record_to_file", "-r", default=None, help="Record point cloud to file")  # optional filename/path to record to
    
    args = parser.parse_args()  # Parse command line arguments
    fetch_point_cloud(args)  # Start example
    
