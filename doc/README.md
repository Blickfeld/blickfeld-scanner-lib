# General

The blickfeld-scanner-lib (BSL) is the interface for communicating with Blickfeld LiDAR devices.

There are some built-in functions, which provide basic functionality. For more advanced use, Protocol Buffers (protobuf) is required.
Protocol Buffers is a language-neutral, platform-neutral, extensible mechanism for serializing structured data.

The BSL uses, Protocol Buffers to communicate  with the LiDAR device. The main communication features have corresponding functions in the BSL.
For some of these functions, parameters packed into a Protocol Buffer object are required.

## Installation

Follow the [installation guide](install).

## Protocol

The following sections describe the current state of the TCP/IP protocol that is used to communicate with Blickfeld’s LiDAR devices.
It is based on Protocol Buffers. For a more detailed description of Protocol Buffers,
please refer to [https://developers.google.com/protocol-buffers](https://developers.google.com/protocol-buffers)

Client implementations are currently available for C++, Python, and TypeScript.
It is recommended to read through the examples and the client wrapper.

### Connection

The TCP/IPv4 connection to the device is established at TCP port 8000.
The connection is used to send control requests to the device – simple requests are client-initiated.

Each request sent by the client will trigger a response to be sent by the device.

The available requests are documented in the [Connection Protocol Documentation](protobuf_protocol.html#blickfeld/connection.proto).

### Stream Requests

In addition to the control requests, there are also special stream requests.

To initiate a stream, a subscribe command has to be sent, e.g., for point cloud data. For every recorded frame, an event response is passed on to the client.
As the events are sent asynchronously, the client has to handle events and normal responses. In order to terminate a subscription, the stream has to be closed.
For non-permanent streams or data-intensive streams, it is recommended to open a separate connection.

The available stream requests are documented in the [Connection Protocol Documentation](protobuf_protocol.html#blickfeld/stream/connection.proto).

## License

Copyright (c) 2020, Blickfeld GmbH
All rights reserved.

This source code is licensed under the BSD-style license found in the
[LICENSE.md](LICENSE.md) file in the root directory of this source tree.