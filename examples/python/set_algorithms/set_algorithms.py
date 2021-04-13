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


def set_algorithms(target, persist, background_subtraction, neighbor_filter):
    """Sets post-processing algorithms on the device.

    This basic example just activates the algorithm with their default parameter set.
    """
    device = blickfeld_scanner.scanner(args.target)  # Connect to the device
    device.set_default_point_cloud_algorithms(
        persist=persist, 
        background_subtraction=background_subtraction,
        neighbor_filter=neighbor_filter,
    )

    print("Applied algorithms in advanced configuration")
    print(device.get_advanced_config().server.default_point_cloud_subscription.algorithms)
    print("Start a device stream to see the algorithms in action")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("target", help="hostname or IP of scanner to connect to")  # host name or IP address of the device
    parser.add_argument("-p", "--persist", help="Persist config on device", action='store_true')
    parser.add_argument("-bgs", "--background_subtraction", help="Enable background subtraction", action='store_true')
    parser.add_argument("-nf", "--neighbor_filter", help="Enable neighbor noise filter", action='store_true')

    args = parser.parse_args()  # Parse command line arguments
    set_algorithms(args.target, args.persist, args.background_subtraction, args.neighbor_filter)  # Start example
