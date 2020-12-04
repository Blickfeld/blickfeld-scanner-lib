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
from blickfeld_scanner.protocol.config import scan_pattern_pb2


def store_and_set_named_scan_pattern(target):
    """Get a list of named scan patterns of a device and print them.
    Afterwards create a new scan pattern and store it with a name. 
    At last get the named scan patterns again and set the created scan pattern.

    :param target: hostname or IP address of the device
    """
    device = blickfeld_scanner.scanner(target)  # Connect to the device

    named_scan_patterns = device.get_named_scan_patterns()  # Get named scan patterns

    # Print if the scan pattern is a default or custom named scan pattern
    for scan_pattern in named_scan_patterns.configs:
        if scan_pattern.read_only:
            print("'" + str(scan_pattern.name) + "' is a default named scan pattern.")
        else:
            print("'" + str(scan_pattern.name) + "' is a custom named scan pattern.")

    # Set default scan pattern by name
    device.set_scan_pattern(name="Default")
    
    # Create new scan pattern
    new_scan_pattern = scan_pattern_pb2.ScanPattern()
    new_scan_pattern.vertical.scanlines_up = 100
    new_scan_pattern.vertical.scanlines_down = 100
    new_scan_pattern.pulse.frame_mode = scan_pattern_pb2.ScanPattern.Pulse.FrameMode.COMBINE_UP_DOWN

    # Fill scan pattern to add missing parameter (optional)
    new_scan_pattern = device.fill_scan_pattern(new_scan_pattern)

    # Store scan pattern as "python test"
    # This method persists the scan pattern on the device. Use 'device.delete_named_scan_pattern("python-test")' to delete it again.
    device.store_named_scan_pattern("python-test", new_scan_pattern)

    # Check if the stored scan pattern was correctly set (optional, just for visualisation)
    named_scan_patterns = device.get_named_scan_patterns()  # Get named scan patterns
    for scan_pattern in named_scan_patterns.configs:
        if scan_pattern.name == "python-test":
            print("'" + str(scan_pattern.name) + "' succesfully stored.")

    # Set scan pattern by name
    device.set_scan_pattern(name="python-test")

    # Delete scan pattern by name, comment in the next line to try it out
    # device.delete_named_scan_pattern("python-test")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("target", help="hostname or IP of scanner to connect to")  # host name or IP address of the device

    args = parser.parse_args()  # Parse command line arguments
    store_and_set_named_scan_pattern(args.target)  # Start example
