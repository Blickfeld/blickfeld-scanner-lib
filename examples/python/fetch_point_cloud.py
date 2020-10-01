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


def fetch_point_cloud(target):
    """Fetch the point cloud of a device and print the frame ID and number of returns in the received frame.

    This example will stop after 10 received frames.

    :param target: hostname or IP address of the device
    """
    device = blickfeld_scanner.scanner(target)  # Connect to the device

    stream = device.get_point_cloud_stream()  # Create a point cloud stream object

    for i in range(10):
        frame = stream.recv_frame()  # Receive a frame. This frame will also be dumped into the file
        # Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
        # Protobuf API is described in https://developers.google.com/protocol-buffers/docs/pythontutorial
        print(f"Got {frame}")
    stream.stop()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("target", help="hostname or IP of scanner to connect to")  # host name or IP address of the device

    args = parser.parse_args()  # Parse command line arguments
    fetch_point_cloud(args.target)  # Start example
