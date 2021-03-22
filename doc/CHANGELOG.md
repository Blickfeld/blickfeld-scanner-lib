
# Changelog
All notable changes to this project are documented in this file.
Add your changes to the 'master' section. Try to keep it short and precise.
Especially make sure to document all API changes and changes, which may 
influence the resulting point cloud.

## [master]

### Added

### Changed

### Removed

## [2.19.0] - 2021.03.22

### Added
* Doc: Add descriptions and source code of Python and C++ examples
* Python: Example script for IMU calibration
* Protocol: Advanced calibration configuration for static IMU accelerometer offset

## [2.18.2] - 2021.03.08

### Added
* C++: Add clean shutdown method to mockup server
* Automatically wait for successful time synchronization

### Removed
* Python: Dropped legacy methods `set_ntp_server` and `get_ntp_server`

## [2.18.1] - 2021.03.02

### Changed
* Protocol: Do not enforce new status fields

## [2.18.0] - 2021.03.02

### Added
* [Introduced in firmware v1.19] Support for on-board IMU stream. Accelerometer and gyroscope data is provided with approx. 1 kHz and synchronized timestamps.
* [Introduced in firmware v1.19] Support for PTPv2 time synchronization
* Protocol: Status message and state of time synchronization

### Changed
* Python: Re-worked time synchronization interface. Advanced configuration is used with up-to-date devices.

### Removed
* Protocol: Remove advanced `classification_threshold` of background subtraction algorithm

## [2.17.3] - 2021.02.12

### Changed
* CI: Update outdated auto-generated protocol documentation

## [2.17.2] - 2021.02.12

### Added
* Mockup-server, which can simulate a Blickfeld scanner by reading a point cloud recording. The implementation is currently **incomplete**, please read the documentation in the source code and the C++ server example.

### Changed
* Protocol: Improved documentation of algorithms
* Python: Extended set_default_point_cloud_algorithms with static transformation

## [2.17.1] - 2021.02.08

### Changed
* Replace c++17 with c++14 call and added test to check compatibility
* Bugfix: Builds without Python failed with missing target dependency
* Enforce protobuf version 3

## [2.17.0] - 2021.02.05

### Added
* [Introduced in firmware v1.18] Algorithm API with three on-device processing algorithms: Background subtraction, neighbour noise filter, and static transformation.
* [Introduced in firmware v1.18] Option to disable the noise filter in the scan pattern configuration

## [2.16.1] - 2021.01.26

### Added
* Python: Structured numpy interface flag is now available for file streams

### Changed
* Bugfix: Do not enforce the legacy serial number for hardware modules
* API: Rename filter `points_with_no_returns` to `delete_points_without_returns`

## [2.16.0] - 2020.12.16

### Added
* [Introduced in firmware v1.17] API: Introduce packed frame format. The format returns point clouds as flat binary structures. These can be efficiently encoded & decoded, which increases the performance on low-performance clients significantly.
* Python: Add structured numpy interface for point clouds
* [Introduced in firmware v1.17] Extended hello message with library, firmware, and hardware information. Added requests for serial number & product variant.

## [2.15.0] - 2020.12.02

### Added
* CPP: Add support for point cloud streams using extensions
* [Introduced in firmware v1.16] API: Add custom trigger API, which allows to control external trigger outputs for special product variants
* [Introduced in firmware v1.16] API: Add named scan pattern API. Multiple patterns can be persisted and are pre-loaded.

## [2.14.2] - 2020.11.25

### Changed
* Python: Raise minimum required Protobuf version due to incompability with currently specified version

## [2.14.1] - 2020.11.09

### Changed
* Python: Moved status stream to stream folder.
* Python: Unified stop and close in the different streams. Please now use stop in all the streams.
* [Introduced in firmware v1.15] Replaced linear filter with non-linear function. Please re-adjust filter configurations in scan patterns stored on the device.

## [2.14.0] - 2020.10.19

### Added
* [Introduced in firmware v1.14] API request to fetch constraints for allowed scan pattern configurations. This improves the Web GUI and results in improved validation error messages.

### Changed
* Python: Fixed point cloud stream close to first call stop and unsubscribe
* Python: Fixed bug when stopping a raw stream and deleting it afterwards, it would call stop again

## [2.13.0] - 2020.09.25

### Added
* [Introduced in firmware v1.13] Separate API request for raw file streams. This allows file recordings with Python on low-performance clients.
* [Introduced in firmware v1.13] Server section to status message. Reports connected clients and network statistics.
* Add protocol files to install target
* [Introduced in firmware v1.13] Extend advanced configuration with `default_point_cloud_subscription`
* [Introduced in firmware v1.13] Request to attempt error recovery

