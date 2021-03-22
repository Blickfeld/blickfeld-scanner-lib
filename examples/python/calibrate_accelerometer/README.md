# Calibrate Accelerometer

Calibrate the rotational offset of the Blickfeld Cube 1 Inertial Measurement Unit (IMU).

The upright pose is identified by the static acceleration reading [0, 0, -1]. This means, that the gravitational acceleration is measured along the negative direction of the devices Z-Axis.

Place the Blickfeld Cube 1 on a level surface for calibrating the IMU. Avoid any kind of movement of the Blickfeld Cube 1 while running the script.

If the Blickfeld Cube 1 has already configured a rotational offset remove it first by running this script with the '--remove' flag.

```.. literalinclude:: calibrate_accelerometer.py
    :language: python
```
