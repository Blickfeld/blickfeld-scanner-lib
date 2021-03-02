#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
from __future__ import print_function

import blickfeld_scanner
import logging
import sys
import argparse
import math

from blickfeld_scanner.protocol.config import scan_pattern_pb2

# Enable logging to console
logging.basicConfig(stream=sys.stdout, level=logging.INFO)

def example(args):
    """ Example function for synchronizing several devices.

    Call this script with a list of hosts (the IP addresses or the hostnames of the devices).

    Some other parameters can be changed: frame_rate, vertical_fov, scan lines, pulse_mode, ntp_server_ip.

    To get help, call the script with -h.

    :param args: argparser will parse incoming arguments (hosts, frame_rate, vertical_fov, scan lines, pulse_mode, ntp_server_ip)
    """

    devices = [blickfeld_scanner.scanner(host) for host in args.hosts]  # Connect to the device

    config = scan_pattern_pb2.ScanPattern()   # Create a scan pattern, see :any:`protobuf_protocol` ScanPattern
    if args.vertical_fov:
        config.vertical.fov = args.vertical_fov / 180 * math.pi  # Set vertical FoV in rad
    config.vertical.scanlines_up = args.scanlines[0]  # Set up scan lines
    config.vertical.scanlines_down = args.scanlines[1]  # Set down scan lines
    config.pulse.frame_mode = scan_pattern_pb2.ScanPattern.Pulse.FrameMode.Value(args.pulse_mode)  # Set pulse mode

    if args.ntp_server_ip != "":  # If the ntp_server_ip variable is set, set the ntp servers
        for device in devices:  # iterate through devices
            device_ntp_server_ip = device.get_ntp_server()  # Get ntp server ip address of the device
            print("On device: " + device.hostname_or_ip  + " current ntp server is: " + device.get_ntp_server())
            if device_ntp_server_ip != args.ntp_server_ip:  # It the ntp server ip address is different to the
                # given ntp_server_ip set the ntp server ip address to the given ntp_server_ip
                device.set_ntp_server(args.ntp_server_ip)
                print("On device: " + device.hostname_or_ip  + " new ntp server is: " + device.get_ntp_server())
    else:
        print("Requested pattern:", config)
        print(devices[0].fill_scan_pattern(config))
        blickfeld_scanner.scanner.sync(devices=devices, scan_pattern=config, target_frame_rate=args.frame_rate)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("--hosts", default="localhost", nargs='+', required=True, help="hostname or IP of scanners")  # host names or IP addresses of the devices
    parser.add_argument("-f", "--frame_rate", type=float, default=None, help="frame rate (default is maximum)")  # desired frame rate
    parser.add_argument("-v", "--vertical_fov", type=float, default=None, help="vertical field of view in deg")  # desired vertical FoV, see :any:`protobuf_protocol` ScanPattern
    parser.add_argument("-s", "--scanlines", nargs=2, metavar=('up', 'down'), type=int, default=[30, 30], help="Scan lines up and down")  # desired scan lines up and down, see :any:`protobuf_protocol` ScanPattern
    parser.add_argument("-p", "--pulse_mode", default="COMBINE_UP_DOWN", choices=["ONLY_UP", "ONLY_DOWN", "COMBINE_UP_DOWN"], help="Pulse mode")  # desired pulse mode, see :any:`protobuf_protocol` ScanPattern
    parser.add_argument("-t", "--ntp_server_ip", default="", help="ip of the ntp server")  # IP address of ntp server
    args = parser.parse_args()  # Parse command line arguments

    example(args)  # Start example
