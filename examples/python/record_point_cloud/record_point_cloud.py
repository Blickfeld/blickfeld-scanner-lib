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


def record_point_cloud(target, file_name):
    """Record a raw point cloud and print the number of received bytes.

    If the record_to_file parameter is set with a path and a filename, the stream will be recorded to this file.

    :param target: hostname or IP address of the device
    :param file_name: file path to record to
    """
    device = blickfeld_scanner.scanner(args.target)  # Connect to the device

    raw_stream = device.record_point_cloud_stream(file_name)  # Create a raw point cloud stream object

    while True:
        raw_file = raw_stream.recv_bytes()  # Receive a raw file bytes
        print(f"Got {len(raw_file)} bytes")
    
    raw_file = raw_stream.stop()
    print(f"Got {len(raw_file)} bytes and stopped")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("target", help="hostname or IP of scanner to connect to")  # host name or IP address of the device
    parser.add_argument("file_name", help="Record point cloud to file")  # optional filename/path to record to

    args = parser.parse_args()  # Parse command line arguments
    record_point_cloud(args.target, args.file_name)  # Start example