### Changed
* Python: Improved examples and refactored arguments
* CPP: Refactored point cloud recording class
* CMake: Improved the approach of linking dependent library to make the install package relocatable

### Removed
* [Removed in firmware v1.13] Drop deprecated legacy scan patterns

## [2.12.1] - 2020.09.08

### Changed
* Windows: Only disable Protobuf-related warnings
* Python: Throw explicit error types in point cloud stream class

## [2.12.0] - 2020.09.08

### Added
* Protocol: Additional processing section in advanced configuration
* Example for builds with external projects which locates all dependencies in a local directory. Can be found in cmake/external. Install section in documentation is updated.

### Changed
* Bugfix: M_PI was not defined and compiler flags were overridden in cpp windows builds

## [2.11.1] - 2020.08.31

### Added
* Python: Flag for detecting frame losses

### Changed
* Change default scan pattern to 40 scanlines up, 40 scanlines down, INTERLEAVE pulse type, and COMBINE_UP_DOWN pulse frame mode
* Bugfix Python: Fixed and simplified the recording functionality. The compression level can now be adjusted and is default set to 1.
* Protocol: Formatted protobuf files for better readability
* Python: Follow PEP 8 version rules

## [2.11.0] - 2020.08.17

### Added
* Python: Example of frame export for matlab / octave
* [Introduced in firmware v1.10] Add filter parameters to scan pattern. It allows to increase the number of returns per point and provides min/max filters for point attributes. The filter can be overridden during a stream request.
* [Introduced in firmware v1.11] Add preliminary noise filter configuration to scan pattern
* Protobuf: Add internal message option to trigger export of custom targets
* CI: Provide test coverage
* [Introduced in firmware v1.11] Interleave frame mode, which allows to double the horizontal density with the COMBINE_UP_DOWN FrameMode and efficiently uses the down-ramp phase in the inner horizontal FoV
* [Introduced in firmware v1.11] Add API request for advanced configuration

### Changed
* Bugfix: Removed incorrect end of line prints in cpp fetch_clouds example

## [2.10.2] - 2020.07.22

### Changed
* Bugfix: Frame was recorded multiple times if Python writer was faster than frame receiver

## [2.10.1] - 2020.07.21

### Added
* Protocol: HW Variant Apollon A1 BDU
* Reference frame for depth map

### Changed
* Doc: Fix rendering of python point cloud stream
* Doc: Add configure point cloud stream to python examples in the doc
* Upgrade debian testing package to libprotobuf23

## [2.10.0] - 2020.07.10

### Added
* [Introduced in firmware v1.9] Protocol: Request to trigger a self test of the device
   - Validates the hardware, operates the device, and generates a report.
   - The report is currently only accessible by developers.
   - A success flag indicates if the device is functional.
* [Introduced in firmware v1.9] Configurable point-cloud stream: Used to reduce the network bandwidth and to decrease the Protobuf encoding/decoding effort
   - The post-processing can be configured with a filter. It allows to increase the number of returns per point and provides min/max filters for point attributes.
   - The desired frame fields in the stream can be requested by passing a reference frame. Example reference frames are available.

### Changed
* CI: Improve internal release automation

## [2.9.1] - 2020.06.25

### Changed
* Update links to documentation

## [2.9.0] - 2020.06.22

### Added
* Protocol: Temporary error flags per point
* Protocol: Enable Protobuf Arena support for data messages to improve performance

### Changed
* Documentation: Revise the BSL documentation with feedback of the technical writers
* CMake: Prevent creation of always out-of-date target with multiple dependencies
* Documentation: Add links to the GitHub Repo

## [2.8.9] - 2020.06.12

### Changed
* Doc: Fix copy-paste error in general guide

## [2.8.8] - 2020.06.12

### Changed
* Doc: Temporarily link to local documentation as full technical documentation is not yet available 

## [2.8.7] - 2020.06.09

### Changed
* Python: Use temporary cert file for SSL connections

## [2.8.6] - 2020.06.05

### Changed
* CI: Re-add git related files

## [2.8.5] - 2020.06.05

### Changed
* CPP code examples

### Removed
* NPM module. Release is postponed for 6-8 weeks.
* Unused commented code
* Old documentation files

## [2.8.4] - 2020.06.04

### Changed
* CI: Correct squashed commits 

## [2.8.3] - 2020.06.04

### Added
* General README with an external link to docs.blickfeld.com

### Changed
* CI: Fix auto generator for tagged builds
* Run code formatter on CPP source code

## [2.8.2] - 2020.06.04

