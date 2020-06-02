File format
===========

Point clouds of Blickfeld scanners are usually recorded in the Blickfeld Point Cloud format with the extension `.bfpc`.
It is based on Protobuf and contains all available meta information, e.g. used scan pattern or firmware versions, and all additional information per point, e.g. timestamp in nanoseconds or scanline id.
The lossless format and the simple Protobuf extensibility with upcoming software upgrades are the main reasons, why the custom format was developed and why it is recommenend for raw recordings.
The dumped recordings can be easily converted with the Blickfeld Recorder to common point cloud formats.

Structure
---------

A point cloud dump is structured into three blocks: First the Header message, then a repeating Data block with encapsulated Frame messages, and concluded by a Data block with a encapsulated Footer message.
All blocks are prefixed with a variable unsigned integer field, which states the length of the consequent, Protobuf encoded, message.
The files are stored with GZIP compression to reduce the structure overhead of Protobuf.

.. mermaid::

    graph TD;
        Header-->Data:Frame;
        Data:Frame-->Data:Frame;
        Data:Frame-->Data:Footer;

The Header contains basic information about the device (e.g. serial number, timestamp, firmware version, etc.) and about the recording client (e.g. library version, timestamp, etc.).
The Header Protobuf definition can be found at `PointCloud.Header <protobuf_protocol.html#blickfeld.protocol.file.PointCloud.Header>`__.

The Data block contains either a Point Cloud Frame or the Footer, which ends the file.
The Point Cloud Frame is aswell stored as Protobuf message, which is defined at `Frame <protobuf_protocol.html#blickfeld.protocol.data.Frame>`__.
The Footer contains further metadata about the recording, which includes statistics, e.g. the total number of frames and points, and configuration events, e.g. active scan patterns in the recording.
The Footer Protobuf definition can be found at `PointCloud.Footer <protobuf_protocol.html#blickfeld.protocol.file.PointCloud.Footer>`__.

The combination of Header and Footer is referred to as `Metadata <protobuf_protocol.html#blickfeld.protocol.file.PointCloud.Metadata>`__ in the library methods.
The Metadata is automatically extracted during loading the Blickfeld Point Cloud recordings and can be used to inspect recordings without parsing all frames in the file.

Expected data sizes
-------------------

.. note:: Measurements and expected data sizes will be available soon.