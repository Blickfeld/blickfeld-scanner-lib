#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
import argparse
import blickfeld_scanner


def getting_started_example(args):
    """ See "Getting Started" example in the Blickfeld-Scanner-Library documentation for a in depth explanation.

    Fetch a pointcloud frame of a device and show some values which are saved in a frame.

    :param args: arguments to parse out the host name or IP address of the device
    """
    scanner = blickfeld_scanner.scanner(args.host)  # Connect to the device
    print(scanner)
    stream = scanner.get_point_cloud_stream()  # Create a point cloud stream object
    print(stream)
    frame = stream.recv_frame()  # Receive a frame

    print(frame.start_time_ns / 1e9)  # Get a field value
    print(frame.scan_pattern.frame_rate.maximum)  # Get a field in a nested protobuf object
    print(frame.scanlines[0].points[0])  # Get a value of a repeated nested field

    # Iterate through all the scanlines in a frame
    for s_ind in range(len(frame.scanlines)):

        # Iterate through all the points in a scanline
        for p_ind in range(len(frame.scanlines[s_ind].points)):
            point = frame.scanlines[s_ind].points[p_ind]

            # Iterate through all the returns for each points
            for r_ind in range(len(point.returns)):
                ret = point.returns[r_ind]
                if s_ind < 10:  # Print the return coordinates of the first 10 points
                    print(f"coordinates: ({ret.cartesian[0]}, {ret.cartesian[1]}, {ret.cartesian[2]})")

    del stream  # Stop pointcloud stream

if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("--host", default="localhost", required=True, help="hostname or IP of device") # host name or IP address of the device
    args = parser.parse_args()  # Parse command line arguments

    getting_started_example(args)  # Start example