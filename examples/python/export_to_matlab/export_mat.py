#
# Copyright (c) 2020 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.
#
import argparse
import numpy as np

import blickfeld_scanner


def get_frame(host: str):
    """ Fetches a pointcloud frame of a device and dump the sample.

    See "Getting Started" example in the Blickfeld-Scanner-Library documentation for a in depth explanation.

    :param host: the host name or IP address of the device
    :type host: str
    """
    scanner = blickfeld_scanner.scanner(args.host)  # Connect to the device
    print("connected to device, starting stream...")
    stream = scanner.get_point_cloud_stream(filter=None)  # Create a verbose point cloud stream object
    stream.record_to_file("frame.bfpc")  # Record the frame(s) to file
    frame = stream.recv_frame()  # Receive a frame
    print("got measurements, closing stream...")
    stream.close()  # close immediately, we only need one frame
    return frame


def adjacency(frame: blickfeld_scanner.protocol.data.frame_pb2.Frame):
    """Converts a BSL frame into a dict of numpy matrices of adjacent measurements.

    BEWARE:
    - these are not OpenCV compatible matrices, they rather use the original datatype
    - the samples are anisotropic, see section "Scan pattern" in the documentation

    :param frame: original time-ordered measurement frame
    :type frame:  blickfeld_scanner.protocol.data.frame_pb2.Frame
    :return: adjacent matrices per property
    :rtype: dict
    """

    # assert ONLY_UP so scanlines are of strictly monotonically increasing opening angle
    if frame.scan_pattern.pulse.frame_mode != 2:
        raise NotImplementedError("sampling mode must be ONLY_UP")

    line_count = frame.scan_pattern.vertical.scanlines_up  # scanline count
    mat_size = (line_count,
                len(frame.scanlines[0].points))  # points per line

    # reserve uninitialized memory of datatype from point.proto
    ambimat = np.empty(mat_size, np.uint32)
    timemat = np.empty(mat_size, np.uint64)
    azimmat = np.empty(mat_size, np.float)
    elevmat = np.empty(mat_size, np.float)
    distmat = np.empty(mat_size, np.float)  # not every


    assert azimmat.size == frame.total_number_of_points, "malformed frame"

    for line_id, line in enumerate(frame.scanlines):
        # reconstruct scanline position from sampling order in ONLY_UP
        lower = line_id % 2 == 0
        if lower:
            row_id = int(np.floor(line_count / 2) + np.floor(line_id / 2))
        else:
            row_id = int(np.floor(line_count / 2) - np.floor(line_id / 2) - 1)
        insert_dir = 1 if lower else -1

        # cache nd-array accessors
        ambirow = ambimat[row_id][::insert_dir]
        timerow = timemat[row_id][::insert_dir]
        azimrow = azimmat[row_id][::insert_dir]
        elevrow = elevmat[row_id][::insert_dir]
        distrow = distmat[row_id][::insert_dir]

        for point_id, point in enumerate(line.points):
            ambirow[point_id] = point.ambient_light_level
            timerow[point_id] = point.start_offset_ns
            azimrow[point_id] = point.direction.azimuth
            elevrow[point_id] = point.direction.elevation
            if len(point.returns) > 0:
                distrow[point_id] = point.returns[0].range
            else:
                distrow[point_id] = np.NaN

    return {'azimuth': azimmat,
            'elevation': elevmat,
            'ambient_light_level': ambimat,
            'range': distmat,
            'start_offset_ns': timemat}


def export_mats(args):
    """Example on how to export a Cube Frame for Matlab, Octave, and OpenCV.

    Fetches a frame from a given Cube, and dumps it for reference.
    The measurements are rearranged in 2D matrices of adjacent measurements.
    These are then optionally serialized as 'frame.mat' for Matlab and Octave,
    or as 'frame.png' using OpenCV.

    :param args: arguments to parse out the hostname or IP address of the device and export switches.
    """
    raw_frame = get_frame(args)  # frame acquisition, see also 'fetch_point_cloud' example

    mats = adjacency(raw_frame)  # convert to matrix format

    if args.mat:
        try:
            from scipy.io import savemat
        except ImportError as ie:
            print("please install SciPy for this example to work (e.g. pip install scipy)")
            raise ie
        savemat("frame.mat", mats)
        print("saved to 'frame.mat'")  # further example code for  matlab / octave in 'export_mat.m'")
    if args.imwrite:
        try:
            import cv2
        except ImportError as ie:
            print("please install OpenCV (e.g. pip3 install opencv-python)")
            raise ie
        # "OpenCV would raise 'TypeError: Expected Ptr<cv::UMat>' for uint32
        amb = mats["ambient_light_level"]
        img = np.interp(amb, (amb.min(), amb.max()), (0, 255))  # normalize
        if cv2.imwrite("frame.png", np.uint8(img)):  # now we can use CV directly
            print("saved to 'frame.png'")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("host", help="hostname or IP of device")
    parser.add_argument("-m", "--mat", action='store_false',
                        help="use SciPy to write frame.mat for matlab/octave import")
    parser.add_argument("-i", "--imwrite", action='store_true', help="use OpenCV to save ambient ght image")
    args = parser.parse_args()  # Parse command line arguments
    export_mats(args)