### Changed
* Doc: Correct artifacts URL of debian packages
* CI: Fix upstream sync to GitHub repository
* [closes #22461] CMake: Clean up targets and drop protoc-dev dependency required for plugin

## [2.8.1] - 2020.06.03

### Changed
* Doc: Correct artifacts URL of debian packages
* Python: Gracefully handle broken `zeroconf` package, as Python 2 support is missing
* Doc: Add missing doumentation for client file header
* CI: Provide python package without version information in filename

## [2.8.0] - 2020.06.02

### Added
* Python: Add string representation for blickfeld_scanner
* Add custom error TimeSyncFailed to provide log information of NTP daemon
* Python & C++: Simple search & connect methods to discover Blickfeld devices
* Add testing debian package for more recent protobuf versions
* Introduce new point cloud format and add support for Python and CPP
* CI: Automatic deployment to GitHub

### Changed
* Move part of the documentation to the top-level technical documentation
* CI: Link to customer release tied to job-id instead of tag-name
* Python: Prepare pypi package for release
* Documentation: Implement feedback from technical writers
* Documentation: Clean up & additional information about installation
* Documented the core functionality in the source code headers

## [2.7.3] - 2020.03.31

### Changed
* Use latest python version for conda packages
* Build PIP package and deploy it
* Bugfix: Adapt set_ntp_server to new REST API

## [2.7.2] - 2020.03.27

### Added
* Introduce developer protobuf permissions

### Changed
* Bugfix: Generation issue in CI pipeline

## [2.7.1] - 2020.03.27

### Changed
* Bugfix: Documentation was not updated in release process

## [2.7.0] - 2020.03.27

### Added
* SSL support for developer requests
* Doc: Add explanation how to check the python protobuf implementation
* Doc: Fix hardcoded paths in the doc creation and move doc to build folder before building it
* Doc: Check for doxygen, sphinx and python before creating the doc and return fatal error when one is missing
* Doc: Read automatic commit for the protobuf_protocol.md creation
* Debian package for blickfeld-scanner-lib

### Changed
* Fix cpp examples (error_handling and set_scan_pattern)
* Bugfix: Library links in CMake for OpenSSL and Protobuf
* Refactor CMake configuration and add dependency check
* Bugfix: Build issues with protobuf plugin on Windows
* Bugfix: Use python3 as default

## [2.6.1] - 2020.03.18

### Added
* Support secure firmware updates

### Changed
* C++: Do not force minimum protobuf version 3.6.1

## [2.6.0] - 2020.03.06

### Changed
* Bugfix: Fix bf_protocol_plugin build in Petalinux

## [2.5.4] - 2020.03.04

### Changed
* Bugfix: Re-add protocol documentation
* Bugfix: Re-add python package documentation

## [2.5.3] - 2020.03.03

### Changed
* Bugfix: Build script in python package was missing

## [2.5.2] - 2020.03.03

### Changed
* Bugfix: CI pipeline failed to generate customer deployment

## [2.5.1] - 2020.03.03

### Changed
* Bugfix: CI pipeline did fail to generate documentation

## [2.5.0] - 2020.03.03

### Added
* Add the bsl documentation to the customer release
* Add `persist` flag to SetScanPattern request, which reloads the scan pattern after a power-cycle
* Add function to get the scanner connection in the C++ BSL implementation
* Automatic discovery to python library

### Changed
* Refactor scan pattern methods of NPM package
* Fallback IP address of the devices changed to `192.168.26.26`. Read [Network Configuration](Network-Configuration) for more information.

## [2.4.0] - 2020.02.13

### Added
* Documentation of our Scan Pattern

### Changed
* Bugfix: Scanner disconnect caused segfault due to uninitialized pointers
* Improved the protobuf documentation

## [v2.3.1] - 2020.02.04

### Changed
* Re-add `get_status_stream` in python  package

## [v2.3.0] - 2020.02.04

### Added
* Add sphinx documentation with general information, python and protocol documentation

### Changed
* The field of view in [ScanPattern.Horizontal](protobuf_protocol.html#blickfeld.protocol.config.ScanPattern.Horizontal) now defines the optical FOV, not the mechanical FOV as previously.
  The `padding` configuration in [ScanPattern.Pulse](protobuf_protocol.html#blickfeld.protocol.config.ScanPattern.Pulse) is, therefore, obsolete.
  Additional, all scan patterns for Cube Range devices now require the optical [ScanPattern.Horizontal.fov](protobuf_protocol.html#blickfeld.protocol.config.ScanPattern.Horizontal) and [ScanPattern.Pulse.angle_spacing](protobuf_protocol.html#blickfeld.protocol.config.ScanPattern.Pulse).
  It is recommended to use [Request.FillScanPattern](protobuf_protocol.html#blickfeld.protocol.Request.FillScanPattern) with an empty configuration to retrieve a valid example configuration of the used device.

