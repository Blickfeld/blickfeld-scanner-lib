#
# Copyright (c) 2021 Blickfeld GmbH.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE.md file in the root directory of this source tree.

from __future__ import print_function

import argparse
from blickfeld_scanner import scanner
import numpy as np
from time import sleep
from blickfeld_scanner.protocol.config.advanced_pb2 import Advanced


def calibrate_accelerometer(args):
    """Calibrate the rotational offset of the Blickfeld Cube 1 Inertial Measurement Unit (IMU).

    The upright pose is identified by the static acceleration reading [0, 0, -1]. This means, that
    the gravitational acceleration is measured along the negative direction of the devices Z-Axis.

    Place the Blickfeld Cube 1 on a level surface for calibrating the IMU.
    Avoid any kind of movement of the Blickfeld Cube 1 while running the script.

    If the Blickfeld Cube 1 has already configured a rotational offset remove it first by running
    this script with the '--remove' flag.
    """

    ORIENTATION_UPRIGHT = [0, 0, -1]
    ERROR_ALLOWED_NORM = 1e-2

    # ensure a given vector is normalized to length 1
    def _unit_vector(v: list) -> np.array:
        return np.array(v) / np.linalg.norm(v)

    # calculate the rotation matrix
    def _calculate_rotation_matrix(acc_imu: list, acc_calib: list = ORIENTATION_UPRIGHT) -> np.array:
        acc_imu = _unit_vector(acc_imu)
        acc_calib = _unit_vector(acc_calib)
        # see https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d
        imu_static_rotation_offset = np.eye(3)
        if np.linalg.norm(np.cross(acc_calib, acc_imu)) < 1e-6:
            imu_static_rotation_offset = -imu_static_rotation_offset
        else:
            axis = np.cross(acc_calib, acc_imu)
            s = np.linalg.norm(axis)
            c = np.dot(acc_calib, acc_imu)
            axis_cross = np.zeros(9)
            np.put(axis_cross, [1, 2, 3, 5, 6, 7], [-axis[2], axis[1], axis[2], -axis[0], -axis[1], axis[0]])
            axis_cross = axis_cross.reshape(3, 3)
            imu_static_rotation_offset = np.eye(3) + axis_cross + np.dot(axis_cross, axis_cross) * (1 - c) / (s ** 2)
        return imu_static_rotation_offset

    device = scanner(args.target)
    print(f"connected to {args.target}")

    cfg = device.get_advanced_config()

    # clear imu_static_rotation_offset and exit
    if args.remove:
        del cfg.processing.imu_static_rotation_offset[:]
        device.set_advanced_config(cfg, persist=args.persist)
        print("static rotation offset removed")
        exit(0)

    # check for configured imu_static_rotation_offset
    if cfg.HasField("processing") and len(cfg.processing.imu_static_rotation_offset) != 0:
        print("imu_static_rotation_offset is already configured")
        print("remove configuration by starting this script with '--remove'")
        exit(0)

    # measure the actual static acceleration by removing configured imu_static_rotation_offset
    del cfg.processing.imu_static_rotation_offset[:]
    device.set_advanced_config(cfg)
    sleep(0.3)

    # calculate and set imu_static_rotation_offset
    imu_static_rotation_offset = _calculate_rotation_matrix(acc_imu=list(device.get_status().imu.static_state.acceleration))
    cfg_new = Advanced()
    cfg_new.MergeFrom(cfg)
    cfg_new.processing.imu_static_rotation_offset[:] = imu_static_rotation_offset.flatten()
    device.set_advanced_config(cfg_new)
    sleep(0.3)

    # check error after calibration
    state = device.get_status()
    acc_imu = _unit_vector(state.imu.static_state.acceleration)
    print(f"offset after calibration: {np.linalg.norm(ORIENTATION_UPRIGHT - acc_imu)} [g]")

    # rollback in case error is too large (e.g. device was moved during calibration)
    if np.linalg.norm(ORIENTATION_UPRIGHT - acc_imu) > ERROR_ALLOWED_NORM:
        print(f"error too large, maximum allowed is {ERROR_ALLOWED_NORM}")
        print("rolling back changes")
        del cfg.processing.imu_static_rotation_offset[:]
        device.set_advanced_config(cfg)
        exit(0)

    device.set_advanced_config(cfg_new, persist=args.persist)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()  # Command line argument parser
    parser.add_argument("target", help="hostname or IP of scanner to connect to")  # host name or IP address of the device
    parser.add_argument("-p", "--persist", help="Persist configuration on device", action="store_true")
    parser.add_argument("-r", "--remove", help="Remove static rotation offset. Enable persist to remove permanently.", action="store_true")

    args = parser.parse_args()
    calibrate_accelerometer(args)
