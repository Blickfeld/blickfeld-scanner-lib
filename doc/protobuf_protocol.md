# Protobuf definitions
<a name="top"></a>

This is the documentation of the protobuf messages used by the Blickfeld API.

These protobuf messages are used to communicate with Blickfeld devices. You can send requests and receive responses.
The corresponding response to a request has the same name.
It is also possible to request a point cloud stream or a status stream.

The data, such as a point cloud, are also packed in protobuf messages.

## Table of Contents

- [blickfeld/common.proto](#blickfeld/common.proto)
    - [Constraint](#blickfeld.protocol.Constraint)
    - [Constraint.Constant](#blickfeld.protocol.Constraint.Constant)
    - [Constraint.Polynomial](#blickfeld.protocol.Constraint.Polynomial)
    - [Field](#blickfeld.protocol.Field)
    - [Field.Identifier](#blickfeld.protocol.Field.Identifier)
    - [HardwareModule](#blickfeld.protocol.HardwareModule)
    - [OptionalValueRange](#blickfeld.protocol.OptionalValueRange)
    - [SoftwareVersion](#blickfeld.protocol.SoftwareVersion)
    - [ValueRange](#blickfeld.protocol.ValueRange)
  
    - [Language](#blickfeld.protocol.Language)
  
  
  

- [blickfeld/connection.proto](#blickfeld/connection.proto)
    - [Request](#blickfeld.protocol.Request)
    - [Request.AttemptErrorRecovery](#blickfeld.protocol.Request.AttemptErrorRecovery)
    - [Request.DeleteNamedScanPattern](#blickfeld.protocol.Request.DeleteNamedScanPattern)
    - [Request.Developer](#blickfeld.protocol.Request.Developer)
    - [Request.FillScanPattern](#blickfeld.protocol.Request.FillScanPattern)
    - [Request.GetAdvancedConfig](#blickfeld.protocol.Request.GetAdvancedConfig)
    - [Request.GetNamedScanPatterns](#blickfeld.protocol.Request.GetNamedScanPatterns)
    - [Request.GetScanPattern](#blickfeld.protocol.Request.GetScanPattern)
    - [Request.GetScanPatternConstraints](#blickfeld.protocol.Request.GetScanPatternConstraints)
    - [Request.Hello](#blickfeld.protocol.Request.Hello)
    - [Request.ProxyHello](#blickfeld.protocol.Request.ProxyHello)
    - [Request.RunSelfTest](#blickfeld.protocol.Request.RunSelfTest)
    - [Request.SetAdvancedConfig](#blickfeld.protocol.Request.SetAdvancedConfig)
    - [Request.SetScanPattern](#blickfeld.protocol.Request.SetScanPattern)
    - [Request.Status](#blickfeld.protocol.Request.Status)
    - [Request.StoreNamedScanPattern](#blickfeld.protocol.Request.StoreNamedScanPattern)
    - [Response](#blickfeld.protocol.Response)
    - [Response.AttemptErrorRecovery](#blickfeld.protocol.Response.AttemptErrorRecovery)
    - [Response.DeleteNamedScanPattern](#blickfeld.protocol.Response.DeleteNamedScanPattern)
    - [Response.Developer](#blickfeld.protocol.Response.Developer)
    - [Response.FillScanPattern](#blickfeld.protocol.Response.FillScanPattern)
    - [Response.GetAdvancedConfig](#blickfeld.protocol.Response.GetAdvancedConfig)
    - [Response.GetNamedScanPatterns](#blickfeld.protocol.Response.GetNamedScanPatterns)
    - [Response.GetScanPattern](#blickfeld.protocol.Response.GetScanPattern)
    - [Response.GetScanPatternConstraints](#blickfeld.protocol.Response.GetScanPatternConstraints)
    - [Response.Hello](#blickfeld.protocol.Response.Hello)
    - [Response.Hello.Firmware](#blickfeld.protocol.Response.Hello.Firmware)
    - [Response.Hello.Firmware.ModuleVersionsEntry](#blickfeld.protocol.Response.Hello.Firmware.ModuleVersionsEntry)
    - [Response.Hello.HardwareModulesEntry](#blickfeld.protocol.Response.Hello.HardwareModulesEntry)
    - [Response.ProxyHello](#blickfeld.protocol.Response.ProxyHello)
    - [Response.RunSelfTest](#blickfeld.protocol.Response.RunSelfTest)
    - [Response.SetAdvancedConfig](#blickfeld.protocol.Response.SetAdvancedConfig)
    - [Response.SetScanPattern](#blickfeld.protocol.Response.SetScanPattern)
    - [Response.StoreNamedScanPattern](#blickfeld.protocol.Response.StoreNamedScanPattern)
  
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
    - [Error.NotSupported](#blickfeld.protocol.Error.NotSupported)
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
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
    - [File-level Extensions](#blickfeld/options.proto-extensions)
  
  

- [blickfeld/config/advanced.proto](#blickfeld/config/advanced.proto)
    - [Advanced](#blickfeld.protocol.config.Advanced)
    - [Advanced.Detector](#blickfeld.protocol.config.Advanced.Detector)
    - [Advanced.Processing](#blickfeld.protocol.config.Advanced.Processing)
    - [Advanced.Server](#blickfeld.protocol.config.Advanced.Server)
    - [Advanced.TimeSynchronization](#blickfeld.protocol.config.Advanced.TimeSynchronization)
    - [Advanced.TimeSynchronization.NTP](#blickfeld.protocol.config.Advanced.TimeSynchronization.NTP)
    - [Advanced.TimeSynchronization.PTP](#blickfeld.protocol.config.Advanced.TimeSynchronization.PTP)
  
    - [Advanced.TimeSynchronization.PTP.DelayMechanism](#blickfeld.protocol.config.Advanced.TimeSynchronization.PTP.DelayMechanism)
  
  
  

- [blickfeld/config/algorithm.proto](#blickfeld/config/algorithm.proto)
    - [Algorithm](#blickfeld.protocol.config.Algorithm)
    - [Algorithm.BackgroundSubtraction](#blickfeld.protocol.config.Algorithm.BackgroundSubtraction)
    - [Algorithm.NeighborFilter](#blickfeld.protocol.config.Algorithm.NeighborFilter)
    - [Algorithm.StaticTransformation](#blickfeld.protocol.config.Algorithm.StaticTransformation)
  
  
  
  

- [blickfeld/config/generate.proto](#blickfeld/config/generate.proto)
    - [Generate](#blickfeld.protocol.config.Generate)
  
    - [Generate.Targets](#blickfeld.protocol.config.Generate.Targets)
  
  
  

- [blickfeld/config/named_scan_pattern.proto](#blickfeld/config/named_scan_pattern.proto)
    - [NamedScanPattern](#blickfeld.protocol.config.NamedScanPattern)
  
  
  
  

- [blickfeld/config/product.proto](#blickfeld/config/product.proto)
  
    - [Product](#blickfeld.protocol.config.Product)
  
  
  

- [blickfeld/config/scan_pattern.proto](#blickfeld/config/scan_pattern.proto)
    - [ScanPattern](#blickfeld.protocol.config.ScanPattern)
    - [ScanPattern.Filter](#blickfeld.protocol.config.ScanPattern.Filter)
    - [ScanPattern.Filter.Noise](#blickfeld.protocol.config.ScanPattern.Filter.Noise)
    - [ScanPattern.FrameRate](#blickfeld.protocol.config.ScanPattern.FrameRate)
    - [ScanPattern.Horizontal](#blickfeld.protocol.config.ScanPattern.Horizontal)
    - [ScanPattern.Pulse](#blickfeld.protocol.config.ScanPattern.Pulse)
    - [ScanPattern.Pulse.Custom](#blickfeld.protocol.config.ScanPattern.Pulse.Custom)
    - [ScanPattern.Pulse.Custom.AngleTrigger](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.AngleTrigger)
    - [ScanPattern.Pulse.Custom.AngleTrigger.Angle](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.AngleTrigger.Angle)
    - [ScanPattern.Pulse.Custom.Duration](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.Duration)
    - [ScanPattern.Pulse.Custom.PointTrigger](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.PointTrigger)
    - [ScanPattern.Pulse.Custom.PointTrigger.Point](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.PointTrigger.Point)
    - [ScanPattern.Vertical](#blickfeld.protocol.config.ScanPattern.Vertical)
  
    - [ScanPattern.Pulse.Custom.Trigger](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.Trigger)
    - [ScanPattern.Pulse.FrameMode](#blickfeld.protocol.config.ScanPattern.Pulse.FrameMode)
    - [ScanPattern.Pulse.Type](#blickfeld.protocol.config.ScanPattern.Pulse.Type)
  
  
  

- [blickfeld/config/secure.proto](#blickfeld/config/secure.proto)
    - [Secure](#blickfeld.protocol.config.Secure)
  
    - [Secure.CertType](#blickfeld.protocol.config.Secure.CertType)
    - [Secure.Permissions](#blickfeld.protocol.config.Secure.Permissions)
  
  
  

- [blickfeld/data/algorithm.proto](#blickfeld/data/algorithm.proto)
    - [Algorithm](#blickfeld.protocol.data.Algorithm)
    - [Algorithm.BackgroundSubtraction](#blickfeld.protocol.data.Algorithm.BackgroundSubtraction)
    - [Algorithm.NeighborFilter](#blickfeld.protocol.data.Algorithm.NeighborFilter)
    - [Algorithm.StaticTransformation](#blickfeld.protocol.data.Algorithm.StaticTransformation)
  
  
  
  

- [blickfeld/data/frame.proto](#blickfeld/data/frame.proto)
    - [Frame](#blickfeld.protocol.data.Frame)
    - [Frame.Packed](#blickfeld.protocol.data.Frame.Packed)
  
  
  
  

- [blickfeld/data/imu.proto](#blickfeld/data/imu.proto)
    - [IMU](#blickfeld.protocol.data.IMU)
    - [IMU.Packed](#blickfeld.protocol.data.IMU.Packed)
    - [IMU.Sample](#blickfeld.protocol.data.IMU.Sample)
  
  
  
  

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
  
  
  
  

- [blickfeld/file/point_cloud.proto](#blickfeld/file/point_cloud.proto)
    - [PointCloud](#blickfeld.protocol.file.PointCloud)
    - [PointCloud.Data](#blickfeld.protocol.file.PointCloud.Data)
    - [PointCloud.Footer](#blickfeld.protocol.file.PointCloud.Footer)
    - [PointCloud.Footer.Event](#blickfeld.protocol.file.PointCloud.Footer.Event)
    - [PointCloud.Footer.Stats](#blickfeld.protocol.file.PointCloud.Footer.Stats)
    - [PointCloud.Footer.Stats.Counter](#blickfeld.protocol.file.PointCloud.Footer.Stats.Counter)
    - [PointCloud.Header](#blickfeld.protocol.file.PointCloud.Header)
    - [PointCloud.Metadata](#blickfeld.protocol.file.PointCloud.Metadata)
  
  
  
  

- [blickfeld/status/imu.proto](#blickfeld/status/imu.proto)
    - [IMU](#blickfeld.protocol.status.IMU)
    - [IMU.StaticState](#blickfeld.protocol.status.IMU.StaticState)
  
  
  
  

- [blickfeld/status/main.proto](#blickfeld/status/main.proto)
    - [Status](#blickfeld.protocol.Status)
  
  
  
  

- [blickfeld/status/scanner.proto](#blickfeld/status/scanner.proto)
    - [Scanner](#blickfeld.protocol.status.Scanner)
  
    - [Scanner.State](#blickfeld.protocol.status.Scanner.State)
  
  
  

- [blickfeld/status/server.proto](#blickfeld/status/server.proto)
    - [Server](#blickfeld.protocol.status.Server)
    - [Server.Client](#blickfeld.protocol.status.Server.Client)
    - [Server.NetworkStats](#blickfeld.protocol.status.Server.NetworkStats)
    - [Server.NetworkStats.Channel](#blickfeld.protocol.status.Server.NetworkStats.Channel)
  
  
  
  

- [blickfeld/status/temperature.proto](#blickfeld/status/temperature.proto)
    - [Temperature](#blickfeld.protocol.status.Temperature)
  
    - [Temperature.Sensor](#blickfeld.protocol.status.Temperature.Sensor)
  
  
  

- [blickfeld/status/time_synchronization.proto](#blickfeld/status/time_synchronization.proto)
    - [TimeSynchronization](#blickfeld.protocol.status.TimeSynchronization)
    - [TimeSynchronization.NTP](#blickfeld.protocol.status.TimeSynchronization.NTP)
    - [TimeSynchronization.PTP](#blickfeld.protocol.status.TimeSynchronization.PTP)
  
    - [TimeSynchronization.State](#blickfeld.protocol.status.TimeSynchronization.State)
  
  
  

- [blickfeld/stream/connection.proto](#blickfeld/stream/connection.proto)
    - [Subscribe](#blickfeld.protocol.stream.Subscribe)
    - [Subscribe.Developer](#blickfeld.protocol.stream.Subscribe.Developer)
    - [Subscribe.IMU](#blickfeld.protocol.stream.Subscribe.IMU)
    - [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud)
    - [Subscribe.RawFile](#blickfeld.protocol.stream.Subscribe.RawFile)
    - [Subscribe.Status](#blickfeld.protocol.stream.Subscribe.Status)
  
  
  
  

- [blickfeld/stream/event.proto](#blickfeld/stream/event.proto)
    - [Event](#blickfeld.protocol.stream.Event)
    - [Event.Developer](#blickfeld.protocol.stream.Event.Developer)
    - [Event.EndOfStream](#blickfeld.protocol.stream.Event.EndOfStream)
  
  
  
  

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



<a name="blickfeld.protocol.Constraint"></a>

### Constraint
> Introduced in BSL v2.14 and firmware v1.14

Describes a constraint for a single target field


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| target | [Field](#blickfeld.protocol.Field) | optional | Link to target field on which the contraint is applied |
| reason | [string](#string) | optional | Human-readable reason for the constraint and its failure |
| constant | [Constraint.Constant](#blickfeld.protocol.Constraint.Constant) | optional |  |
| polynomial | [Constraint.Polynomial](#blickfeld.protocol.Constraint.Polynomial) | optional |  |






<a name="blickfeld.protocol.Constraint.Constant"></a>

### Constraint.Constant
Constant constraint which is used to apply min and/or max ranges


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| minimum | [float](#float) | optional | Target value must be equals or higher than the specified minimum |
| maximum | [float](#float) | optional | Target value must be equals or smaller than the specified maximum |






<a name="blickfeld.protocol.Constraint.Polynomial"></a>

### Constraint.Polynomial
Polynomial constraint which describe a value relationship to another field


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| reference | [Field](#blickfeld.protocol.Field) | optional | Link to reference field. The value of the reference field is used as x input for the polynomial. |
| minimum | [float](#float) | repeated | Target value must be equals or higher than the specified minimum, which is described by the specified repeated coefficients: c0 + x * c1 + x^2 * c2 .. |
| maximum | [float](#float) | repeated | Target value must be equals or smaller than the specified maximum, which is described by the specified repeated coefficients: c0 + x * c1 + x^2 * c2 .. |






<a name="blickfeld.protocol.Field"></a>

### Field
> Introduced in BSL v2.14 and firmware v1.14

Describes a protobuf field inside a (nested) message to efficiently use reflection on all supported platforms


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| identifiers | [Field.Identifier](#blickfeld.protocol.Field.Identifier) | repeated | Path to field in relation to given top level message. |
| scale | [float](#float) | optional | Numerical scale which is applied on a field value before using it for validation. This is required to prevent floating point precision issues due to rounding, which could fail a validation.<br> Validation example: Field value is stored as radian but is shown to the user as degree. Without scaling and rounding, a possible degree format could be rejected as it exceeded the maximum value of the radian respresentation after down-scaling. |






<a name="blickfeld.protocol.Field.Identifier"></a>

### Field.Identifier
Describes unique identifier of (sub) field


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| id | [uint32](#uint32) | optional | Protobuf number of field |
| key | [string](#string) | optional | Protobuf key of field |
| camelcase_key | [string](#string) | optional | Protobuf camelcase key of field |






<a name="blickfeld.protocol.HardwareModule"></a>

### HardwareModule



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| serial_number | [string](#string) | optional | Serial number of the hardware module |
| legacy_serial_number | [string](#string) | optional | Deprecated legacy serial number format |
| version | [string](#string) | optional | Hardware version of the hardware module if available. |






<a name="blickfeld.protocol.OptionalValueRange"></a>

### OptionalValueRange



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| minimum | [float](#float) | optional |  |
| maximum | [float](#float) | optional |  |






<a name="blickfeld.protocol.SoftwareVersion"></a>

### SoftwareVersion



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| name | [string](#string) | optional | Tag or reference name of the version. It is not the the name of the software. |
| revision | [string](#string) | optional | The GIT revision of the software version if available. |
| ci_job_id | [uint32](#uint32) | optional | The unique continuous integration job ID if available. |






<a name="blickfeld.protocol.ValueRange"></a>

### ValueRange



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| minimum | [float](#float) | optional |  |
| maximum | [float](#float) | optional |  |





 <!-- end messages -->


<a name="blickfeld.protocol.Language"></a>

### Language


| Name | Number | Description |
| ---- | ------ | ----------- |
| CPP | 1 |  |
| PYTHON | 2 |  |
| TYPESCRIPT | 3 |  |


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
| set_advanced_config | [Request.SetAdvancedConfig](#blickfeld.protocol.Request.SetAdvancedConfig) | optional | <blockquote>Introduced in BSL v2.11 and firmware v1.11</blockquote> Refer to [Request.SetAdvancedConfig](#blickfeld.protocol.Request.SetAdvancedConfig) |
| get_advanced_config | [Request.GetAdvancedConfig](#blickfeld.protocol.Request.GetAdvancedConfig) | optional | <blockquote>Introduced in BSL v2.11 and firmware v1.11</blockquote> Refer to [Request.GetAdvancedConfig](#blickfeld.protocol.Request.GetAdvancedConfig) |
| unsubscribe | [stream.Subscribe](#blickfeld.protocol.stream.Subscribe) | optional | <blockquote>Introduced in BSL v2.13 and firmware v1.13</blockquote> Unsubscribe a stream started with a [Subscribe](#blickfeld.protocol.stream.Subscribe) request. |
| attempt_error_recovery | [Request.AttemptErrorRecovery](#blickfeld.protocol.Request.AttemptErrorRecovery) | optional | <blockquote>Introduced in BSL v2.13 and firmware v1.13</blockquote> Refer to [Request.AttemptErrorRecovery](#blickfeld.protocol.Request.AttemptErrorRecovery) |
| get_scan_pattern_constraints | [Request.GetScanPatternConstraints](#blickfeld.protocol.Request.GetScanPatternConstraints) | optional | <blockquote>Introduced in BSL v2.14 and firmware v1.14</blockquote> Refer to [Request.GetScanPatternConstraints](#blickfeld.protocol.Request.GetScanPatternConstraints) |
| get_named_scan_patterns | [Request.GetNamedScanPatterns](#blickfeld.protocol.Request.GetNamedScanPatterns) | optional | <blockquote>Introduced in BSL v2.15 and firmware v1.16</blockquote> Refer to [Request.GetNamedScanPatterns](#blickfeld.protocol.Request.GetNamedScanPatterns) |
| store_named_scan_pattern | [Request.StoreNamedScanPattern](#blickfeld.protocol.Request.StoreNamedScanPattern) | optional | <blockquote>Introduced in BSL v2.15 and firmware v1.16</blockquote> Refer to [Request.StoreNamedScanPattern](#blickfeld.protocol.Request.StoreNamedScanPattern) |
| delete_named_scan_pattern | [Request.DeleteNamedScanPattern](#blickfeld.protocol.Request.DeleteNamedScanPattern) | optional | <blockquote>Introduced in BSL v2.15 and firmware v1.16</blockquote> Refer to [Request.DeleteNamedScanPattern](#blickfeld.protocol.Request.DeleteNamedScanPattern) |
| proxy_hello | [Request.ProxyHello](#blickfeld.protocol.Request.ProxyHello) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Refer to [Request.ProxyHello](#blickfeld.protocol.Request.ProxyHello) |
| _asJSON | [string](#string) | optional | Internal use only |
| accept_format | [Format](#blickfeld.protocol.Format) | optional | Internal use only Default: PROTOBUF |






<a name="blickfeld.protocol.Request.AttemptErrorRecovery"></a>

### Request.AttemptErrorRecovery
> Introduced in BSL v2.13 and firmware v1.13

This request can be used to attempt a re-initialization of the device if it is errored.
A self test is automatically triggered after a successful re-initialization.






<a name="blickfeld.protocol.Request.DeleteNamedScanPattern"></a>

### Request.DeleteNamedScanPattern
> Introduced in BSL v2.15 and firmware v1.16

This request deletes a named scan pattern.
Default scan patterns can't be deleted.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| name | [string](#string) | optional | Name |






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






<a name="blickfeld.protocol.Request.GetAdvancedConfig"></a>

### Request.GetAdvancedConfig
> Introduced in BSL v2.11 and firmware v1.11

This request is used to retrieve the currently set [Advanced](#blickfeld.protocol.config.Advanced).






<a name="blickfeld.protocol.Request.GetNamedScanPatterns"></a>

### Request.GetNamedScanPatterns
> Introduced in BSL v2.15 and firmware v1.16

This request returns a list of named scan patterns.






<a name="blickfeld.protocol.Request.GetScanPattern"></a>

### Request.GetScanPattern
This request is used to retrieve the currently set [ScanPattern](#blickfeld.protocol.config.ScanPattern).






<a name="blickfeld.protocol.Request.GetScanPatternConstraints"></a>

### Request.GetScanPatternConstraints
> Introduced in BSL v2.14 and firmware v1.14

This request returns a list of constraints which are applied on scan patterns.
The constraints define the constant and dynamic relationships between field values.
The constraints are equal for a device type and firmware, but might vary for firmware releases and device variants.
It is mainly used to visualize the constraints in the scan pattern configuration of the web gui.






<a name="blickfeld.protocol.Request.Hello"></a>

### Request.Hello
This request is used for initial communication after connecting to a device and testing the established connection.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| protocol_version | [uint32](#uint32) | optional | Version of the Blickfeld protocol |
| library_version | [string](#string) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Library version |
| language | [Language](#blickfeld.protocol.Language) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Library language |






<a name="blickfeld.protocol.Request.ProxyHello"></a>

### Request.ProxyHello
Internal use only

This request is used by internal proxy implementation to forward the correct ip address to the server.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| ip_address | [string](#string) | optional | IP address of the real client |






<a name="blickfeld.protocol.Request.RunSelfTest"></a>

### Request.RunSelfTest
> Introduced in BSL v2.10 and firmware v1.9

This request triggers a self test of the device.
It validates the hardware, operates the device, and generates a report.
The report is currently only accessible by developers.






<a name="blickfeld.protocol.Request.SetAdvancedConfig"></a>

### Request.SetAdvancedConfig
> Introduced in BSL v2.11 and firmware v1.11

This request is used for configuring the advanced config.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.Advanced](#blickfeld.protocol.config.Advanced) | optional | Refer to [Advanced](#blickfeld.protocol.config.Advanced) |
| persist | [bool](#bool) | optional | Persists the config and sets it after a power cycle. Default: True Default: true |






<a name="blickfeld.protocol.Request.SetScanPattern"></a>

### Request.SetScanPattern
This request is used for setting a Scan Pattern.
A Scan Pattern can either be set by providing a Scan Pattern configuration or a name of a named Scan Pattern.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| name | [string](#string) | optional | Set named Scan Pattern, refer to [NamedScanPattern](#blickfeld.protocol.config.NamedScanPattern) |
| persist | [bool](#bool) | optional | Persists the scan pattern and sets it after a power cycle. Default: False Default: false |






<a name="blickfeld.protocol.Request.Status"></a>

### Request.Status
This request is used for receiving the current [Status](#blickfeld.protocol.status.Status).






<a name="blickfeld.protocol.Request.StoreNamedScanPattern"></a>

### Request.StoreNamedScanPattern
> Introduced in BSL v2.15 and firmware v1.16

This request sets a named scan patterns.
The name has to be different of the default scan patterns and can only contain letters, numbers, space, underscore and minus.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| name | [string](#string) | optional | Name |
| config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |






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
| set_advanced_config | [Response.SetAdvancedConfig](#blickfeld.protocol.Response.SetAdvancedConfig) | optional | <blockquote>Introduced in BSL v2.11 and firmware v1.11</blockquote> Refer to [Response.SetAdvanced](#blickfeld.protocol.Response.SetAdvancedConfig) |
| get_advanced_config | [Response.GetAdvancedConfig](#blickfeld.protocol.Response.GetAdvancedConfig) | optional | <blockquote>Introduced in BSL v2.11 and firmware v1.11</blockquote> Refer to [Response.GetAdvanced](#blickfeld.protocol.Response.GetAdvancedConfig) |
| attempt_error_recovery | [Response.AttemptErrorRecovery](#blickfeld.protocol.Response.AttemptErrorRecovery) | optional | <blockquote>Introduced in BSL v2.13 and firmware v1.13</blockquote> Refer to [Response.AttemptErrorRecovery](#blickfeld.protocol.Response.AttemptErrorRecovery) |
| get_scan_pattern_constraints | [Response.GetScanPatternConstraints](#blickfeld.protocol.Response.GetScanPatternConstraints) | optional | <blockquote>Introduced in BSL v2.14 and firmware v1.14</blockquote> Refer to [Response.GetScanPatternConstraints](#blickfeld.protocol.Response.GetScanPatternConstraints) |
| get_named_scan_patterns | [Response.GetNamedScanPatterns](#blickfeld.protocol.Response.GetNamedScanPatterns) | optional | <blockquote>Introduced in BSL v2.15 and firmware v1.16</blockquote> Refer to [Response.GetNamedScanPatterns](#blickfeld.protocol.Response.GetNamedScanPatterns) |
| store_named_scan_pattern | [Response.StoreNamedScanPattern](#blickfeld.protocol.Response.StoreNamedScanPattern) | optional | <blockquote>Introduced in BSL v2.15 and firmware v1.16</blockquote> Refer to [Response.StoreNamedScanPattern](#blickfeld.protocol.Response.StoreNamedScanPattern) |
| delete_named_scan_pattern | [Response.DeleteNamedScanPattern](#blickfeld.protocol.Response.DeleteNamedScanPattern) | optional | <blockquote>Introduced in BSL v2.15 and firmware v1.16</blockquote> Refer to [Response.DeleteNamedScanPattern](#blickfeld.protocol.Response.DeleteNamedScanPattern) |
| proxy_hello | [Response.ProxyHello](#blickfeld.protocol.Response.ProxyHello) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Refer to [Response.ProxyHello](#blickfeld.protocol.Response.ProxyHello) |
| _asJSON | [string](#string) | optional | Internal use only |






<a name="blickfeld.protocol.Response.AttemptErrorRecovery"></a>

### Response.AttemptErrorRecovery
> Introduced in BSL v2.13 and firmware v1.13

This response is sent out after sending AttemptErrorRecovery.






<a name="blickfeld.protocol.Response.DeleteNamedScanPattern"></a>

### Response.DeleteNamedScanPattern
> Introduced in BSL v2.15 and firmware v1.16

This response is sent out after sending DeleteNamedScanPattern.






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






<a name="blickfeld.protocol.Response.GetAdvancedConfig"></a>

### Response.GetAdvancedConfig
> Introduced in BSL v2.11 and firmware v1.11

This response is returned after a request to get the current [Advanced](#blickfeld.protocol.config.Advanced).


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.Advanced](#blickfeld.protocol.config.Advanced) | optional | Refer to [Advanced](#blickfeld.protocol.config.Advanced) |






<a name="blickfeld.protocol.Response.GetNamedScanPatterns"></a>

### Response.GetNamedScanPatterns
> Introduced in BSL v2.15 and firmware v1.16

This response is sent out after sending GetNamedScanPatterns.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| configs | [config.NamedScanPattern](#blickfeld.protocol.config.NamedScanPattern) | repeated | List of named scan patterns, refer to [NamedScanPattern](#blickfeld.protocol.config.NamedScanPattern) |






<a name="blickfeld.protocol.Response.GetScanPattern"></a>

### Response.GetScanPattern
This response is returned after a request to get the current [ScanPattern](#blickfeld.protocol.config.ScanPattern).


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| config | [config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |






<a name="blickfeld.protocol.Response.GetScanPatternConstraints"></a>

### Response.GetScanPatternConstraints
> Introduced in BSL v2.14 and firmware v1.14

This response is sent out after sending GetScanPatternConstraints.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| constraints | [Constraint](#blickfeld.protocol.Constraint) | repeated | List of constraints which apply for scan patterns. |






<a name="blickfeld.protocol.Response.Hello"></a>

### Response.Hello
This response is sent out after establishing a connection.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| protocol_version | [uint32](#uint32) | optional | Version of the Blickfeld Protocol |
| library_version | [string](#string) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Library version |
| language | [Language](#blickfeld.protocol.Language) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Library language |
| product | [config.Product](#blickfeld.protocol.config.Product) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Refer to [Product](#blickfeld.protocol.config.Product). |
| serial_number | [string](#string) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Cube serial number |
| legacy_serial_number | [string](#string) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Deprecated legacy serial number |
| firmware | [Response.Hello.Firmware](#blickfeld.protocol.Response.Hello.Firmware) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Refer to [Firmware](#blickfeld.protocol.Response.Hello.Firmware). |
| hardware_modules | [Response.Hello.HardwareModulesEntry](#blickfeld.protocol.Response.Hello.HardwareModulesEntry) | repeated | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Hardware module map, where the key is the name and the object is the [HardwareModule](#blickfeld.protocol.HardwareModule). |






<a name="blickfeld.protocol.Response.Hello.Firmware"></a>

### Response.Hello.Firmware
> Introduced in BSL v2.16 and firmware v1.17

Contains all firmware names, versions, and revisions.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| version | [SoftwareVersion](#blickfeld.protocol.SoftwareVersion) | optional | Baseline version of firmware. Refer to [SoftwareVersion](#blickfeld.protocol.SoftwareVersion). |
| module_versions | [Response.Hello.Firmware.ModuleVersionsEntry](#blickfeld.protocol.Response.Hello.Firmware.ModuleVersionsEntry) | repeated | Version of software modules. The key of the map is the name and the object is the [SoftwareVersion](#blickfeld.protocol.SoftwareVersion). |






<a name="blickfeld.protocol.Response.Hello.Firmware.ModuleVersionsEntry"></a>

### Response.Hello.Firmware.ModuleVersionsEntry



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| key | [string](#string) | optional |  |
| value | [SoftwareVersion](#blickfeld.protocol.SoftwareVersion) | optional |  |






<a name="blickfeld.protocol.Response.Hello.HardwareModulesEntry"></a>

### Response.Hello.HardwareModulesEntry



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| key | [string](#string) | optional |  |
| value | [HardwareModule](#blickfeld.protocol.HardwareModule) | optional |  |






<a name="blickfeld.protocol.Response.ProxyHello"></a>

### Response.ProxyHello
Internal use only






<a name="blickfeld.protocol.Response.RunSelfTest"></a>

### Response.RunSelfTest
> Introduced in BSL v2.10 and firmware v1.9

This response is returned after a self test.
The success flag indicates if the hardware is fully operational.
The generated report is currently only accessible by developers.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| success | [bool](#bool) | optional |  |






<a name="blickfeld.protocol.Response.SetAdvancedConfig"></a>

### Response.SetAdvancedConfig
> Introduced in BSL v2.11 and firmware v1.11

This response is sent out after setting an SetAdvancedConfig.






<a name="blickfeld.protocol.Response.SetScanPattern"></a>

### Response.SetScanPattern
This response is sent out after setting a scan pattern.






<a name="blickfeld.protocol.Response.StoreNamedScanPattern"></a>

### Response.StoreNamedScanPattern
> Introduced in BSL v2.15 and firmware v1.16

This response is sent out after sending StoreNamedScanPattern.





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
| not_supported | [Error.NotSupported](#blickfeld.protocol.Error.NotSupported) | optional | Refer to [NotSupported](#blickfeld.protocol.Error.NotSupported) |






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
| constraints | [Constraint](#blickfeld.protocol.Constraint) | repeated | <blockquote>Introduced in BSL v2.14 and firmware v1.14</blockquote> Contains list of failed constraints. The `validation_error` field contains a human-readable output of the constraint validation. |






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


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| reason | [string](#string) | optional | Description why this is not implemented Default: No detailed reason available. |






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






<a name="blickfeld.protocol.Error.NotSupported"></a>

### Error.NotSupported
The action is not supported by the current client or server API. Check the versions of the client and firmware software and read the changelog.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| reason | [string](#string) | optional | Description why this is not supported Default: No detailed reason available. |






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
| legacy_field_id | uint64 | .google.protobuf.FieldOptions | 50010 |  |
| length | sint32 | .google.protobuf.FieldOptions | 50004 |  Default: `2147483647` |
| max_length | sint32 | .google.protobuf.FieldOptions | 50009 |  Default: `2147483647` |
| min_length | sint32 | .google.protobuf.FieldOptions | 50008 |  Default: `0` |
| optional | bool | .google.protobuf.FieldOptions | 50006 |  Default: `false` |
| regex | string | .google.protobuf.FieldOptions | 50005 |  Default: `.*` |
| ui_decimal_places | uint32 | .google.protobuf.FieldOptions | 50014 |  Default: `0` |
| ui_scale | double | .google.protobuf.FieldOptions | 50013 | Scaling of field value in user interfaces (UIs). Example: rad-to-degree conversions. Default: `1` |
| ui_unit | string | .google.protobuf.FieldOptions | 50012 | Unit of field value in user interfaces (UIs). |
| unit | string | .google.protobuf.FieldOptions | 50011 | Unit of field value |
| e_desc | string | .google.protobuf.MessageOptions | 60000 | Error description Default: `No additional error description available.` |
| generate | config.Generate | .google.protobuf.MessageOptions | 60003 |  |
| help | string | .google.protobuf.MessageOptions | 60001 | Help description |
| secure | config.Secure | .google.protobuf.MessageOptions | 60002 |  |
| optional_one_of | bool | .google.protobuf.OneofOptions | 50006 |  Default: `false` |

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/config/advanced.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/config/advanced.proto



<a name="blickfeld.protocol.config.Advanced"></a>

### Advanced
> Introduced in BSL v2.11 and firmware v1.11

Expert parameters: It is not recommended to adapt this calibrated configuration without understanding the influences on the resulting point cloud quality.

The current set of parameters is preliminary, additional parameters may be added or may get obsolete in the future.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| detector | [Advanced.Detector](#blickfeld.protocol.config.Advanced.Detector) | optional | Refer to [Detector](#blickfeld.protocol.config.Advanced.Detector) |
| processing | [Advanced.Processing](#blickfeld.protocol.config.Advanced.Processing) | optional | <blockquote>Introduced in BSL v2.12 and firmware v1.12</blockquote> Refer to [Processing](#blickfeld.protocol.config.Advanced.Processing) |
| server | [Advanced.Server](#blickfeld.protocol.config.Advanced.Server) | optional | <blockquote>Introduced in BSL v2.13 and firmware v1.13</blockquote> Refer to [Server](#blickfeld.protocol.config.Advanced.Server) |
| time_synchronization | [Advanced.TimeSynchronization](#blickfeld.protocol.config.Advanced.TimeSynchronization) | optional | <blockquote>Introduced in BSL v2.18 and firmware v1.19</blockquote> Refer to [TimeSynchronization](#blickfeld.protocol.config.Advanced.TimeSynchronization) |






<a name="blickfeld.protocol.config.Advanced.Detector"></a>

### Advanced.Detector
The behavior of the detector can be adjusted to improve the performance under extreme environmental conditions, e.g. static setup with strong daylight or basement rooms without daylight.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| sensitivity | [float](#float) | optional | Relatively influences the sensitivity of the detector. Higher values might also result into range loss. It is recommended to validate the noise filter parameters after changes to this setting. Default: 1 |






<a name="blickfeld.protocol.config.Advanced.Processing"></a>

### Advanced.Processing
> Introduced in BSL v2.12 and firmware v1.12

Processing parameters are set at the factory after calibration. Changing these values invalidates the factory calibration.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| range_offset | [float](#float) | optional | in [m] Default: 0 |
| imu_static_rotation_offset | [float](#float) | repeated | <blockquote>Introduced in BSL v2.19 and firmware v1.20</blockquote> in [rad] - Offset rotation matrix for acceleration of IMU. Can be used to calibrate the accelerometer e.g. on a flat surface. |
| horizontal_phase_offset | [float](#float) | optional | <blockquote>Introduced in BSL v2.19.5 and firmware v1.21.1</blockquote> in [s] - Correction for phase offset in the horizontal field of view. This parameter might be adjusted in the field if the upper and lower half of the frame drifted horizontally. Default: 0 |






<a name="blickfeld.protocol.config.Advanced.Server"></a>

### Advanced.Server
> Introduced in BSL v2.13 and firmware v1.13

Parameters, which control the server behavior.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| default_point_cloud_subscription | [blickfeld.protocol.stream.Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud) | optional | Overwrite default point cloud subscription. Can still be overridden with client requests. |






<a name="blickfeld.protocol.config.Advanced.TimeSynchronization"></a>

### Advanced.TimeSynchronization
> Introduced in BSL v2.18 and firmware v1.19

Activate or deactivate synchronization and change parameters


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| ntp | [Advanced.TimeSynchronization.NTP](#blickfeld.protocol.config.Advanced.TimeSynchronization.NTP) | optional | If this field is set, NTP synchronization is activated, which is the default configuration. Refer to [NTP](#blickfeld.protocol.config.Advanced.Synchronization.NTP) |
| ptp | [Advanced.TimeSynchronization.PTP](#blickfeld.protocol.config.Advanced.TimeSynchronization.PTP) | optional | If this field is set, PTP synchronization is activated and NTP synchronization is deactivated. Refer to [PTP](#blickfeld.protocol.config.Advanced.Synchronization.PTP) |






<a name="blickfeld.protocol.config.Advanced.TimeSynchronization.NTP"></a>

### Advanced.TimeSynchronization.NTP
The Blickfeld devices can use an NTPv4 client to synchronize the local device time to a time server.
NTP can achieve synchronization accuracy below one millisecond for measurement results in local networks.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| servers | [string](#string) | repeated | Specify list of NTP server. By default, debian pool NTP servers are used. |






<a name="blickfeld.protocol.config.Advanced.TimeSynchronization.PTP"></a>

### Advanced.TimeSynchronization.PTP
The Blickfeld devices can use a PTP client to synchronize the local device time to a time server.
PTP can achieve an higher accuracy than NTP in local networks.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| domain | [uint32](#uint32) | optional | PTP domain number. Default: 0 |
| delay_mechanism | [Advanced.TimeSynchronization.PTP.DelayMechanism](#blickfeld.protocol.config.Advanced.TimeSynchronization.PTP.DelayMechanism) | optional | Delay detection mode used Default: E2E |
| unicast_destinations | [string](#string) | repeated | Specify unicast slave addresses for unicast master operation, or unicast master addresses for slave operation. If this is set unicast mode will be activated. |





 <!-- end messages -->


<a name="blickfeld.protocol.config.Advanced.TimeSynchronization.PTP.DelayMechanism"></a>

### Advanced.TimeSynchronization.PTP.DelayMechanism


| Name | Number | Description |
| ---- | ------ | ----------- |
| E2E | 1 | End to end delay detection |
| P2P | 2 | Peer to peer delay detection |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/config/algorithm.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/config/algorithm.proto



<a name="blickfeld.protocol.config.Algorithm"></a>

### Algorithm
> Introduced in BSL v2.17 and firmware v1.18

This section describes configuration of on-device algorithms.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| background_subtraction | [Algorithm.BackgroundSubtraction](#blickfeld.protocol.config.Algorithm.BackgroundSubtraction) | optional | Refer to [BackgroundSubtraction](#blickfeld.protocol.config.Algorithm.BackgroundSubtraction) |
| neighbor_filter | [Algorithm.NeighborFilter](#blickfeld.protocol.config.Algorithm.NeighborFilter) | optional | Refer to [NeighborFilter](#blickfeld.protocol.config.Algorithm.NeighborFilter) |
| static_transformation | [Algorithm.StaticTransformation](#blickfeld.protocol.config.Algorithm.StaticTransformation) | optional | Refer to [StaticTransformation](#blickfeld.protocol.config.Algorithm.StaticTransformation) |






<a name="blickfeld.protocol.config.Algorithm.BackgroundSubtraction"></a>

### Algorithm.BackgroundSubtraction
> Introduced in firmware v1.16

This message configures the background subtraction.

The algorithm uses a configurable number of reference frames to estimate static points (background) in the scene.
Once the number of reference frames are processed, it removes all static points from the point cloud output.
The resulting point cloud then only contains the non-static points (foreground).
Points that are a initially part of the foreground and but then stop moving, will be added to the background and
thus disappear after a certain amount of time, which is given by the exponential decay rate parameter.
Conversely, if an object that is already part of the background suddenly starts moving, it will be added to the foreground.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| initialization_frames | [uint32](#uint32) | optional | Number of frames to initialize the background with Default: 10 |
| exponential_decay_rate | [float](#float) | optional | Controls how fast objects switch between foreground and background. Exponential decay factor. Default: 0.0025 |






<a name="blickfeld.protocol.config.Algorithm.NeighborFilter"></a>

### Algorithm.NeighborFilter
> Introduced in firmware v1.18

The neighbor filter is a simple noise filter, which uses neighboring returns in the scan pattern to decide if returns should be filtered.

The algorithm iterates through all returns and removes all, which have zero neighbors.
A neighbor return must be:
- directly to the left / to the right / above / below the current return in the scan pattern
- close to the current return. The maximum range, used for this condition, is defined by the angle spacing of the scan pattern and the range of the current return. As with higher distance, neighboring returns are expected to be further away.

No parameters can be configured at the moment.






<a name="blickfeld.protocol.config.Algorithm.StaticTransformation"></a>

### Algorithm.StaticTransformation
> Introduced in firmware v1.18

This message configures a static geometric transformation.

The algorithm applies a static rotation and translation to the point cloud output.

The rotation matrix **R** is expected to be a 3 x 3 matrix serialized in row major order.

Rotation in matrix representation:

|     |  R  |     |
| --- | --- | --- |
| r<sub>1,1</sub> | r<sub>1,2</sub> | r<sub>1,3</sub> |
| r<sub>2,1</sub> | r<sub>2,2</sub> | r<sub>2,3</sub> |
| r<sub>3,1</sub> | r<sub>3,2</sub> | r<sub>3,3</sub> |

Rotation in serialized representation:

**R** = [r<sub>1,1</sub>, r<sub>1,2</sub>, ... r<sub>3,2</sub>, r<sub>3,3</sub>]

The translation vector **T** is expected to be a 3 x 1 vector.

Translation in vector representation:

|  T  |
| --- |
|  t<sub>1</sub>  |
|  t<sub>2</sub>  |
|  t<sub>3</sub>  |

Translation in serialized representation:

**T** = [t<sub>1</sub>, t<sub>2</sub>, t<sub>3</sub>] =  [x, y, z]


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| rotation_matrix | [float](#float) | repeated | rotation matrix **R** in serialized representation. |
| translation_vector | [float](#float) | repeated | translation vector **T** in serialized representation. |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/config/generate.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/config/generate.proto



<a name="blickfeld.protocol.config.Generate"></a>

### Generate



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| targets | [Generate.Targets](#blickfeld.protocol.config.Generate.Targets) | repeated | Internal generate targets |





 <!-- end messages -->


<a name="blickfeld.protocol.config.Generate.Targets"></a>

### Generate.Targets
Internal use only

| Name | Number | Description |
| ---- | ------ | ----------- |
| DEFAULT | 0 |  |
| PROTO_HASH | 1 |  |
| MATLAB_BUS | 2 |  |
| CSTRUCT | 3 |  |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/config/named_scan_pattern.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/config/named_scan_pattern.proto



<a name="blickfeld.protocol.config.NamedScanPattern"></a>

### NamedScanPattern
> Introduced in BSL v2.15 and firmware v1.16

Named scan patterns are saved on the device. There are two kinds of named scan patterns:
1. Default scan patterns: Each device has default scan patterns, these are saved on the device with the firmware. They are not changeable.
2. Custom scan patterns: Users can save scan patterns on the device for later use. If the scan pattern is saved as a named scan pattern, the time changing to this scan pattern will greatly decrease.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| name | [string](#string) | optional | The name has to be different of the default scan patterns and can only contain letters, numbers, space, underscore and minus. |
| config | [ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Scan pattern config, refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| read_only | [bool](#bool) | optional | this is an read_only flag and it will be set if this is a default scan pattern, which can't be changed or deleted. Default: false |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/config/product.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/config/product.proto


 <!-- end messages -->


<a name="blickfeld.protocol.config.Product"></a>

### Product
Product variant

| Name | Number | Description |
| ---- | ------ | ----------- |
| PRODUCT_CUBE | 0 |  |
| PRODUCT_CUBE_RANGE | 2 |  |


 <!-- end enums -->

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
| filter | [ScanPattern.Filter](#blickfeld.protocol.config.ScanPattern.Filter) | optional | <blockquote>Introduced in BSL v2.11 and firmware v1.11</blockquote> Refer to [Filter](#blickfeld.protocol.config.ScanPattern.Filter) |
| constraints | [blickfeld.protocol.Constraint](#blickfeld.protocol.Constraint) | repeated |  |






<a name="blickfeld.protocol.config.ScanPattern.Filter"></a>

### ScanPattern.Filter
> Introduced in BSL v2.10 and firmware v1.9

Filter points and returns by point attributes during the post-processing on the device.
This can be used to e.g. filter points with low intensity or to enable secondary returns.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| max_number_of_returns_per_point | [uint32](#uint32) | optional | Set maximum number of returns per point. By default, secondary returns are disabled. Set this, e.g. to 2, to enable secondary returns. Default: 1 |
| intensity | [blickfeld.protocol.OptionalValueRange](#blickfeld.protocol.OptionalValueRange) | optional | Filter all points, which intensity values are not within this value range. |
| ambient_light_level | [blickfeld.protocol.OptionalValueRange](#blickfeld.protocol.OptionalValueRange) | optional | Filter all points, which ambient light level values are not within this value range. |
| range | [blickfeld.protocol.OptionalValueRange](#blickfeld.protocol.OptionalValueRange) | optional | Filter all points, which range values are not within this value range. |
| noise | [ScanPattern.Filter.Noise](#blickfeld.protocol.config.ScanPattern.Filter.Noise) | optional | <blockquote>Introduced in BSL v2.11 and firmware v1.11</blockquote> Refer to [Filter.Noise](#blickfeld.protocol.config.ScanPattern.Filter.Noise) |
| delete_points_without_returns | [bool](#bool) | optional | <blockquote> Introduced in BSL v2.16 and firmware v1.17</blockquote> All points without any returns are filtered if this setting is true. With active algorithms, such as background subtraction, this can reduce the bandwidth significantly. <br> Note: It is recommended to not estimate required network bandwidths when this setting is active. In scenarios, where from one frame to another, all points have returns (e.g. due to environmental changes), the peak bandwidths might increase significantly. This could result, in these scenarios, in frame losses. Default: false |






<a name="blickfeld.protocol.config.ScanPattern.Filter.Noise"></a>

### ScanPattern.Filter.Noise
> Introduced in BSL v2.11 and firmware v1.11

This is a preliminary interface to control the noise filter on the device.
All points, which have a lower intensity than the minimum intensity calculated by this filter will be filtered out.

> Changed in firmware v1.15. Please re-adjust filter configurations in scan patterns.

The internal algorithm implements a non-linear function, which describes the noise level for different ambient light levels.
The function can be scaled with the gain and shifted with the offset.

> Before firmware v1.15

The formula is: minimum_intensity = offset + gain * point.ambient_light_level,
where the point.ambient_light_level is the ambient light level of the point the laser shoots at.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| offset | [uint32](#uint32) | optional | Can be seen as minimum intensity filter, if there is no ambient light. |
| gain | [float](#float) | optional | The gain scales a non-linear function, which uses the ambient light level of the point the laser shoots at, as an input. |
| enable | [bool](#bool) | optional | <blockquote>Introduced in BSL v2.17 and firmware v1.18</blockquote>	This flag can be used to disable the internal noise filter completely. By default, the noise filter is enabled. Default: true |






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
| angle_spacing | [float](#float) | optional | Unit: [rad] – This defines the angle within which the horizontal mirror moves between two laser pulses. This parameter therefore defines the horizontal sampling resolution. |
| type | [ScanPattern.Pulse.Type](#blickfeld.protocol.config.ScanPattern.Pulse.Type) | optional | Refer to [ScanPattern.Pulse.Type](#blickfeld.protocol.config.ScanPattern.Pulse.Type) Default: INTERLEAVE |
| frame_mode | [ScanPattern.Pulse.FrameMode](#blickfeld.protocol.config.ScanPattern.Pulse.FrameMode) | optional | Refer to [ScanPattern.Pulse.FrameMode](#blickfeld.protocol.config.ScanPattern.Pulse.FrameMode) Default: COMBINE_UP_DOWN |
| distortion_correction | [bool](#bool) | optional | This parameter defines whether the distortion correction is applied when calculating the laser pulse angles. Default: True Default: true |
| custom | [ScanPattern.Pulse.Custom](#blickfeld.protocol.config.ScanPattern.Pulse.Custom) | optional | <blockquote>Introduced in BSL v2.15 and firmware v1.16</blockquote> Refer to [ScanPattern.Pulse.Custom](#blickfeld.protocol.config.ScanPattern.Pulse.Custom) |






<a name="blickfeld.protocol.config.ScanPattern.Pulse.Custom"></a>

### ScanPattern.Pulse.Custom
> Introduced in BSL v2.15 and firmware v1.16

Custom scan pattern configuration. Required for different use cases, as e.g. angle triggers, point triggers, or detector recordings.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| angle_trigger | [ScanPattern.Pulse.Custom.AngleTrigger](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.AngleTrigger) | optional |  |
| point_trigger | [ScanPattern.Pulse.Custom.PointTrigger](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.PointTrigger) | optional |  |
| duration | [ScanPattern.Pulse.Custom.Duration](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.Duration) | optional |  |






<a name="blickfeld.protocol.config.ScanPattern.Pulse.Custom.AngleTrigger"></a>

### ScanPattern.Pulse.Custom.AngleTrigger
Allows to trigger based on horizontal angles.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| angles | [ScanPattern.Pulse.Custom.AngleTrigger.Angle](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.AngleTrigger.Angle) | repeated | List of angles |






<a name="blickfeld.protocol.config.ScanPattern.Pulse.Custom.AngleTrigger.Angle"></a>

### ScanPattern.Pulse.Custom.AngleTrigger.Angle



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| horizontal_angle | [float](#float) | optional | in [rad] |
| scanline_id | [uint64](#uint64) | optional | Define in which scanline the trigger should occur, if none is given, it will trigger in all scanlines |
| enabled_triggers | [ScanPattern.Pulse.Custom.Trigger](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.Trigger) | repeated |  |






<a name="blickfeld.protocol.config.ScanPattern.Pulse.Custom.Duration"></a>

### ScanPattern.Pulse.Custom.Duration
Specify duration for trigger type in seconds.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| external_0 | [float](#float) | optional | in [s] Default: 0.0001 |
| external_1 | [float](#float) | optional | in [s] Default: 0.0001 |






<a name="blickfeld.protocol.config.ScanPattern.Pulse.Custom.PointTrigger"></a>

### ScanPattern.Pulse.Custom.PointTrigger
Set point triggers for the normal equi-distant horizontal scan pattern.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| points | [ScanPattern.Pulse.Custom.PointTrigger.Point](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.PointTrigger.Point) | repeated |  |






<a name="blickfeld.protocol.config.ScanPattern.Pulse.Custom.PointTrigger.Point"></a>

### ScanPattern.Pulse.Custom.PointTrigger.Point



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| id | [uint64](#uint64) | optional | Define which point ID should be triggered |
| enabled_triggers | [ScanPattern.Pulse.Custom.Trigger](#blickfeld.protocol.config.ScanPattern.Pulse.Custom.Trigger) | repeated |  |






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
| scanlines_up | [uint32](#uint32) | optional | Configures the number of scan lines required for the up-ramping phase. During the up-ramping phase, the vertical mirror increases its amplitude from 0 degrees to the target FoV. Default: 200 Default: 40 |
| scanlines_down | [uint32](#uint32) | optional | Configures the amount of scan lines required for the down-ramping phase. During the down-ramping phase, the vertical mirror decreases its amplitude from the target FoV to 0 degrees. Default: 30 Default: 40 |





 <!-- end messages -->


<a name="blickfeld.protocol.config.ScanPattern.Pulse.Custom.Trigger"></a>

### ScanPattern.Pulse.Custom.Trigger
List of available device triggers.

| Name | Number | Description |
| ---- | ------ | ----------- |
| TRG_EXTERNAL_0 | 1 | External output pin, routed to TEST0 on the debug board. |
| TRG_EXTERNAL_1 | 2 | External output pin, routed to TEST1 on the debug board. |
| TRG_DETECTOR | 3 | Internal detector recording, which can retrieved with the detector stream. |
| TRG_LASER | 4 | Trigger laser. |



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
| EQUI_HORIZONTAL_ANGLE | 0 | The laser is triggered depending on the horizontal angle of the laser beam with the configured angle spacing. |
| EQUI_DENSITY | 1 | Reserved for future use. Not yet supported. |
| CUSTOM | 2 | Internal feature for custom scan patterns |
| INTERLEAVE | 3 | The laser is triggered depending on the horizontal angle of the laser beam with the configured angle spacing. Scanlines are shifted by angle_spacing/4 with alternate directions. This doubles the horizontal density with the COMBINE_UP_DOWN FrameMode and efficiently uses the down-ramp phase in the inner horizontal FoV. |


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



<a name="blickfeld/data/algorithm.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/data/algorithm.proto



<a name="blickfeld.protocol.data.Algorithm"></a>

### Algorithm
> Introduced in BSL v2.17 and firmware v1.18

This section describes the additional data output of on-device algorithms.
The data is attached to the frame.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| background_subtraction | [Algorithm.BackgroundSubtraction](#blickfeld.protocol.data.Algorithm.BackgroundSubtraction) | optional | Refer to [BackgroundSubtraction](#blickfeld.protocol.data.Algorithm.BackgroundSubtraction). |
| neighbor_filter | [Algorithm.NeighborFilter](#blickfeld.protocol.data.Algorithm.NeighborFilter) | optional | Refer to [NeighborFilter](#blickfeld.protocol.data.Algorithm.NeighborFilter). |
| static_transformation | [Algorithm.StaticTransformation](#blickfeld.protocol.data.Algorithm.StaticTransformation) | optional | Refer to [StaticTransformation](#blickfeld.protocol.data.Algorithm.StaticTransformation). |
| execution_time | [float](#float) | optional | Execution time of the algorithm in [s] |






<a name="blickfeld.protocol.data.Algorithm.BackgroundSubtraction"></a>

### Algorithm.BackgroundSubtraction
The description of the algorithm can be found at [config.Algorithm.BackgroundSubtraction](#blickfeld.protocol.config.Algorithm.BackgroundSubtraction).

The algorithm does not output any additional information.
It subtracts returns from the main frame.






<a name="blickfeld.protocol.data.Algorithm.NeighborFilter"></a>

### Algorithm.NeighborFilter
The description of the algorithm can be found at [config.Algorithm.NeighborFilter](#blickfeld.protocol.config.Algorithm.NeighborFilter).

The algorithm does not output any additional information.
It subtracts returns from the main frame.






<a name="blickfeld.protocol.data.Algorithm.StaticTransformation"></a>

### Algorithm.StaticTransformation
The description of the algorithm can be found at [config.Algorithm.StaticTransformation](#blickfeld.protocol.config.Algorithm.StaticTransformation).

The algorithm does not output any additional information.
It transforms the cartesian coordinates in the frame.





 <!-- end messages -->

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
| start_time_ns | [uint64](#uint64) | optional | Unit: [s] – start frame timestamp |
| scan_pattern | [blickfeld.protocol.config.ScanPattern](#blickfeld.protocol.config.ScanPattern) | optional | Refer to [ScanPattern](#blickfeld.protocol.config.ScanPattern) |
| is_ramp_up_phase | [bool](#bool) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> If the frame_mode in the scan pattern is set to SEPARATE, this value indicates if the given frame was captured during up-ramping or down-ramping. |
| total_number_of_points | [uint32](#uint32) | optional | Number of laser pulses emitted in this frame. |
| total_number_of_returns | [uint32](#uint32) | optional | Number of returned points recorded: Each point of the [total_number_of_points](#blickfeld.protocol.data.Frame.total_number_of_points) can produce several returns. |
| scanlines | [Scanline](#blickfeld.protocol.data.Scanline) | repeated | Refer to [Scanline](#blickfeld.protocol.data.Scanline) |
| packed | [Frame.Packed](#blickfeld.protocol.data.Frame.Packed) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Refer to [Packed](#blickfeld.protocol.data.Frame.Packed) |
| algorithms | [Algorithm](#blickfeld.protocol.data.Algorithm) | repeated | <blockquote>Introduced in BSL v2.17 and firmware v1.16</blockquote> Refer to [data.Algorithm](#blickfeld.protocol.data.Algorithm). Each algorithm configured can output additional data, which is provided with this field. |






<a name="blickfeld.protocol.data.Frame.Packed"></a>

### Frame.Packed
> Introduced in BSL v2.16 and firmware v1.17

This format returns point clouds as flat binary structures.
These can be efficiently encoded & decoded, which increases the performance on low-performance clients significantly.
The network bandwidth is usually slightly higher as the data is not encoded.

Note:
The reference frame concept is handled differently for packed structures.
To activate the packed format, it is sufficient to set an empty packed message in the reference frame.
The requested fields need to be selected in the scanlines structure.
Refer to REF_FRAME_PACKED in the client implementations.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| length | [uint32](#uint32) | optional | Length of arrays |
| cartesian | [bytes](#bytes) | optional | Cartesian coordinates. 3-dimensional array in row-major format with [x, y, z] tuples. Unit: [m]. Type: Float32. Byte Order: Big Endian. |
| direction | [bytes](#bytes) | optional | Polar coordinates. 2-dimensional array in row-major format with [azimuth, elevation] tuples. Unit: [rad]. Type: Float32. Byte Order: Big Endian. |
| range | [bytes](#bytes) | optional | Distance to the return. 1-dimensional array. Unit: [m]. Type: Float32. Byte Order: Big Endian. |
| intensity | [bytes](#bytes) | optional | Intensity of the returned laser pulse. 1-dimensional array. Type: UInt32. Byte Order: Big Endian. |
| ambient_light_level | [bytes](#bytes) | optional | Ambient light level in the direction of the point. 1-dimensional array. Type: UInt32. Byte Order: Big Endian. |
| start_offset_ns | [bytes](#bytes) | optional | Starting time of the point in relation to [Frame.start_time_ns](#blickfeld.protocol.data.Frame.start_time_ns). 1-dimensional array. Unit: [ns]. Type: UInt64. Byte Order: Big Endian. |
| point_id | [bytes](#bytes) | optional | Unique point identifier within a frame and scan pattern. 1-dimensional array. Type: UInt32. Byte Order: Big Endian. |
| channel_id | [bytes](#bytes) | optional | Identifier of the channel that detected the point. 1-dimensional array. Type: UInt8. |
| return_id | [bytes](#bytes) | optional | Identifier of the return. Note: Returns are ordered by intensity not by distance. 1-dimensional array. Type: UInt8. |





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/data/imu.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/data/imu.proto



<a name="blickfeld.protocol.data.IMU"></a>

### IMU
> Introduced in BSL v2.18 and firmware v1.19

The Cube has an on-board IMU, which can be used for e.g. mapping applications or static auto-leveling.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| start_time_ns | [uint64](#uint64) | optional | Unit: [ns] – start timestamp of data. The start_offset_ns in each sample refers to this value. The timestamp is synchronized to the ones in point cloud frames. |
| samples | [IMU.Sample](#blickfeld.protocol.data.IMU.Sample) | repeated | Refer to [Sample](#blickfeld.protocol.data.IMU.Sample) |
| packed | [IMU.Packed](#blickfeld.protocol.data.IMU.Packed) | optional | Refer to [Packed](#blickfeld.protocol.data.IMU.Packed) |






<a name="blickfeld.protocol.data.IMU.Packed"></a>

### IMU.Packed



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| length | [uint32](#uint32) | optional | Length of arrays |
| start_offset_ns | [bytes](#bytes) | optional | Starting time of the point in relation to [Data.start_time_ns](#blickfeld.protocol.data.IMU.Data.start_time_ns). 1-dimensional array. Unit: [ns]. Type: UInt64. Byte Order: Big Endian. |
| acceleration | [bytes](#bytes) | optional | Accerlation. 3-dimensional array in row-major format with [x, y, z] tuples. Unit: [g]. Type: Float32. Byte Order: Big Endian. |
| angular_velocity | [bytes](#bytes) | optional | Angular velocity, measured with gyroscope. 3-dimensional array in row-major format with [x, y, z] tuples. Unit: [rad/s]. Type: Float32. Byte Order: Big Endian. |






<a name="blickfeld.protocol.data.IMU.Sample"></a>

### IMU.Sample
Describes a single IMU state.
Data rate is approx. 1.125 khz.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| start_offset_ns | [uint64](#uint64) | optional | Unit: [ns] – Offset to [start_time_ns](#blickfeld.protocol.data.IMU.start_time_ns) |
| acceleration | [float](#float) | repeated | Unit: [g] - Format: [x,y,z] - Measured acceleration for 3-axes. |
| angular_velocity | [float](#float) | repeated | Unit: [rad/s] - Format: [x,y,z] - Measured angular velocity by gyroscope for 3-axes. |





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
| subscription | [blickfeld.protocol.stream.Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud) | optional | <blockquote>Introduced in BSL v2.16 and firmware v1.17</blockquote> Used & merged subscription for stream |





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
| language | [blickfeld.protocol.Language](#blickfeld.protocol.Language) | optional | Used library language for file creation |





 <!-- end messages -->

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



<a name="blickfeld/status/imu.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/status/imu.proto



<a name="blickfeld.protocol.status.IMU"></a>

### IMU
> Introduced in BSL v2.18 and firmware v1.19

This section defines the status of the IMU.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| static_state | [IMU.StaticState](#blickfeld.protocol.status.IMU.StaticState) | optional |  |






<a name="blickfeld.protocol.status.IMU.StaticState"></a>

### IMU.StaticState



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| acceleration | [float](#float) | repeated |  |





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
| server | [status.Server](#blickfeld.protocol.status.Server) | optional | Refer to [Client](#blickfeld.protocol.status.server.Server) |
| imu | [status.IMU](#blickfeld.protocol.status.IMU) | optional | <blockquote>Introduced in BSL v2.18 and firmware v1.19</blockquote> Refer to [IMU](#blickfeld.protocol.status.IMU) |
| time_synchronization | [status.TimeSynchronization](#blickfeld.protocol.status.TimeSynchronization) | optional | <blockquote>Introduced in BSL v2.18 and firmware v1.19</blockquote> Refer to [TimeSynchronization](#blickfeld.protocol.status.TimeSynchronization) |





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



<a name="blickfeld/status/server.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/status/server.proto



<a name="blickfeld.protocol.status.Server"></a>

### Server
This section defines the status of the network server.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| clients | [Server.Client](#blickfeld.protocol.status.Server.Client) | repeated |  |
| network_stats | [Server.NetworkStats](#blickfeld.protocol.status.Server.NetworkStats) | optional |  |






<a name="blickfeld.protocol.status.Server.Client"></a>

### Server.Client



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| subscriptions | [blickfeld.protocol.stream.Subscribe](#blickfeld.protocol.stream.Subscribe) | repeated |  |
| network_stats | [Server.NetworkStats](#blickfeld.protocol.status.Server.NetworkStats) | optional |  |
| identifier | [string](#string) | optional |  |






<a name="blickfeld.protocol.status.Server.NetworkStats"></a>

### Server.NetworkStats



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| sent | [Server.NetworkStats.Channel](#blickfeld.protocol.status.Server.NetworkStats.Channel) | optional |  |
| received | [Server.NetworkStats.Channel](#blickfeld.protocol.status.Server.NetworkStats.Channel) | optional |  |
| dropped_messages | [uint64](#uint64) | optional |  Default: 0 |






<a name="blickfeld.protocol.status.Server.NetworkStats.Channel"></a>

### Server.NetworkStats.Channel



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| total_byte_count | [uint64](#uint64) | optional |  Default: 0 |
| bytes_per_second | [float](#float) | optional |  |
| maximum_bytes_per_second | [float](#float) | optional |  |





 <!-- end messages -->

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



<a name="blickfeld/status/time_synchronization.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/status/time_synchronization.proto



<a name="blickfeld.protocol.status.TimeSynchronization"></a>

### TimeSynchronization
> Introduced in BSL v2.18 and firmware v1.19

This section defines the synchronization status of the server.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| state | [TimeSynchronization.State](#blickfeld.protocol.status.TimeSynchronization.State) | optional |  |
| offset_from_master | [float](#float) | optional | Current offset value from master device in seconds. NTP: time offset (seconds) in loopstats PTP: It is the main output of the PTP engine that is in the worker state. This value is the input for clock corrections in the clock servo algorithms. This value is typically measured when estimating the performance of the worker device. |
| ntp | [TimeSynchronization.NTP](#blickfeld.protocol.status.TimeSynchronization.NTP) | optional | Refer to [NTP](#blickfeld.protocol.status.TimeSynchronization.NTP) |
| ptp | [TimeSynchronization.PTP](#blickfeld.protocol.status.TimeSynchronization.PTP) | optional | Refer to [PTP](#blickfeld.protocol.status.TimeSynchronization.PTP) |






<a name="blickfeld.protocol.status.TimeSynchronization.NTP"></a>

### TimeSynchronization.NTP



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| frequency_offset | [float](#float) | optional | frequency offset (parts per million - PPM) as in loopstats |
| rms_jitter | [float](#float) | optional | RMS jitter (seconds) as in loopstats |
| allan_deviation | [float](#float) | optional | Allan deviation (PPM) as in loopstats |
| clock_discipline_time_constant | [float](#float) | optional | clock discipline time constant as in loopstats |






<a name="blickfeld.protocol.status.TimeSynchronization.PTP"></a>

### TimeSynchronization.PTP



| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| state | [string](#string) | optional | ONLY AVAILABLE FOR PTP, NTP doesnt have a state Follwing states can occur: init: Initializing, flt: Faulty, lstn_init: Listening (first time), lstn_reset: Listening (subsequent reset), pass: Passive (not best master, not announcing), uncl: Uncalibrated, slv: Worker, pmst: Pre-master, mst: Master (active), dsbl: Disabled, ? (unk): Unknown state |
| clock_id | [string](#string) | optional | TODO Find out for NTP!! Clock ID Port identity of the current best master, as defined by IEEE 1588 standard. This ID is the local clock's ID if the local clock is the best master. This parameter is displayed as clock_id or port (host). Port is the PTP clock port number, not the User Datagram Protocol (UDP) port numbers. The clock ID is an Extended Unique Identifier (EUI)-64 64-bit ID, converted from the 48-bit MAC address, by inserting 0xfffe at the middle of the MAC address. |
| one_way_delay | [float](#float) | optional | One-way delay Current value of one-way delay (or mean-path delay) in seconds, calculated by the ptpd daemon that is in the worker state from the delay request and delay response message exchange. Note: If this value remains at zero, it means that no delay response messages are being received, which might be because of a network issue. |
| slave_to_master | [float](#float) | optional | Slave to master Intermediate offset value (seconds) extracted from the delay request and delay response message exchange. This value is used for computing one-way delay. If the last value was rejected by a filter, the previous value is shown in the log file. This value is zero (0) if the delay response messages are not received. |
| master_to_slave | [float](#float) | optional | Master to slave Intermediate offset value (seconds) extracted from the sync messages. This value is used for computing the offset value from the master devices. If the last value was rejected by a filter, the previous value is shown in the log file. |
| observed_drift | [float](#float) | optional | Observed drift The frequency difference between the worker clock and the master clock as measured by the integral accumulator of the clock control proportional integral (PI) servo model. This value stabilizes when the clock offset value is stabilized, and this value is used to detect clock stability. |
| last_packet_received | [string](#string) | optional | Last packet Received This field shows which message was received last. It displays S for sync messages and D for delay response messages. If a worker device logs no D entries, it means that the worker device is not receiving delay response messages because of network issue. |





 <!-- end messages -->


<a name="blickfeld.protocol.status.TimeSynchronization.State"></a>

### TimeSynchronization.State


| Name | Number | Description |
| ---- | ------ | ----------- |
| STOPPED | 1 |  |
| INITIALIZING | 2 |  |
| SYNCED | 3 |  |
| FAILED | 4 |  |


 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/stream/connection.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/stream/connection.proto



<a name="blickfeld.protocol.stream.Subscribe"></a>

### Subscribe
This section describes the different streams to which it is possible to subscribe.
A stream regularly provides data or status updates for the user. The events will not be pushed automatically to the BSL; the client has to retrieve them.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| point_cloud | [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud) | optional | Refer to [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud) |
| status | [Subscribe.Status](#blickfeld.protocol.stream.Subscribe.Status) | optional | Refer to [Subscribe.Status](#blickfeld.protocol.stream.Subscribe.Status) |
| developer | [Subscribe.Developer](#blickfeld.protocol.stream.Subscribe.Developer) | optional | Refer to [Subscribe.Developer](#blickfeld.protocol.stream.Subscribe.Developer) |
| raw_file | [Subscribe.RawFile](#blickfeld.protocol.stream.Subscribe.RawFile) | optional | Refer to [Subscribe.RawFile](#blickfeld.protocol.stream.Subscribe.RawFile) |
| imu | [Subscribe.IMU](#blickfeld.protocol.stream.Subscribe.IMU) | optional | <blockquote>Introduced in BSL v2.18 and firmware v1.19</blockquote> Refer to [Subscribe.IMU](#blickfeld.protocol.stream.Subscribe.IMU) |






<a name="blickfeld.protocol.stream.Subscribe.Developer"></a>

### Subscribe.Developer
Internal use only






<a name="blickfeld.protocol.stream.Subscribe.IMU"></a>

### Subscribe.IMU
> Introduced in BSL v2.18 and firmware v1.19

The Cube has an on-board IMU, which can be used for e.g. mapping applications or static auto-leveling.
A stream can be requested with this subscription.
The default rate is approx. 1.125 kHz and is provided in bursts.
The timestamps inside the protocol are synchronized to the ones in point cloud frames.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| packed_format | [bool](#bool) | optional | Request a packed binary format. This is currently used for efficent Python numpy decoding. Default: false |






<a name="blickfeld.protocol.stream.Subscribe.PointCloud"></a>

### Subscribe.PointCloud
This request is used for subscribing to a point cloud stream.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| reference_frame | [blickfeld.protocol.data.Frame](#blickfeld.protocol.data.Frame) | optional | <blockquote>Introduced in BSL v2.10 and firmware v1.9</blockquote> If present, only fields that are set in this message and submessages will be present in the point cloud. If less fields are requested, the Protobuf encoding and network transport time can reduce significantly. |
| filter | [blickfeld.protocol.config.ScanPattern.Filter](#blickfeld.protocol.config.ScanPattern.Filter) | optional | <blockquote>Introduced in BSL v2.10 and firmware v1.9</blockquote> Refer to [ScanPattern.Filter](#blickfeld.protocol.config.ScanPattern.Filter). Overrides parameters of scan pattern. |
| algorithms | [blickfeld.protocol.config.Algorithm](#blickfeld.protocol.config.Algorithm) | repeated | <blockquote>Introduced in BSL v2.17 and firmware v1.16</blockquote> Refer to [config.Algorithm](#blickfeld.protocol.config.Algorithm). Sets algorithms, which are initialized, configured for the stream and are executed on each frame in the post-processing on the device. |
| prepend_advanced_config_algorithms | [bool](#bool) | optional | <blockquote>Introduced in BSL v2.17 and firmware v1.16</blockquote> With the server.default_point_cloud_subscription algorithms can be persisted on the device and they are by default prepended to the list of sent algorithms in the subscribe request. This behaviour can be disabled with this field to e.g. disable or overwrite all algorithms. Default: true |






<a name="blickfeld.protocol.stream.Subscribe.RawFile"></a>

### Subscribe.RawFile
> Introduced in BSL v2.13 and firmware v1.13

This request is used for subscribing to a raw file stream.
The requested stream is directly packed in the Blickfeld data format and only raw bytes are sent to the client, which it should write sequentially in a file.
An [Unsubscribe](#blickfeld.protocol.stream.Unsubscribe) request with the same request data must be sent to properly end the file stream.
The request raw file stream is ended with an [EndOfStream](#blickfeld.protocol.stream.Event.EndOfStream) event.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| point_cloud | [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud) | optional | Subscribe to a raw point cloud stream. Refer to [Subscribe.PointCloud](#blickfeld.protocol.stream.Subscribe.PointCloud). |






<a name="blickfeld.protocol.stream.Subscribe.Status"></a>

### Subscribe.Status
This request is used for subscribing to a status stream.





 <!-- end messages -->

 <!-- end enums -->

 <!-- end HasExtensions -->

 <!-- end services -->



<a name="blickfeld/stream/event.proto"></a>
<p align="right"><a href="#top">Top</a></p>

## blickfeld/stream/event.proto



<a name="blickfeld.protocol.stream.Event"></a>

### Event
This section describes the events of streams.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| point_cloud | [blickfeld.protocol.data.PointCloud](#blickfeld.protocol.data.PointCloud) | optional | Refer to [PointCloud](#blickfeld.protocol.data.PointCloud) |
| status | [blickfeld.protocol.Status](#blickfeld.protocol.Status) | optional | Refer to [Status](#blickfeld.protocol.Status) |
| developer | [Event.Developer](#blickfeld.protocol.stream.Event.Developer) | optional | Refer to [Event.Developer](#blickfeld.protocol.stream.Event.Developer) |
| raw_file | [bytes](#bytes) | optional | <blockquote>Introduced in BSL v2.13 and firmware v1.13</blockquote> Raw bytes, which should be written sequentially in a file. Refer to [RawFile](#blickfeld.protocol.stream.Subscribe.RawFile). |
| end_of_stream | [Event.EndOfStream](#blickfeld.protocol.stream.Event.EndOfStream) | optional | <blockquote>Introduced in BSL v2.13 and firmware v1.13</blockquote> Refer to [EndOfStream](#blickfeld.protocol.stream.Event.EndOfStream) |
| imu | [blickfeld.protocol.data.IMU](#blickfeld.protocol.data.IMU) | optional | <blockquote>Introduced in BSL v2.18 and firmware v1.19</blockquote> Refer to [IMU](#blickfeld.protocol.data.IMU) |






<a name="blickfeld.protocol.stream.Event.Developer"></a>

### Event.Developer
Internal use only






<a name="blickfeld.protocol.stream.Event.EndOfStream"></a>

### Event.EndOfStream
> Introduced in BSL v2.13 and firmware v1.13

Event to indicate the end of stream.
This is called after an [Unsubscribe](#blickfeld.protocol.stream.Unsubscribe) request.
No further events will arrive for the subscribed stream after this event.


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| subscription | [Subscribe](#blickfeld.protocol.stream.Subscribe) | optional | Ended subscription. Refer to [Subscribe](#blickfeld.protocol.stream.Subscribe). |





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
| APOLLON_A1_BDU | 8 |  |


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

