#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
from __future__ import print_function
import argparse
import sys
import numpy as np
import blickfeld_scanner
from blickfeld_scanner.protocol.config import scan_pattern_pb2
from blickfeld_scanner.scanner import protocol_exception

def example(args):
    """ Example to for handling protocol exceptions.

    The errors are described in the protobuf protocol definition of the errors.

    This example tries to set an invalid ScanPattern and a protocol exception will be raised and catched.

    :param args: arguments to parse out the host name or IP address of the device
    """

    device = blickfeld_scanner.scanner(args.host)  # Connect to the device

    config = scan_pattern_pb2.ScanPattern()   # Create a scan pattern, see protobuf protocol definition of the scan pattern
    config.horizontal.fov = np.deg2rad(10)
    config.vertical.fov = np.deg2rad(30)
    config.vertical.scanlines_up = 10  # Set invalid values
    config.vertical.scanlines_down = 30
    config.pulse.frame_mode = scan_pattern_pb2.ScanPattern.Pulse.FrameMode.Value("COMBINE_UP_DOWN")

    try:
        device.fill_scan_pattern(config)  # Try to fill the remaining fields with default values
    except protocol_exception as pe:  # Catch protocol exception and print it
        print(pe)

    try:
        device.set_scan_pattern(config)  # Send scan pattern to device
    except protocol_exception as pe:  # Catch protocol exception and print it
        print(pe)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("--host", default="localhost", required=True, help="hostname or IP of device") # host name or IP address of the device
    args = parser.parse_args()  # Parse command line arguments

    example(args)  # Start example
