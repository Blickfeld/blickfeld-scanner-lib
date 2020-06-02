# Exceptions and error-handling

During communication, the device may report possible error states.
The error class and a detailed description are embedded in the response packet.

When using exception-handling in C++, the client application can detect exceptions of interest and handle the situation accordingly.

## Examples

For a live example, take a look at blickfeld-scanner-library/examples/cpp/error_handling/main.cpp (link not working at the moment!)
from the C++ examples.

### InvalidRequest
If improperly defined scan patterns (e.g., fields are not set) are sent to the device, it will respond with the corresponding exception.
In the example below, the desired scan pattern is not configured at all.

This will generate an InvalidRequest exception, which is embedded in the response from the sensor.
  
```cpp
std::shared_ptr<blickfeld::scanner> scanner;
scanner = blickfeld::scanner::connect("my_cube");

blickfeld::protocol::config::ScanPattern scan_pattern;
try {
    scanner->set_scan_pattern(scan_pattern);
} catch (const blickfeld::protocol_exception<blickfeld::protocol::Error_InvalidRequest>& e) {
    fprintf(stderr, "%s\n", e.what());
}
```
### NotInRange
When configuring the scan pattern, exceptions can also indicate the 
misconfigured parameter and give hints on how to properly set the value. In the
example below the scan-pattern does not have the right amount of scanlines
*(16 scanlines-up @ 30[deg] vertical-fov)*.

```cpp
blickfeld::protocol::config::ScanPattern scan_pattern;
scan_pattern.mutable_horizontal()->set_fov(10.f / 180.f * 3.1415f);
scan_pattern.mutable_vertical()->set_fov(30.f / 180.f * 3.1415f);
scan_pattern.mutable_vertical()->set_scanlines_up(16);
scan_pattern.mutable_vertical()->set_scanlines_down(30);
scan_pattern.mutable_pulse()->set_frame_mode(blickfeld::protocol::config::ScanPattern::Pulse::FrameMode::ScanPattern_Pulse_FrameMode_COMBINE_UP_DOWN);

try {
    scan_pattern = scanner->fill_scan_pattern(scan_pattern);
} catch (const blickfeld::protocol_exception<blickfeld::protocol::Error_NotInRange>& e) {
    fprintf(stderr, "%s\n", e.what());
}
```

This request will generate the following output:

```
blickfeld.protocol.Error.NotInRange:
	The desired number of scan lines up 16.00 is not in range. The number of scanlines up has to be in the interval of 17.00 < inf.
```

## Note for developers

### Throwing errors

In C++ every defined errormessage from ```error.proto``` can be created in two ways:

* Protobuf syntax
* Custom constructor using fields

The custom constructor already includes all the required fields. See the example below on how
to create an exception with error messages using different approaches.


```cpp
// custom exception constructor using fields
throw blickfeld::protocol::CreateErrorUnknown("something went wrong!");
```

```cpp
// protobuf-syntax
auto err_protobuf = blickfeld::protocol::Error::Unknown();
err_protobuf.set_description("something went wrong!");
throw protocol_exception<typeof(err)>(err);
``` 

### Adding new error types

To define new error types, edit the error definition in ```error.proto```.

1) Add a new submessage to the ```Error``` message type. For example:
    ```proto
   message NothingToSeeHere {
        option (e_desc) = "The {object:%s} has not been seen for the last {duration:%d}{unit:%s}.";
	    optional string object = 1;
        optional uint32 duration = 2;
        optional string unit = 3 [default = "s"];
    }
   ```
2) Reference the newly created submessage in the ```Error.error``` oneof field.
    ```proto
   oneof error {
       NothingToSeeHere nothing_to_see_here = 42;
    }
   ```
    Be careful when choosing the new field ID within the one-of field.
    The ID for your new error may not already have been assigned to an existing field and may not be within the reserved ranges.
3) After one successful build, the new error type is ready to be used and integrated via the constructor.
