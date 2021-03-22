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

import numpy as np
np.set_printoptions(threshold=5)

def fetch_imu(target):
    """Fetch the IMU data and print it.

    This example will stop after 10 received IMU bursts.

    :param target: hostname or IP address of the device
    """
    device = blickfeld_scanner.scanner(target)  # Connect to the device

    stream = device.get_imu_stream(as_numpy=True)  # Create a point cloud stream object

    for i in range(10):
        header, data = stream.recv_burst_as_numpy()  # Receive an IMU data burst.
        # Format of frame is described in protocol/blickfeld/data/frame.proto or doc/protocol.md
        # Protobuf API is described in https://developers.google.com/protocol-buffers/docs/pythontutorial
        print(f"Got burst: {header} with {data.dtype}:\n{data}")

        rots = stream.convert_numpy_burst_to_rotations(data)
        print(f" to rotations {rots.dtype}:\n{rots}")

    stream.stop()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("target", help="hostname or IP of scanner to connect to")  # host name or IP address of the device

    args = parser.parse_args()  # Parse command line arguments
    fetch_imu(args.target)  # Start example
