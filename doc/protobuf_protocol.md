# Protobuf definitions
<a name="top"></a>

This is the documentation of the protobuf messages used by the Blickfeld API.

These protobuf messages are used to communicate with Blickfeld devices. You can send requests and receive responses.
The corresponding response to a request has the same name.
It is also possible to request a point cloud stream or a status stream.

The data, such as a point cloud, are also packed in protobuf messages.

## Table of Contents

- [blickfeld/common.proto](#blickfeld/common.proto)
    - [OptionalValueRange](#blickfeld.protocol.OptionalValueRange)
    - [ValueRange](#blickfeld.protocol.ValueRange)
  
  
  
  

- [blickfeld/connection.proto](#blickfeld/connection.proto)
    - [Request](#blickfeld.protocol.Request)
    - [Request.Developer](#blickfeld.protocol.Request.Developer)
    - [Request.FillScanPattern](#blickfeld.protocol.Request.FillScanPattern)
    - [Request.GetScanPattern](#blickfeld.protocol.Request.GetScanPattern)
    - [Request.Hello](#blickfeld.protocol.Request.Hello)
    - [Request.RunSelfTest](#blickfeld.protocol.Request.RunSelfTest)
    - [Request.SetScanPattern](#blickfeld.protocol.Request.SetScanPattern)
    - [Request.Status](#blickfeld.protocol.Request.Status)
    - [Response](#blickfeld.protocol.Response)
    - [Response.Developer](#blickfeld.protocol.Response.Developer)
    - [Response.FillScanPattern](#blickfeld.protocol.Response.FillScanPattern)
    - [Response.GetScanPattern](#blickfeld.protocol.Response.GetScanPattern)
    - [Response.Hello](#blickfeld.protocol.Response.Hello)
    - [Response.RunSelfTest](#blickfeld.protocol.Response.RunSelfTest)
    - [Response.SetScanPattern](#blickfeld.protocol.Response.SetScanPattern)
  
    - [Format](#blickfeld.protocol.Format)
  
  
  

- [blickfeld/error.proto](#blickfeld/error.proto)
    - [Error](#blickfeld.protocol.Error)
    - [Error.ConnectionClosed](#blickfeld.protocol.Error.ConnectionClosed)
    - [Error.Empty](#blickfeld.protocol.Error.Empty)
    - [Error.HardwareError](#blickfeld.protocol.Error.HardwareError)
    - [Error.InvalidRequest](#blickfeld.protocol.Error.InvalidRequest)
    - [Error.NoDeviceDiscovered](#blickfeld.protocol.Error.NoDeviceDiscovered)
    - [Error.NotAllowed](#blickfeld.protocol.Error.NotAllowed)
    - [Error.NotFound](#blickfeld.protocol.Error.NotFound)
    - [Error.NotImplemented](#blickfeld.protocol.Error.NotImplemented)
    - [Error.NotInRange](#blickfeld.protocol.Error.NotInRange)
    - [Error.OutdatedClientProtocol](#blickfeld.protocol.Error.OutdatedClientProtocol)
    - [Error.OutdatedServerProtocol](#blickfeld.protocol.Error.OutdatedServerProtocol)
    - [Error.ScannerBusy](#blickfeld.protocol.Error.ScannerBusy)
    - [Error.ServerImplementation](#blickfeld.protocol.Error.ServerImplementation)
    - [Error.SystemStop](#blickfeld.protocol.Error.SystemStop)
    - [Error.TimeSyncFailed](#blickfeld.protocol.Error.TimeSyncFailed)
    - [Error.Unknown](#blickfeld.protocol.Error.Unknown)
    - [Error.UnknownErrorCode](#blickfeld.protocol.Error.UnknownErrorCode)
    - [Error.WrongOperationMode](#blickfeld.protocol.Error.WrongOperationMode)
  
  
  
  

- [blickfeld/options.proto](#blickfeld/options.proto)
  
  
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
  
  

- [blickfeld/config/scan_pattern.proto](#blickfeld/config/scan_pattern.proto)
    - [ScanPattern](#blickfeld.protocol.config.ScanPattern)
    - [ScanPattern.FrameRate](#blickfeld.protocol.config.ScanPattern.FrameRate)
    - [ScanPattern.Horizontal](#blickfeld.protocol.config.ScanPattern.Horizontal)
    - [ScanPattern.Pulse](#blickfeld.protocol.config.ScanPattern.Pulse)
    - [ScanPattern.Vertical](#blickfeld.protocol.config.ScanPattern.Vertical)
  
    - [ScanPattern.Pulse.FrameMode](#blickfeld.protocol.config.ScanPattern.Pulse.FrameMode)
    - [ScanPattern.Pulse.Type](#blickfeld.protocol.config.ScanPattern.Pulse.Type)
  
  
  

- [blickfeld/config/secure.proto](#blickfeld/config/secure.proto)
    - [Secure](#blickfeld.protocol.config.Secure)
  
    - [Secure.CertType](#blickfeld.protocol.config.Secure.CertType)
    - [Secure.Permissions](#blickfeld.protocol.config.Secure.Permissions)
  
  
  

- [blickfeld/data/frame.proto](#blickfeld/data/frame.proto)
    - [Frame](#blickfeld.protocol.data.Frame)
  
  
  
  

- [blickfeld/data/point.proto](#blickfeld/data/point.proto)
    - [Point](#blickfeld.protocol.data.Point)
    - [Point.Direction](#blickfeld.protocol.data.Point.Direction)
    - [Point.Return](#blickfeld.protocol.data.Point.Return)
  
    - [Point.ErrorFlag](#blickfeld.protocol.data.Point.ErrorFlag)
  
  
  

- [blickfeld/data/point_cloud.proto](#blickfeld/data/point_cloud.proto)
    - [PointCloud](#blickfeld.protocol.data.PointCloud)
    - [PointCloud.Header](#blickfeld.protocol.data.PointCloud.Header)
  
  
  
  

- [blickfeld/data/scanline.proto](#blickfeld/data/scanline.proto)
    - [Scanline](#blickfeld.protocol.data.Scanline)
  
  
  
  

- [blickfeld/file/general.proto](#blickfeld/file/general.proto)
    - [Client](#blickfeld.protocol.file.Client)
  
    - [Client.Language](#blickfeld.protocol.file.Client.Language)
  
  
  

- [blickfeld/file/point_cloud.proto](#blickfeld/file/point_cloud.proto)
    - [PointCloud](#blickfeld.protocol.file.PointCloud)
    - [PointCloud.Data](#blickfeld.protocol.file.PointCloud.Data)
    - [PointCloud.Footer](#blickfeld.protocol.file.PointCloud.Footer)
    - [PointCloud.Footer.Event](#blickfeld.protocol.file.PointCloud.Footer.Event)
    - [PointCloud.Footer.Stats](#blickfeld.protocol.file.PointCloud.Footer.Stats)
    - [PointCloud.Footer.Stats.Counter](#blickfeld.protocol.file.PointCloud.Footer.Stats.Counter)
    - [PointCloud.Header](#blickfeld.protocol.file.PointCloud.Header)
    - [PointCloud.Metadata](#blickfeld.protocol.file.PointCloud.Metadata)
  
  
  
  

- [blickfeld/status/main.proto](#blickfeld/status/main.proto)
    - [Status](#blickfeld.protocol.Status)
  
  
  
  

- [blickfeld/status/scanner.proto](#blickfeld/status/scanner.proto)
    - [Scanner](#blickfeld.protocol.status.Scanner)
  
    - [Scanner.State](#blickfeld.protocol.status.Scanner.State)
  
  
  

- [blickfeld/status/temperature.proto](#blickfeld/status/temperature.proto)
    - [Temperature](#blickfeld.protocol.status.Temperature)
  
    - [Temperature.Sensor](#blickfeld.protocol.status.Temperature.Sensor)
  
  
  

- [blickfeld/stream/connection.proto](#blickfeld/stream/connection.proto)
    - [Event](#blickfeld.protocol.stream.Event)
    - [Event.Developer](#blickfeld.protocol.stream.Event.Developer)
    - [Subscribe](#blickfeld.protocol.stream.Subscribe)
    - [Subscribe.Developer](#blickfeld.protocol.stream.Subscribe.Developer)
    - [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud)
    - [Subscribe.PointCloud.Filter](#blickfeld.protocol.stream.Subscribe.PointCloud.Filter)
    - [Subscribe.Status](#blickfeld.protocol.stream.Subscribe.Status)
  
  
  
  

- [blickfeld/update/hardware.proto](#blickfeld/update/hardware.proto)
    - [partial_module_eeprom_msg](#blickfeld.protocol.update.partial_module_eeprom_msg)
    - [partial_trenz_eeprom_msg](#blickfeld.protocol.update.partial_trenz_eeprom_msg)
  
    - [HardwareVariant](#blickfeld.protocol.update.HardwareVariant)
  
  
  

- [blickfeld/update/manifest.proto](#blickfeld/update/manifest.proto)
    - [manifest_msg](#blickfeld.protocol.update.manifest_msg)
    - [manifest_msg.CompatibleHardware](#blickfeld.protocol.update.manifest_msg.CompatibleHardware)
  
    - [manifest_msg.ImageType](#blickfeld.protocol.update.manifest_msg.ImageType)
  
  
  

- [Scalar Value Types](#scalar-value-types)



<a name="blickfeld/common.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/common.proto



<a name="blickfeld.protocol.OptionalValueRange"></a>

### OptionalValueRange



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| minimum | [float](#float) | optional |  |
| maximum | [float](#float) | optional |  |






<a name="blickfeld.protocol.ValueRange"></a>

### ValueRange



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| minimum | [float](#float) | optional |  |
| maximum | [float](#float) | optional |  |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/connection.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/connection.proto



<a name="blickfeld.protocol.Request"></a>

### Request
This section describes the different requests a client can send to the server.
A request is always answered with a response. For every response, there is a request with the same name.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| hello | [Request.Hello](#blickfeld.protocol.Request.Hello) | optional | Refer to [Request.Hello](#blickfeld.protocol.Request.Hello) |
| developer | [Request.Developer](#blickfeld.protocol.Request.Developer) | optional | Refer to [Request.Developer](#blickfeld.protocol.Request.Developer) |
| set_scan_pattern | [Request.SetScanPattern](#blickfeld.protocol.Request.SetScanPattern) | optional | Refer to [Request.SetScanPattern](#blickfeld.protocol.Request.SetScanPattern) |
| fill_scan_pattern | [Request.FillScanPattern](#blickfeld.protocol.Request.FillScanPattern) | optional | Refer to [Request.FillScanPattern](#blickfeld.protocol.Request.FillScanPattern) |
| get_scan_pattern | [Request.GetScanPattern](#blickfeld.protocol.Request.GetScanPattern) | optional | Refer to [Request.GetScanPattern](#blickfeld.protocol.Request.GetScanPattern) |
| subscribe | [stream.Subscribe](#blickfeld.protocol.stream.Subscribe) | optional | Refer to [Subscribe](#blickfeld.protocol.stream.Subscribe) |
| status | [Request.Status](#blickfeld.protocol.Request.Status) | optional | Refer to [Request.Status](#blickfeld.protocol.Request.Status) |
| run_self_test | [Request.RunSelfTest](#blickfeld.protocol.Request.RunSelfTest) | optional | Refer to [Request.RunSelfTest](#blickfeld.protocol.Request.RunSelfTest) |
| _asJSON | [string](#string) | optional | Internal use only |
| accept_format | [Format](#blickfeld.protocol.Format) | optional | Internal use only Default: PROTOBUF |






<a name="blickfeld.protocol.Request.Developer"></a>

### Request.Developer
Internal use only






<a name="blickfeld.protocol.Request.FillScanPattern"></a>

### Request.FillScanPattern
This request is used to fill an empty or partially filled scan pattern with the required parameters.
It will fill the given scan pattern with default values and return the filled scan pattern to the client.
The filled scan pattern can then be set as input for  [Request.SetScanPattern](#blickfeld.protocol.Request.SetScanPattern).


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| legacy_config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Deprecated |






<a name="blickfeld.protocol.Request.GetScanPattern"></a>

### Request.GetScanPattern
This request is used to retrieve the currently set [ScanPattern](#blickfeld.protocol.config.ScanPattern).






<a name="blickfeld.protocol.Request.Hello"></a>

### Request.Hello
This request is used for initial communication after connecting to a device and testing the established connection.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| protocol_version | [uint32](#uint32) | optional | Version of the Blickfeld protocol |






<a name="blickfeld.protocol.Request.RunSelfTest"></a>

### Request.RunSelfTest
> Introduced in BSL v2.9 and firmware v1.9

This request triggers a self test of the device.
It validates the hardware, operates the device, and generates a report.
The report is currently only accessible by developers.






<a name="blickfeld.protocol.Request.SetScanPattern"></a>

### Request.SetScanPattern
This request is used for configuring a Scan Pattern.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| persist | [bool](#bool) | optional | Persists with the scan pattern and sets it after a power cycle. Default: False Default: false |
| legacy_config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Deprecated old 'config' due to horizontal field of view definition |






<a name="blickfeld.protocol.Request.Status"></a>

### Request.Status
This request is used for receiving the current [Status](#blickfeld.protocol.status.Status).






<a name="blickfeld.protocol.Response"></a>

### Response
This section describes the responses a client receives after sending a request to the server.
For the request that corresponds to a given response, please refer to [Request](#blickfeld.protocol.Request).
Each response has the same name as the request.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| timestamp_ns | [uint64](#uint64) | optional | Unit: [ns] - timestamp when the response is sent out |
| error | [Error](#blickfeld.protocol.Error) | optional | Refer to [Error](#blickfeld.protocol.Error) |
| hello | [Response.Hello](#blickfeld.protocol.Response.Hello) | optional | Refer to [Response.Hello](#blickfeld.protocol.Response.Hello) |
| developer | [Response.Developer](#blickfeld.protocol.Response.Developer) | optional | Refer to [Response.Developer](#blickfeld.protocol.Response.Developer) |
| set_scan_pattern | [Response.SetScanPattern](#blickfeld.protocol.Response.SetScanPattern) | optional | Refer to [Response.SetScanPattern](#blickfeld.protocol.Response.SetScanPattern) |
| fill_scan_pattern | [Response.FillScanPattern](#blickfeld.protocol.Response.FillScanPattern) | optional | Refer to [Response.FillScanPattern](#blickfeld.protocol.Response.FillScanPattern) |
| get_scan_pattern | [Response.GetScanPattern](#blickfeld.protocol.Response.GetScanPattern) | optional | Refer to [Response.GetScanPattern](#blickfeld.protocol.Response.GetScanPattern) |
| event | [stream.Event](#blickfeld.protocol.stream.Event) | optional | Refer to [Event](#blickfeld.protocol.stream.Event) |
| status | [Status](#blickfeld.protocol.Status) | optional | Refer to [Response.Status](#blickfeld.protocol.Response.Status) |
| run_self_test | [Response.RunSelfTest](#blickfeld.protocol.Response.RunSelfTest) | optional | Refer to [Response.RunSelfTest](#blickfeld.protocol.Response.RunSelfTest) |
| _asJSON | [string](#string) | optional | Internal use only |






<a name="blickfeld.protocol.Response.Developer"></a>

### Response.Developer
Internal use only






<a name="blickfeld.protocol.Response.FillScanPattern"></a>

### Response.FillScanPattern
This response is given after a request to fill a sparse [ScanPattern](#blickfeld.protocol.config.ScanPattern) has been transmitted.
It returns a scan pattern, the unset fields are filled with default values.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| legacy_config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Deprecated |






<a name="blickfeld.protocol.Response.GetScanPattern"></a>

### Response.GetScanPattern
This response is returned after a request to get the current [ScanPattern](#blickfeld.protocol.config.ScanPattern).


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| legacy_config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Deprecated |






<a name="blickfeld.protocol.Response.Hello"></a>

### Response.Hello
This response is sent out after establishing a connection.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| protocol_version | [uint32](#uint32) | optional |  |






<a name="blickfeld.protocol.Response.RunSelfTest"></a>

### Response.RunSelfTest
> Introduced in BSL v2.9 and firmware v1.9

This response is returned after a self test.
The success flag indicates if the hardware is fully operational.
The generated report is currently only accessible by developers.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| success | [bool](#bool) | optional |  |






<a name="blickfeld.protocol.Response.SetScanPattern"></a>

### Response.SetScanPattern
This response is sent out after setting a scan pattern.





 <!-- end messages -->


<a name="blickfeld.protocol.Format"></a>

### Format
The format type of response is an undocumented, internal feature required for the REST API.

| Name | Number | Description |
| ---- | ------ | ----------- |
| PROTOBUF | 1 |  |
| JSON | 2 |  |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/error.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/error.proto



<a name="blickfeld.protocol.Error"></a>

### Error



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| unknown | [Error.Unknown](#blickfeld.protocol.Error.Unknown) | optional | Refer to [Unknown](#blickfeld.protocol.Error.Unknown) |
| not_implemented | [Error.NotImplemented](#blickfeld.protocol.Error.NotImplemented) | optional | Refer to [NotImplemented](#blickfeld.protocol.Error.NotImplemented) |
| empty | [Error.Empty](#blickfeld.protocol.Error.Empty) | optional | Refer to [Empty](#blickfeld.protocol.Error.Empty) |
| server_implementation | [Error.ServerImplementation](#blickfeld.protocol.Error.ServerImplementation) | optional | Refer to [ServerImplementation](#blickfeld.protocol.Error.ServerImplementation) |
| invalid_request | [Error.InvalidRequest](#blickfeld.protocol.Error.InvalidRequest) | optional | Refer to [InvalidRequest](#blickfeld.protocol.Error.InvalidRequest) |
| connection_closed | [Error.ConnectionClosed](#blickfeld.protocol.Error.ConnectionClosed) | optional | Refer to [ConnectionClosed](#blickfeld.protocol.Error.ConnectionClosed) |
| outdated_server_protocol | [Error.OutdatedServerProtocol](#blickfeld.protocol.Error.OutdatedServerProtocol) | optional | Refer to [OutdatedServerProtocol](#blickfeld.protocol.Error.OutdatedServerProtocol) |
| outdated_client_protocol | [Error.OutdatedClientProtocol](#blickfeld.protocol.Error.OutdatedClientProtocol) | optional | Refer to [OutdatedClientProtocol](#blickfeld.protocol.Error.OutdatedClientProtocol) |
| scanner_busy | [Error.ScannerBusy](#blickfeld.protocol.Error.ScannerBusy) | optional | Refer to [ScannerBusy](#blickfeld.protocol.Error.ScannerBusy) |
| wrong_operation_mode | [Error.WrongOperationMode](#blickfeld.protocol.Error.WrongOperationMode) | optional | Refer to [WrongOperationMode](#blickfeld.protocol.Error.WrongOperationMode) |
| not_allowed | [Error.NotAllowed](#blickfeld.protocol.Error.NotAllowed) | optional | Refer to [NotAllowed](#blickfeld.protocol.Error.NotAllowed) |
| hardware_error | [Error.HardwareError](#blickfeld.protocol.Error.HardwareError) | optional | Refer to [HardwareError](#blickfeld.protocol.Error.HardwareError) |
| system_stop | [Error.SystemStop](#blickfeld.protocol.Error.SystemStop) | optional | Refer to [SystemStop](#blickfeld.protocol.Error.SystemStop) |
| not_found | [Error.NotFound](#blickfeld.protocol.Error.NotFound) | optional | Refer to [NotFound](#blickfeld.protocol.Error.NotFound) |
| unknown_error_code | [Error.UnknownErrorCode](#blickfeld.protocol.Error.UnknownErrorCode) | optional | Refer to [UnknownErrorCode](#blickfeld.protocol.Error.UnknownErrorCode) |
| not_in_range | [Error.NotInRange](#blickfeld.protocol.Error.NotInRange) | optional | Refer to [NotInRange](#blickfeld.protocol.Error.NotInRange) |
| time_sync_failed | [Error.TimeSyncFailed](#blickfeld.protocol.Error.TimeSyncFailed) | optional | Refer to [TimeSyncFailed](#blickfeld.protocol.Error.TimeSyncFailed) |
| no_device_discovered | [Error.NoDeviceDiscovered](#blickfeld.protocol.Error.NoDeviceDiscovered) | optional | Refer to [NoDeviceDiscovered](#blickfeld.protocol.Error.NoDeviceDiscovered) |






<a name="blickfeld.protocol.Error.ConnectionClosed"></a>

### Error.ConnectionClosed
The server closed the connection.






<a name="blickfeld.protocol.Error.Empty"></a>

### Error.Empty
The server sent an empty message.






<a name="blickfeld.protocol.Error.HardwareError"></a>

### Error.HardwareError
The device is in an error state and cannot be operated. Attempt to power-cycle the device or use diagnostic software to read out the hardware state.






<a name="blickfeld.protocol.Error.InvalidRequest"></a>

### Error.InvalidRequest
Validation of the sent request failed, please send a request with-in a valid range.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| validation_error | [string](#string) | optional | Validation error string |






<a name="blickfeld.protocol.Error.NoDeviceDiscovered"></a>

### Error.NoDeviceDiscovered
No device has been discovered. Check the network connection and power supply of the device. The 'discover' requests may have been blocked by a local or a network firewall.






<a name="blickfeld.protocol.Error.NotAllowed"></a>

### Error.NotAllowed
The requested operation is not allowed.






<a name="blickfeld.protocol.Error.NotFound"></a>

### Error.NotFound
The requested data is not available. Please check the request parameters.






<a name="blickfeld.protocol.Error.NotImplemented"></a>

### Error.NotImplemented
The firmware on the device is not compatible with the BSL version. Update the device or downgrade the BSL.






<a name="blickfeld.protocol.Error.NotInRange"></a>

### Error.NotInRange
The requested parameter is not within the valid range.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| parameter | [string](#string) | optional | The parameter, which was not in range |
| minimum | [float](#float) | optional | Minimum value of the parameter |
| maximum | [float](#float) | optional | Maximum value of the parameter |
| requested | [float](#float) | optional | Requested value of the parameter |
| unit | [string](#string) | optional | Unit of the parameter |






<a name="blickfeld.protocol.Error.OutdatedClientProtocol"></a>

### Error.OutdatedClientProtocol
The protocol of the server is too new. Please update the client software.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| required_version | [uint32](#uint32) | optional | Version that is required |






<a name="blickfeld.protocol.Error.OutdatedServerProtocol"></a>

### Error.OutdatedServerProtocol
The protocol of the client is too new. Please update the device.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| required_version | [uint32](#uint32) | optional | Version that is required |






<a name="blickfeld.protocol.Error.ScannerBusy"></a>

### Error.ScannerBusy
The device is currently busy with another operation. Cancel it and try again.






<a name="blickfeld.protocol.Error.ServerImplementation"></a>

### Error.ServerImplementation
The server failed to process the request.






<a name="blickfeld.protocol.Error.SystemStop"></a>

### Error.SystemStop
The device stopped unexpectedly and can no longer be operated. Attempt to power cycle the device or use diagnostic software to read-out the hardware state.






<a name="blickfeld.protocol.Error.TimeSyncFailed"></a>

### Error.TimeSyncFailed
The time synchronization failed. The NTP daemon failed or timed out.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| ntp_daemon_log | [string](#string) | optional | Log of the NTP daemon |






<a name="blickfeld.protocol.Error.Unknown"></a>

### Error.Unknown
Unknown error. Please consult the Blickfeld support team for further information.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| description | [string](#string) | optional | Description of the unknown error |






<a name="blickfeld.protocol.Error.UnknownErrorCode"></a>

### Error.UnknownErrorCode
Unknown Error. Please contact the Blickfeld support team and make sure to use compatible BSL and FW versions.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| error_code | [uint32](#uint32) | optional | Error code |






<a name="blickfeld.protocol.Error.WrongOperationMode"></a>

### Error.WrongOperationMode
The device is set to a different operation mode. Perhaps it is currently being used by another application.
Change the operation mode and try again if no one else is using it.





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/options.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/options.proto


 <!-- end messages -->

 <!-- end enums -->


<a name="blickfeld/options.proto-extensions"></a>

### File-level Extensions
| Extension | Type | Base | Number | Description |
| --------- | ---- | ---- | ------ | ----------- |
| allow_sparse | bool | .google.protobuf.FieldOptions | 50007 |  Default: `false` |
| d_max | double | .google.protobuf.FieldOptions | 50001 |  Default: `inf` |
| d_min | double | .google.protobuf.FieldOptions | 50000 |  Default: `-inf` |
| i_max | uint64 | .google.protobuf.FieldOptions | 50003 |  Default: `18446744073709551615` |
| i_min | sint64 | .google.protobuf.FieldOptions | 50002 |  Default: `-9223372036854775808` |
| legacy_field_id | uint64 | .google.protobuf.FieldOptions | 50010 |  |
| length | sint32 | .google.protobuf.FieldOptions | 50004 |  Default: `2147483647` |
| max_length | sint32 | .google.protobuf.FieldOptions | 50009 |  Default: `2147483647` |
| min_length | sint32 | .google.protobuf.FieldOptions | 50008 |  Default: `0` |
| optional | bool | .google.protobuf.FieldOptions | 50006 |  Default: `false` |
| regex | string | .google.protobuf.FieldOptions | 50005 |  Default: `.*` |
| e_desc | string | .google.protobuf.MessageOptions | 60000 | Error description Default: `No additional error description available.` |
| help | string | .google.protobuf.MessageOptions | 60001 | Help description |
| secure | config.Secure | .google.protobuf.MessageOptions | 60002 |  |
| optional_one_of | bool | .google.protobuf.OneofOptions | 50006 |  Default: `false` |

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/config/scan_pattern.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/config/scan_pattern.proto



<a name="blickfeld.protocol.config.ScanPattern"></a>

### ScanPattern
The scan pattern defines the movement of the mirrors.
The key parameters of the pattern are the horizontal and vertical fields of view (FoV) as well as the number of scan lines per frame.
The frame rate is defined by the total number of scan lines and the oscillation frequency of the mirrors which is fixed and device-spcific.

For a more detailed explanation, see: [Scan Pattern documentation](Scan_Pattern)


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| horizontal | [ScanPattern.Horizontal](#blickfeld.protocol.config.ScanPattern.Horizontal) | optional | Refer to [ScanPattern.Horizontal](#blickfeld.protocol.config.ScanPattern.Horizontal) |
| vertical | [ScanPattern.Vertical](#blickfeld.protocol.config.ScanPattern.Vertical) | optional | Refer to [ScanPattern.Vertical](#blickfeld.protocol.config.ScanPattern.Vertical) |
| pulse | [ScanPattern.Pulse](#blickfeld.protocol.config.ScanPattern.Pulse) | optional | Refer to [ScanPattern.Pulse](#blickfeld.protocol.config.ScanPattern.Pulse) |
| frame_rate | [ScanPattern.FrameRate](#blickfeld.protocol.config.ScanPattern.FrameRate) | optional | Refer to [ScanPattern.FrameRate](#blickfeld.protocol.config.ScanPattern.FrameRate) |






<a name="blickfeld.protocol.config.ScanPattern.FrameRate"></a>

### ScanPattern.FrameRate
This section defines the target frame rate for synchronization between several LiDAR devices.
The feature is described in the [Time synchronization documentation](Sync).


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| target | [double](#double) | optional | Target frame rate during synchronization between several LiDAR devices |
| maximum | [double](#double) | optional | Read-only parameter for maximum achievable frame rate. |
| reference_time_offset | [double](#double) | optional | In [s]. Adds a constant value to the Unix reference time, starting from 00:00:00 UTC on January 1, 1970. It can be used to compensate delays or to interleave LiDAR devices. Default: 0 Default: 0 |






<a name="blickfeld.protocol.config.ScanPattern.Horizontal"></a>

### ScanPattern.Horizontal
This section defines the movement of the horizontal mirror.
The mirror moves continuously with its own frequency. Each half-oscillation period of the mirror results in one scan line.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| fov | [float](#float) | optional | Unit: [rad] – optical field of view of the horizontal mirror. Note: In v2.2* and below, this was defined as a mechanical FoV. |






<a name="blickfeld.protocol.config.ScanPattern.Pulse"></a>

### ScanPattern.Pulse
This section defines the pattern in which the laser pulses and captures sample points


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| angle_spacing | [float](#float) | optional | Unit: [rad] – this defines the angle within which the horizontal mirror moves between two laser pulses. This parameter therefore defines the horizontal sampling resolution. |
| type | [ScanPattern.Pulse.Type](#blickfeld.protocol.config.ScanPattern.Pulse.Type) | optional | Refer to [ScanPattern.Pulse.Type](#blickfeld.protocol.config.ScanPattern.Pulse.Type) Default: EQUI_HORIZONTAL_ANGLE |
| frame_mode | [ScanPattern.Pulse.FrameMode](#blickfeld.protocol.config.ScanPattern.Pulse.FrameMode) | optional | Refer to [ScanPattern.Pulse.FrameMode](#blickfeld.protocol.config.ScanPattern.Pulse.FrameMode) Default: ONLY_UP |
| distortion_correction | [bool](#bool) | optional | This parameter defines whether the distortion correction is applied when calculating the laser pulse angles. Default: True Default: true |






<a name="blickfeld.protocol.config.ScanPattern.Vertical"></a>

### ScanPattern.Vertical
This section defines the movement of the vertical mirror within one frame.
The mirror moves for from 0 degrees to the configured target FoV (up-ramping phase) and back to 0 degrees (down-ramping phase) with its eigenfrequency.
The number of scanlines for each phase can be configured.
Due to the dynamics of the mirror, a certain number of scan lines are required to reach the target FoV and then to reach 0 degrees again.
Consequently, if the limit is reached, a lower number of scan lines requires a reduced FoV.
Thus, a higher number of scan lines allows a larger vertical FoV. The vertical FoV is limited by the optical components.

Note: Due to the time required for one scan line, the rounded even number of scan lines directly scales inverse proportionally with65t the frame rate.

For a more detailed explanation, see: [Scan Pattern documentation](Scan_Pattern)


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| fov | [float](#float) | optional | Unit: [rad] – FoV in the center of the frame. Due to the eye shape of the scan pattern, the vertical FoV decreases the outer boundaries of the horizontal mirror. |
| scanlines_up | [uint32](#uint32) | optional | Configures the number of scan lines required for the up-ramping phase. During the up-ramping phase, the vertical mirror increases its amplitude from 0 degrees to the target FoV. Default: 200 Default: 200 |
| scanlines_down | [uint32](#uint32) | optional | Configures the amount of scan lines required for the down-ramping phase. During the down-ramping phase, the vertical mirror decreases its amplitude from the target FoV to 0 degrees. Default: 30 Default: 30 |





 <!-- end messages -->


<a name="blickfeld.protocol.config.ScanPattern.Pulse.FrameMode"></a>

### ScanPattern.Pulse.FrameMode
Because of the mirrors oscillating and the resulting Lissajous curve, it is not possible to pulse in a completely arbitrary pattern.
A frame always starts with a phase in which the vertical mirror increases its amplitude (up-ramping).
It is followed by a second phase, in which it decreases its amplitude until it no longer moves (down-ramping).
It is possible to pulse in both phases. There are four different options to pulse:

| Name | Number | Description |
| ---- | ------ | ----------- |
| NONE | 0 |  |
| COMBINE_UP_DOWN | 1 | The device pulses in the up-ramping and down-ramping phases and one frame is created by combining the resulting sample points. |
| ONLY_UP | 2 | The device pulses in the **up-ramping** phase and **does not** pulse in the **down-ramping** phase. |
| ONLY_DOWN | 3 | The device is **does not** pulse in the **up-ramping** phase and pulses in the **down-ramping** phase. |
| SEPARATE | 4 | The device pulses in the up-ramping and down-ramping phases and one frame for each phase is created. |



<a name="blickfeld.protocol.config.ScanPattern.Pulse.Type"></a>

### ScanPattern.Pulse.Type
The device pulses depending on the mirror positioning.
Currently, only pulse triggering on the horizontal angle is supported.
The laser pulses are emittet with the configured angular spacing.

| Name | Number | Description |
| ---- | ------ | ----------- |
| EQUI_HORIZONTAL_ANGLE | 0 | The laser is triggered depending on the horizontal angle of the laser beam. |
| EQUI_DENSITY | 1 | Reserved for future use. Not yet supported. |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/config/secure.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/config/secure.proto



<a name="blickfeld.protocol.config.Secure"></a>

### Secure



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| permissions | [Secure.Permissions](#blickfeld.protocol.config.Secure.Permissions) | repeated |  |
| cert_type | [Secure.CertType](#blickfeld.protocol.config.Secure.CertType) | optional |  |
| version_major | [uint32](#uint32) | optional |  |
| version_minor | [uint32](#uint32) | optional |  |
| version_patch | [uint32](#uint32) | optional |  |





 <!-- end messages -->


<a name="blickfeld.protocol.config.Secure.CertType"></a>

### Secure.CertType


| Name | Number | Description |
| ---- | ------ | ----------- |
| CLIENT | 0 |  |
| CUBE | 1 |  |



<a name="blickfeld.protocol.config.Secure.Permissions"></a>

### Secure.Permissions
do NOT change enum labels, they are used as strings in ssl certs

| Name | Number | Description |
| ---- | ------ | ----------- |
| PUBLIC | 0 |  |
| DEVELOPER | 1 |  |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/data/frame.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/data/frame.proto



<a name="blickfeld.protocol.data.Frame"></a>

### Frame
This section describes the contents of a point cloud frame.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| id | [uint64](#uint64) | optional | Incremental frame ID since startup of the device |
| scanlines | [Scanline](#blickfeld.protocol.data.Scanline) | repeated | Refer to [Scanline](#blickfeld.protocol.data.Scanline) |
| start_time_ns | [uint64](#uint64) | optional | Unit: [s] – start frame timestamp |
| scan_pattern | [blickfeld.protocol.config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| total_number_of_points | [uint32](#uint32) | optional | Number of laser pulses emitted in this frame. |
| total_number_of_returns | [uint32](#uint32) | optional | Number of returned points recorded:

Each point of the [total_number_of_points](#blickfeld.protocol.data.Frame.total_number_of_points) can produce several returns. |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/data/point.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/data/point.proto



<a name="blickfeld.protocol.data.Point"></a>

### Point
This section describes the contents of a single point of a point cloud frame or scan line.
Each point resembles a direction in which the laser has fired.
From each point (direction) the device can receive several responses/returns, see [Point.Return](#blickfeld.protocol.data.Point.Return).


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| id | [uint32](#uint32) | optional | Point ID in the corresponding frame |
| returns | [Point.Return](#blickfeld.protocol.data.Point.Return) | repeated | Refer to [Point.Return](#blickfeld.protocol.data.Point.Return) |
| start_offset_ns | [uint64](#uint64) | optional | Unit: [ns] - starting time of the point in relation to [Frame.start_time_ns](#blickfeld.protocol.data.Frame.start_time_ns) |
| ambient_light_level | [uint32](#uint32) | optional | Ambient light level in the direction of the point |
| direction | [Point.Direction](#blickfeld.protocol.data.Point.Direction) | optional | Refer to [Point.Direction](#blickfeld.protocol.data.Point.Direction) |
| channel_id | [uint32](#uint32) | optional | ID of the channel that detected the point |
| error_flags | [Point.ErrorFlag](#blickfeld.protocol.data.Point.ErrorFlag) | repeated | Refer to [Point.ErrorFlag](#blickfeld.protocol.data.Point.ErrorFlag) |






<a name="blickfeld.protocol.data.Point.Direction"></a>

### Point.Direction
This section describes the direction and the origin of a point.
Use these polar coordinates combined with the range information of a given return, if you want to locate the reflection in the spherical coordinatesystem.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| azimuth | [float](#float) | optional | Unit: [rad] - azimuth angle: Angle with respect to y-z plane |
| elevation | [float](#float) | optional | Unit: [rad] - elevation angle: Angle with respect to x-y plane |
| origin | [float](#float) | repeated | Unit: [m] - origin of the laser beam [x,y,z] |






<a name="blickfeld.protocol.data.Point.Return"></a>

### Point.Return
This section describes the contents of a single return of a point, sorted by intensity.
A return is created when the LiDAR detects the laser light of the reflection of an object.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| id | [uint32](#uint32) | optional | ID of the return in the point |
| cartesian | [float](#float) | repeated | Unit: [m] – Cartesian coordinates of the target [x,y,z] |
| range | [float](#float) | optional | Unit: [m] – distance of target to the origin |
| intensity | [uint32](#uint32) | optional | Intensity of the returned laser pulse |





 <!-- end messages -->


<a name="blickfeld.protocol.data.Point.ErrorFlag"></a>

### Point.ErrorFlag
Error flag indicating why a point delivers no valid returns.
If a flag is set, the point should not be interpreted neither processed.
Flags are only set temporarily for a short period of time.
If the error state does not recover, the device will stop operation.

| Name | Number | Description |
| ---- | ------ | ----------- |
| E_VIBRATION_THRESHOLD_EXCEEDED | 1 | The environmental vibration threshold was exceeded. This should not happen in normal operation. |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/data/point_cloud.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/data/point_cloud.proto



<a name="blickfeld.protocol.data.PointCloud"></a>

### PointCloud

A point cloud object can contain either a full frame or a single scan line.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| header | [PointCloud.Header](#blickfeld.protocol.data.PointCloud.Header) | optional | Refer to [PointCloud.Header](#blickfeld.protocol.data.PointCloud.Header) |
| frame | [Frame](#blickfeld.protocol.data.Frame) | optional | Refer to [Frame](#blickfeld.protocol.data.Frame) |
| scanline | [Scanline](#blickfeld.protocol.data.Scanline) | optional | Refer to [Scanline](#blickfeld.protocol.data.Scanline) |






<a name="blickfeld.protocol.data.PointCloud.Header"></a>

### PointCloud.Header

This section describes the contents of a point cloud header.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| legacy_cube_serial_number | [string](#string) | optional | Legacy serial number of the device which recorded the pointcloud |
| cube_serial_number | [string](#string) | optional | Serial number of the device which recorded the pointcloud |
| start_time_ns | [uint64](#uint64) | optional | Unit: [s] - Start timestamp of the requested pointcloud stream |
| firmware_version | [string](#string) | optional | Firmware version of the device which recorded the pointcloud |
| hardware_variant | [blickfeld.protocol.update.HardwareVariant](#blickfeld.protocol.update.HardwareVariant) | optional | Hardware variant of the device which recorded the pointcloud |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/data/scanline.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/data/scanline.proto



<a name="blickfeld.protocol.data.Scanline"></a>

### Scanline
This section describes the contents of a single scan line in a point cloud frame.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| id | [uint64](#uint64) | optional | Scan line ID in the corresponding frame |
| frame_id | [uint64](#uint64) | optional | Frame ID of the corresponding frame |
| points | [Point](#blickfeld.protocol.data.Point) | repeated | Refer to [Point](#blickfeld.protocol.data.Point) |
| start_offset_ns | [uint64](#uint64) | optional | Unit: [ns] - start time of the scanline in relation to [Frame.start_time_ns](#blickfeld.protocol.data.Frame.start_time_ns) |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/file/general.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/file/general.proto



<a name="blickfeld.protocol.file.Client"></a>

### Client



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| library_version | [string](#string) | optional | Library version |
| file_time_ns | [uint64](#uint64) | optional | Unit: [ns] - File time stamp |
| language | [Client.Language](#blickfeld.protocol.file.Client.Language) | optional | Used library language for file creation |





 <!-- end messages -->


<a name="blickfeld.protocol.file.Client.Language"></a>

### Client.Language


| Name | Number | Description |
| ---- | ------ | ----------- |
| CPP | 1 |  |
| PYTHON | 2 |  |
| TYPESCRIPT | 3 |  |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/file/point_cloud.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/file/point_cloud.proto



<a name="blickfeld.protocol.file.PointCloud"></a>

### PointCloud
This section describes the contents of a point cloud. The first message in a Blickfeld protobuf pointcloud should always be the 
[PointCloud.Header](#blickfeld.protocol.data.PointCloud.Header) message followed by [PointCloud.Data](#blickfeld.protocol.data.PointCloud.Data) messages.






<a name="blickfeld.protocol.file.PointCloud.Data"></a>

### PointCloud.Data



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| frame | [blickfeld.protocol.data.Frame](#blickfeld.protocol.data.Frame) | optional | Refer to [PointCloud](#blickfeld.protocol.data.Frame) |
| footer | [PointCloud.Footer](#blickfeld.protocol.file.PointCloud.Footer) | optional |  |






<a name="blickfeld.protocol.file.PointCloud.Footer"></a>

### PointCloud.Footer



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| stats | [PointCloud.Footer.Stats](#blickfeld.protocol.file.PointCloud.Footer.Stats) | optional |  |
| events | [PointCloud.Footer.Event](#blickfeld.protocol.file.PointCloud.Footer.Event) | repeated |  |
| stop_time_ns | [uint64](#uint64) | optional | Unit: [ns] - Stop timestamp of the requested pointcloud stream |






<a name="blickfeld.protocol.file.PointCloud.Footer.Event"></a>

### PointCloud.Footer.Event



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| from_frame_id | [uint64](#uint64) | optional |  |
| scan_pattern | [blickfeld.protocol.config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional |  |






<a name="blickfeld.protocol.file.PointCloud.Footer.Stats"></a>

### PointCloud.Footer.Stats



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| counter | [PointCloud.Footer.Stats.Counter](#blickfeld.protocol.file.PointCloud.Footer.Stats.Counter) | optional |  |






<a name="blickfeld.protocol.file.PointCloud.Footer.Stats.Counter"></a>

### PointCloud.Footer.Stats.Counter



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| frames | [uint64](#uint64) | optional |  |
| points | [uint64](#uint64) | optional |  |
| returns | [uint64](#uint64) | optional |  |






<a name="blickfeld.protocol.file.PointCloud.Header"></a>

### PointCloud.Header



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| device | [blickfeld.protocol.data.PointCloud.Header](#blickfeld.protocol.data.PointCloud.Header) | optional | Refer to [PointCloud.Header](#blickfeld.protocol.data.PointCloud.Header) |
| client | [Client](#blickfeld.protocol.file.Client) | optional |  |






<a name="blickfeld.protocol.file.PointCloud.Metadata"></a>

### PointCloud.Metadata



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| header | [PointCloud.Header](#blickfeld.protocol.file.PointCloud.Header) | optional |  |
| footer | [PointCloud.Footer](#blickfeld.protocol.file.PointCloud.Footer) | optional |  |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/status/main.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/status/main.proto



<a name="blickfeld.protocol.Status"></a>

### Status
This section contains the status messages of the two deflection mirrors and the temperature sensors in the device.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| scanner | [status.Scanner](#blickfeld.protocol.status.Scanner) | optional | Refer to [Scanner](#blickfeld.protocol.status.scanner.Scanner) |
| temperatures | [status.Temperature](#blickfeld.protocol.status.Temperature) | repeated | Refer to [Temperature](#blickfeld.protocol.status.temperature.Temperature) |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/status/scanner.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/status/scanner.proto



<a name="blickfeld.protocol.status.Scanner"></a>

### Scanner
This section defines the status of the device.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| state | [Scanner.State](#blickfeld.protocol.status.Scanner.State) | optional | Refer to [Scanner.State](#blickfeld.protocol.status.Scanner.State) |
| scan_pattern | [blickfeld.protocol.config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| error | [blickfeld.protocol.Error](#blickfeld.protocol.Error) | optional | Refer to [Error](#blickfeld.protocol.Error) |
| legacy_scan_pattern | [blickfeld.protocol.config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Deprecated old ´scan_pattern´ definition |





 <!-- end messages -->


<a name="blickfeld.protocol.status.Scanner.State"></a>

### Scanner.State


| Name | Number | Description |
| ---- | ------ | ----------- |
| INITIALIZING | 1 | Device is initializing the hardware. |
| READY | 2 | Device is ready to start and no error occurred. |
| STARTING | 3 | Device is starting a point cloud recording, but is not yet recording. |
| RUNNING | 4 | Device is recording a point cloud. |
| STOPPING | 5 | Device stops point cloud recording; it is no longer recording. |
| ERRORED | 6 | Device is in an error state; it can no longer operate. |
| SELF_TESTING | 7 | Device is testing the hardware. |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/status/temperature.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/status/temperature.proto



<a name="blickfeld.protocol.status.Temperature"></a>

### Temperature
This section describes the temperature of the hardware modules in the device.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| sensor | [Temperature.Sensor](#blickfeld.protocol.status.Temperature.Sensor) | optional | Refer to [Temperature.Sensor](#blickfeld.protocol.status.Temperature.Sensor) |
| value | [float](#float) | optional | Unit: [degrees Celsius] – temperature value of the module. |
| failed_reason | [string](#string) | optional | Error reason why the temperature cannot be read out. |





 <!-- end messages -->


<a name="blickfeld.protocol.status.Temperature.Sensor"></a>

### Temperature.Sensor
This section describes the hardware modules in the device.

| Name | Number | Description |
| ---- | ------ | ----------- |
| UNKNOWN | 0 |  |
| LDM | 1 | Laser and detector module |
| ETHERNET | 2 | Ethernet adapter |
| MSB | 3 | Mixed signal board |
| PL | 4 | Programmable logic |
| PS | 5 | Processing system |
| SCANNER_SMALL | 6 | Vertical mirror |
| SCANNER_BIG | 7 | Horizontal mirror |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/stream/connection.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/stream/connection.proto



<a name="blickfeld.protocol.stream.Event"></a>

### Event
This section describes the events of streams.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| point_cloud | [blickfeld.protocol.data.PointCloud](#blickfeld.protocol.data.PointCloud) | optional | Refer to [PointCloud](#blickfeld.protocol.data.PointCloud) |
| status | [blickfeld.protocol.Status](#blickfeld.protocol.Status) | optional | Refer to [Status](#blickfeld.protocol.status.Status) |
| developer | [Event.Developer](#blickfeld.protocol.stream.Event.Developer) | optional | Refer to [Event.Developer](#blickfeld.protocol.stream.Event.Developer) |






<a name="blickfeld.protocol.stream.Event.Developer"></a>

### Event.Developer
Internal use only






<a name="blickfeld.protocol.stream.Subscribe"></a>

### Subscribe
This section describes the different streams to which it is possible to subscribe.
A stream regularly provides data or status updates for the user. The events will not be pushed automatically to the BSL; the client has to retrieve them.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| point_cloud | [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud) | optional | Refer to [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud) |
| status | [Subscribe.Status](#blickfeld.protocol.stream.Subscribe.Status) | optional | Refer to [Subscribe.Status](#blickfeld.protocol.stream.Subscribe.Status) |
| developer | [Subscribe.Developer](#blickfeld.protocol.stream.Subscribe.Developer) | optional | Refer to [Subscribe.Developer](#blickfeld.protocol.stream.Subscribe.Developer) |






<a name="blickfeld.protocol.stream.Subscribe.Developer"></a>

### Subscribe.Developer
Internal use only






<a name="blickfeld.protocol.stream.Subscribe.PointCloud"></a>

### Subscribe.PointCloud
This request is used for subscribing to a point cloud stream.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| reference_frame | [blickfeld.protocol.data.Frame](#blickfeld.protocol.data.Frame) | optional | <blockquote>Introduced in BSL v2.9 and firmware v1.9</blockquote> If present, only fields that are set in this message and submessages will be present in the point cloud. If less fields are requested, the Protobuf encoding and network transport time can reduce significantly. |
| filter | [Subscribe.PointCloud.Filter](#blickfeld.protocol.stream.Subscribe.PointCloud.Filter) | optional | <blockquote>Introduced in BSL v2.9 and firmware v1.9</blockquote> Refer to [PointCloud.Filter](#blickfeld.protocol.stream.Subscribe.PointCloud.Filter) |






<a name="blickfeld.protocol.stream.Subscribe.PointCloud.Filter"></a>

### Subscribe.PointCloud.Filter
> Introduced in BSL v2.9 and firmware v1.9

Filter points and returns by point attributes during the post-processing on the device.
This can be used to e.g. filter points with low intensity or to enable secondary returns.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| max_number_of_returns_per_point | [uint32](#uint32) | optional | Set maximum number of returns per point. By default, secondary returns are disabled. Set this, e.g. to 2, to enable secondary returns. Default: 1 |
| intensity | [blickfeld.protocol.OptionalValueRange](#blickfeld.protocol.OptionalValueRange) | optional | Filter all points, which intensity values are not within this value range. |
| ambient_light_level | [blickfeld.protocol.OptionalValueRange](#blickfeld.protocol.OptionalValueRange) | optional | Filter all points, which ambient light level values are not within this value range. |
| range | [blickfeld.protocol.OptionalValueRange](#blickfeld.protocol.OptionalValueRange) | optional | Filter all points, which range values are not within this value range. |






<a name="blickfeld.protocol.stream.Subscribe.Status"></a>

### Subscribe.Status
This request is used for subscribing to a status stream.





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/update/hardware.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/update/hardware.proto



<a name="blickfeld.protocol.update.partial_module_eeprom_msg"></a>

### partial_module_eeprom_msg
partial parse of {ldm,scanner,msb}_eeprom_msg


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| hardware_version | [string](#string) | optional |  |






<a name="blickfeld.protocol.update.partial_trenz_eeprom_msg"></a>

### partial_trenz_eeprom_msg
partial parse of trenz_eeprom_msg


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| hardware_variant | [HardwareVariant](#blickfeld.protocol.update.HardwareVariant) | optional |  |





 <!-- end messages -->


<a name="blickfeld.protocol.update.HardwareVariant"></a>

### HardwareVariant
do NOT change enum labels, they are used as strings in code

| Name | Number | Description |
| ---- | ------ | ----------- |
| ALL_HARDWARE | 5 |  |
| UNKNOWN_HARDWARE | 6 |  |
| NO_HARDWARE | 7 |  |
| CUBE_V0_2 | 0 |  |
| CUBE_V0_3 | 1 |  |
| CUBE_V1_0 | 4 |  |
| AURORA_P3 | 3 |  |
| APOLLON_A0 | 2 |  |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/update/manifest.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/update/manifest.proto



<a name="blickfeld.protocol.update.manifest_msg"></a>

### manifest_msg



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| min_bus_version | [string](#string) | optional |  |
| image_type | [manifest_msg.ImageType](#blickfeld.protocol.update.manifest_msg.ImageType) | optional |  |
| compatible_hardware | [manifest_msg.CompatibleHardware](#blickfeld.protocol.update.manifest_msg.CompatibleHardware) | optional |  |
| secure | [bool](#bool) | optional |  Default: false |






<a name="blickfeld.protocol.update.manifest_msg.CompatibleHardware"></a>

### manifest_msg.CompatibleHardware



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| variant | [HardwareVariant](#blickfeld.protocol.update.HardwareVariant) | repeated |  |
| msb | [string](#string) | repeated |  |
| scanner_big | [string](#string) | repeated |  |
| scanner_small | [string](#string) | repeated |  |
| hsd | [string](#string) | repeated |  |





 <!-- end messages -->


<a name="blickfeld.protocol.update.manifest_msg.ImageType"></a>

### manifest_msg.ImageType


| Name | Number | Description |
| ---- | ------ | ----------- |
| PARTITIONED | 0 |  |
| SYSTEM | 1 |  |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



## Scalar Value Types

| .proto Type | Notes | C++ Type | Python Type |
| ----------- | ----- | -------- | ----------- |
| <a name="double" /> double |  | double | float |
| <a name="float" /> float |  | float | float |
| <a name="int32" /> int32 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint32 instead. | int32 | int |
| <a name="int64" /> int64 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint64 instead. | int64 | int/long |
| <a name="uint32" /> uint32 | Uses variable-length encoding. | uint32 | int/long |
| <a name="uint64" /> uint64 | Uses variable-length encoding. | uint64 | int/long |
| <a name="sint32" /> sint32 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s. | int32 | int |
| <a name="sint64" /> sint64 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s. | int64 | int/long |
| <a name="fixed32" /> fixed32 | Always four bytes. More efficient than uint32 if values are often greater than 2^28. | uint32 | int |
| <a name="fixed64" /> fixed64 | Always eight bytes. More efficient than uint64 if values are often greater than 2^56. | uint64 | int/long |
| <a name="sfixed32" /> sfixed32 | Always four bytes. | int32 | int |
| <a name="sfixed64" /> sfixed64 | Always eight bytes. | int64 | int/long |
| <a name="bool" /> bool |  | bool | boolean |
| <a name="string" /> string | A string must always contain UTF-8 encoded or 7-bit ASCII text. | string | str/unicode |
| <a name="bytes" /> bytes | May contain any arbitrary sequence of bytes. | string | str |

