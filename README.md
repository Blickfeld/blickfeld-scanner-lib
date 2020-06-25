# blickfeld-scanner-lib

The blickfeld-scanner-lib (BSL) is the interface for communicating with Blickfeld LiDAR devices.

The library is currently available for Python and C++.
With it, Blickfeld [Cube 1](https://www.blickfeld.com/products/cube-1/) and [Cube Range 1](https://www.blickfeld.com/products/cube-range-1/) devices can be configured and point clouds can be fetched.
Additionally, it supports NTP time synchronization and point cloud recordings.

## Quick start

Follow the [installation guide](https://docs.blickfeld.com/cube/latest/external/blickfeld-scanner-lib/install.html) or find the most common installation commands below.

### Python

```bash
pip install blickfeld_scanner
```

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y wget libprotobuf-dev libprotobuf17
wget https://github.com/Blickfeld/blickfeld-scanner-lib/releases/latest/download/blickfeld-scanner-lib-dev-Linux.deb
sudo dpkg -i blickfeld-scanner-lib-dev-Linux.deb
```

Follow the [getting started guide](https://docs.blickfeld.com/cube/latest/external/blickfeld-scanner-lib/getting_started.html).

## Documentation

The Blickfeld software manual, which includes the BSL documentation, can be found at [docs.blickfeld.com/cube](https://docs.blickfeld.com/cube/latest/external/blickfeld-scanner-lib/README.html).

To find the matching software manual for a specific BSL version, automatic redirect links are available.
Use the link `https://docs.blickfeld.com/cube/redirect/blickfeld-scanner-lib/vx.x.x.html` and replace `vx.x.x` with the BSL version.

## License

Copyright (c) 2020, Blickfeld GmbH
All rights reserved.

This source code is licensed under the BSD-style license found in the
[LICENSE.md](LICENSE.md) file in the root directory of this source tree.
