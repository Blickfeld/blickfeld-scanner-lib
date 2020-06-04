# blickfeld-scanner-lib

The blickfeld-scanner-lib (BSL) is the interface for communicating with Blickfeld LiDAR devices.

There are some built-in functions, which provide basic functionality. For more advanced use, Protocol Buffers (protobuf) is required.
Protocol Buffers is a language-neutral, platform-neutral, extensible mechanism for serializing structured data.

The BSL uses, Protocol Buffers to communicate  with the LiDAR device. The main communication features have corresponding functions in the BSL.
For some of these functions, parameters packed into a Protocol Buffer object are required.

## Installation

Follow the [installation guide](https://docs.blickfeld.com/external/blickfeld-scanner-lib/install.html).

### Python

```bash
pip install blickfeld_scanner
```

### CPP

```bash
sudo apt update
sudo apt install -y wget libprotobuf-dev libprotobuf17
wget https://github.com/Blickfeld/blickfeld-scanner-lib/releases/download/latest/blickfeld-scanner-lib-dev-Linux.deb
sudo dpkg -i blickfeld-scanner-lib-dev-Linux.deb
```

Follow the [getting started guide](https://docs.blickfeld.com/external/blickfeld-scanner-lib/getting_started.html).

## Documentation

The technical documentation can be found at [docs.blickfeld.com](https://docs.blickfeld.com).

The BSL part can be found at https://docs.blickfeld.com/external/blickfeld-scanner-lib/README.html.

## License

Copyright (c) 2020, Blickfeld GmbH
All rights reserved.

This source code is licensed under the BSD-style license found in the
[LICENSE.md](LICENSE.md) file in the root directory of this source tree.
