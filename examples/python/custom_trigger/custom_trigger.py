#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
from __future__ import print_function

import argparse
import math
import blickfeld_scanner
from blickfeld_scanner.protocol.config import scan_pattern_pb2

def custom_trigger(target, horizontal_angle):
    """Configure custom triggers and retrieve a point cloud with these settings

    This example will stop after 10 received frames.

    :param target: hostname or IP address of the device
    :param horizontal_angle: desired trigger angle per scanline in radians
    """
    scanner = blickfeld_scanner.scanner(target)

    # retrieve currently active scan pattern
    pattern = scanner.get_scan_pattern()
    
    # set pulse type to custom; we are goign to overwrite it
    pattern.pulse.type = pattern.pulse.CUSTOM
    custom_pattern = pattern.pulse.custom

    # define a single trigger angle
    at = scan_pattern_pb2.ScanPattern.Pulse.Custom.AngleTrigger.Angle()
    # set the desired horizontal trigger angle (per scanline)
    at.horizontal_angle = horizontal_angle
    # set the desired trigger signals we want to have. TRG_LASER for regular acquisition, and TRG_EXTERNAL_0 for a GPIO output (not available with regular Cubes)
    at.enabled_triggers.extend([ scan_pattern_pb2.ScanPattern.Pulse.Custom.TRG_LASER, scan_pattern_pb2.ScanPattern.Pulse.Custom.TRG_EXTERNAL_0 ])

    # reset currently configured trigger angles
    del custom_pattern.angle_trigger.angles[:]
    # and add our single trigger to trigger list
    custom_pattern.angle_trigger.angles.extend([at])

    print("Configuration set send to device:")
    print(custom_pattern)

    scanner.set_scan_pattern(pattern)

    # starting stream
    stream = scanner.get_point_cloud_stream()

    for i in range(10):
        frame = stream.recv_frame()
        print(f"Got {frame}")

    stream.stop()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('host')
    parser.add_argument('horizontal_angle', type=float)
    args = parser.parse_args()
    
    custom_trigger(args.host, math.radians(args.horizontal_angle))