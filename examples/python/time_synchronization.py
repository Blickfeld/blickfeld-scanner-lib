#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
from __future__ import print_function

import blickfeld_scanner
import argparse
import time

from blickfeld_scanner.protocol.config import scan_pattern_pb2


def example(args):
    """ Example function for activating time synchronization on devices.

    You can choose NTP or PTP synchronization and as an example two different parameters for each are set.

    In the end the status of the synchronization will be read.

    :param args: argparser will parse incoming arguments (host, ptp, ntp)
    """

    device = blickfeld_scanner.scanner(args.host)  # Connect to the device

    print("Starting time_synchronization..");
    device.set_time_synchronization(ntp=args.ntp, ptp=args.ptp)  # Set time synchronization method. A list of ntp server or a list of ptp unicast destinations can be given as arguments as well.

    print(device.get_status().time_synchronization)  # Print time synchronization status


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("host", default="localhost", help="hostname or IP of scanner")  # host name or IP addresse of the device
    parser.add_argument("--ptp", action='store_true', help="PTP synchronization")  # Start PTP synchronization
    parser.add_argument("--ntp", action='store_true', help="NTP synchronization")  # Start NTP synchronization
    args = parser.parse_args()  # Parse command line arguments

    example(args)  # Start example
