
# Changelog
All notable changes to this project are documented in this file.
Add your changes to the 'master' section. Try to keep it short and precise.
Especially make sure to document all API changes and changes, which may 
influence the resulting point cloud.

## [master]

### Added

### Changed

### Removed

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

