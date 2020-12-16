/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once
#include <blickfeld/protocol_exception.h>
namespace blickfeld {
namespace protocol {

    static inline protocol_exception<Error::Unknown> CreateErrorUnknown(std::string description) { auto err = Error::Unknown(); err.set_description(description);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::NotImplemented> CreateErrorNotImplemented(std::string reason="No detailed reason available.") { auto err = Error::NotImplemented(); err.set_reason(reason);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::Empty> CreateErrorEmpty() { auto err = Error::Empty();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::ServerImplementation> CreateErrorServerImplementation() { auto err = Error::ServerImplementation();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::InvalidRequest> CreateErrorInvalidRequest(std::string validation_error) { auto err = Error::InvalidRequest(); err.set_validation_error(validation_error);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::ConnectionClosed> CreateErrorConnectionClosed() { auto err = Error::ConnectionClosed();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::OutdatedServerProtocol> CreateErrorOutdatedServerProtocol(uint32_t required_version) { auto err = Error::OutdatedServerProtocol(); err.set_required_version(required_version);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::OutdatedClientProtocol> CreateErrorOutdatedClientProtocol(uint32_t required_version) { auto err = Error::OutdatedClientProtocol(); err.set_required_version(required_version);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::ScannerBusy> CreateErrorScannerBusy() { auto err = Error::ScannerBusy();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::WrongOperationMode> CreateErrorWrongOperationMode() { auto err = Error::WrongOperationMode();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::NotAllowed> CreateErrorNotAllowed() { auto err = Error::NotAllowed();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::HardwareError> CreateErrorHardwareError() { auto err = Error::HardwareError();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::SystemStop> CreateErrorSystemStop() { auto err = Error::SystemStop();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::NotFound> CreateErrorNotFound() { auto err = Error::NotFound();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::UnknownErrorCode> CreateErrorUnknownErrorCode(uint32_t error_code) { auto err = Error::UnknownErrorCode(); err.set_error_code(error_code);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::NotInRange> CreateErrorNotInRange(std::string parameter, float minimum, float maximum, float requested, std::string unit="") { auto err = Error::NotInRange(); err.set_parameter(parameter); err.set_minimum(minimum); err.set_maximum(maximum); err.set_requested(requested); err.set_unit(unit);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::TimeSyncFailed> CreateErrorTimeSyncFailed(std::string ntp_daemon_log) { auto err = Error::TimeSyncFailed(); err.set_ntp_daemon_log(ntp_daemon_log);  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::NoDeviceDiscovered> CreateErrorNoDeviceDiscovered() { auto err = Error::NoDeviceDiscovered();  return protocol_exception<decltype(err)>(err); }
static inline protocol_exception<Error::NotSupported> CreateErrorNotSupported(std::string reason="No detailed reason available.") { auto err = Error::NotSupported(); err.set_reason(reason);  return protocol_exception<decltype(err)>(err); }


    static inline void throw_typed_protocol_exception (Error err) {
        switch(err.error_case()) {
            case 1: throw protocol_exception<Error::Unknown>(err.unknown()); break; 
case 2: throw protocol_exception<Error::NotImplemented>(err.not_implemented()); break; 
case 3: throw protocol_exception<Error::Empty>(err.empty()); break; 
case 4: throw protocol_exception<Error::ServerImplementation>(err.server_implementation()); break; 
case 5: throw protocol_exception<Error::InvalidRequest>(err.invalid_request()); break; 
case 6: throw protocol_exception<Error::ConnectionClosed>(err.connection_closed()); break; 
case 11: throw protocol_exception<Error::OutdatedServerProtocol>(err.outdated_server_protocol()); break; 
case 12: throw protocol_exception<Error::OutdatedClientProtocol>(err.outdated_client_protocol()); break; 
case 13: throw protocol_exception<Error::ScannerBusy>(err.scanner_busy()); break; 
case 14: throw protocol_exception<Error::WrongOperationMode>(err.wrong_operation_mode()); break; 
case 15: throw protocol_exception<Error::NotAllowed>(err.not_allowed()); break; 
case 16: throw protocol_exception<Error::HardwareError>(err.hardware_error()); break; 
case 17: throw protocol_exception<Error::SystemStop>(err.system_stop()); break; 
case 18: throw protocol_exception<Error::NotFound>(err.not_found()); break; 
case 21: throw protocol_exception<Error::UnknownErrorCode>(err.unknown_error_code()); break; 
case 22: throw protocol_exception<Error::NotInRange>(err.not_in_range()); break; 
case 23: throw protocol_exception<Error::TimeSyncFailed>(err.time_sync_failed()); break; 
case 24: throw protocol_exception<Error::NoDeviceDiscovered>(err.no_device_discovered()); break; 
case 25: throw protocol_exception<Error::NotSupported>(err.not_supported()); break; 

            case Error::ERROR_NOT_SET:
            default:
            break;
        }
        auto unknown = Error::UnknownErrorCode();
        unknown.set_error_code(err.error_case());
        throw protocol_exception<decltype(unknown)>(unknown);
    }
}
}


