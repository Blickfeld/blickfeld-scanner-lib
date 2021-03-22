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


def read_point_cloud_file(file_path):
    """Read bfpc dump and print the frame ID and number of returns in the received frame.

    This example will stop, when the dump is completly read out.

    :param file_path: Path to dump file
    """
    file_stream = blickfeld_scanner.stream.point_cloud(from_file=file_path)  # Connect to the device

    print(file_stream.get_metadata())  # print meta data of the dump (footer and header)

    while not file_stream.end_of_stream():
        frame = file_stream.recv_frame()  # Receive a frame.
        # Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
        # Protobuf API is described in https://developers.google.com/protocol-buffers/docs/pythontutorial
        print(f"Got {frame}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("file_path", help="Path to dump file.")  # path to dump file

    args = parser.parse_args()  # Parse command line arguments
    read_point_cloud_file(args.file_path)  # Start example
