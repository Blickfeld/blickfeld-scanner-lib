/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include <string>
#include <map>
#include <memory>
#include <thread>
#include <functional>

#include "logged_object.h"

#include "blickfeld/scanner-config.h"
#include "blickfeld/data.h"
#include "blickfeld/point_cloud_stream.h"

#ifndef BSL_STANDALONE
#include "blickfeld/connection.pb.h"
#include "blickfeld/file/point_cloud.pb.h"
#include "blickfeld/imu_stream.h"
#endif

namespace asio {
class io_context;
    #ifdef HAVE_OPENSSL
// asio/ssl.hpp
namespace ssl {
class context;
}
    #endif
}

namespace blickfeld {

namespace protocol {
class Response;
class Response_Hello;
namespace data {
class Frame;
}
namespace config {
class ScanPattern_Filter;
}
namespace stream {
class Subscribe_PointCloud;
}
}

class connection;
class point_cloud_record;

/**
 * Blickfeld Scanner class for point cloud streams, status, and configuration requests.
 */
class BF_DLLEXPORT scanner : public logged_object {
public:

#ifndef BSL_STANDALONE
	static const protocol::data::Frame REF_FRAME_XYZ; // Reference frame: XYZ coordinates
	static const protocol::data::Frame REF_FRAME_XYZ_I; // Reference frame: XYZ coordinates, intensity
	static const protocol::data::Frame REF_FRAME_XYZ_I_ID; // Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id
	static const protocol::data::Frame REF_FRAME_XYZ_I_ID_TS; // Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id, timestamps
	static const protocol::data::Frame REF_FRAME_DEPTH_MAP; // Reference frame: ambient_light_level, intensity, range, frame id, scanline id, point id
	static const protocol::data::Frame REF_FRAME_FULL; // Reference frame: All attributes (default)
	static const protocol::data::Frame REF_FRAME_PACKED; // Reference frame in packed format: XYZ coordinates, direction, range, intensity, ambient_light_level, frame id, scanline id, point id, return id, timestamps
#endif

	template<class frame_t>
	using point_cloud_stream = point_cloud_stream<frame_t>;

#ifndef BSL_STANDALONE
	using subscribe_status_callback_t = std::function<void (const protocol::Status&)>;
#endif

protected:
	asio::io_context* io_context = nullptr;
	std::string hostname_or_ip;
	connection* conn = nullptr;

#ifdef HAVE_OPENSSL
	asio::ssl::context* ssl_context = nullptr;
#endif

	void __hello(protocol::Response_Hello* hello);

	/**
	 * Internal use. Use static method connect.
	 *
	 * @param hostname_or_ip Hostname or IPv4 address of the device.
	 * @param cert_key_file Filename of the cert file for secured connections.
	 */
	scanner(std::string hostname_or_ip, std::string cert_key_file = "");

	/**
	 * Duplicate scanner connection.
	 * @return Dedicated scanner connection with same configuration as the scanner instance.
	 */
	std::shared_ptr<connection> create_connection();

public:
	virtual ~scanner();

	/**
	 * Creates scanner instance with the hostname or IPv4 address of the device.
	 * An initial handshake is performed in the connect.
	 * Fails if the handshake failed.
	 *
	 * @param hostname_or_ip Hostname or IPv4 address of the device.
	 * @return Shared pointer of the scanner instance.
	 */
	static std::shared_ptr<scanner> connect(std::string hostname_or_ip);

#ifdef HAVE_OPENSSL
	/**
	 * Creates secured scanner instance.
	 * Currently used internally for configuration purposes.
	 *
	 * @param hostname_or_ip Hostname or IPv4 address of the device.
	 * @param cert_key_file Filename of the cert file for secured connections.
	 * @return Shared pointer of the secured scanner instance.
	 */
	static std::shared_ptr<scanner> connect(std::string hostname_or_ip, std::string cert_key_file);
#endif

	/**
	 * Get name of the scanner instance
	 * @return Name of the scanner device.
	 */
	const std::string name();

	/**
	 * Runs asynchronous scheduler for subscriptions.
	 * Use the subscribe methods to register asynchronous handler.
	 */
	void async_run_blocking();

	/**
	 * Wrapper for running asynchronous scheduler in a thread.
	 * Thread has to started and kept in scope.
	 *
	 * @return Thread with asynchronous scheduler.
	 */
	std::shared_ptr<std::thread> async_run_thread();

	/**
	 * Fetches point cloud frames in a reduced data format without Protobuf dependencies.
	 * It is recommended to use get_point_cloud_stream to get all available scan pattern and point information.
	 * This call already boots up the device.
	 * It fails if the device is in an errored state.
	 * Delete the shared reference to stop the stream.
	 *
	 * @return Shared pointer of stream instance. Use the recv_frame method to get Point Cloud Frames.
	 */
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::frame_t> > get_simple_point_cloud_stream();
#ifdef BSL_RECORDING

	/**
	 * Reads point cloud in a reduced data format without Protobuf dependencies.
	 * The istream must be kept in scope until all shared references are deleted.
	 *
	 * @return Shared pointer of stream instance. Use the recv_frame method to fetch Point Cloud Frames.
	 */
	static std::shared_ptr<scanner::point_cloud_stream<protocol::data::frame_t> > simple_file_point_cloud_stream(std::istream* istream);
#endif

	/**
	 * > Introduced in BSL v2.13 and firmware v1.13
	 *
	 * Can be used to attempt a re-initialization of the device if it is errored.
	 * A self test is automatically triggered after a successful re-initialization.
	 */
	void attempt_error_recovery();

	/**
	 * > Introduced in BSL v2.16 and firmware v1.17
	 *
	 * Retrieve unique serial number of the device.
	 * Consider using hello() to retrieve more information.
	 */
	const std::string serial_number();

#ifndef BSL_STANDALONE
	/**
	 * > Introduced in BSL v2.16 and firmware v1.17
	 *
	 * Retrieve product type.
	 * Consider using hello() to retrieve more information.
	 */
	const protocol::config::Product product();

	/**
	 * Fetches point cloud frames from the device.
	 * This call already boots up the device.
	 * It fails if the device is in an errored state.
	 * Delete the shared reference to stop the stream.
	 *
	 * @return Shared pointer of stream instance. Use the recv_frame method to get Point Cloud Frames.
	 */
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > get_point_cloud_stream();

	/**
	 * > Introduced in BSL v2.10 and firmware v1.9
	 *
	 * Fetches point cloud frames from the device.
	 * This call already boots up the device.
	 * It fails if the device is in an errored state.
	 * Delete the shared reference to stop the stream.
	 *
	 * @param filter Filter points and returns by point attributes during the post-processing on the device.
	 * @return Shared pointer of stream instance. Use the recv_frame method to get Point Cloud Frames.
	 */
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > get_point_cloud_stream(const protocol::config::ScanPattern_Filter filter);

	/**
	 * > Introduced in BSL v2.10 and firmware v1.9
	 *
	 * Fetches point cloud frames from the device.
	 * This call already boots up the device.
	 * It fails if the device is in an errored state.
	 * Delete the shared reference to stop the stream.
	 *
	 * @param reference_frame Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
	 * @return Shared pointer of stream instance. Use the recv_frame method to get Point Cloud Frames.
	 */
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > get_point_cloud_stream(const protocol::data::Frame reference_frame);

	/**
	 * > Introduced in BSL v2.10 and firmware v1.9
	 *
	 * Fetches point cloud frames from the device.
	 * This call already boots up the device.
	 * It fails if the device is in an errored state.
	 * Delete the shared reference to stop the stream.
	 *
	 * @param filter Filter points and returns by point attributes during the post-processing on the device.
	 * @param reference_frame Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
	 * @return Shared pointer of stream instance. Use the recv_frame method to get Point Cloud Frames.
	 */
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > get_point_cloud_stream(const protocol::config::ScanPattern_Filter filter, const protocol::data::Frame reference_frame);

	/**
	 * > Introduced in BSL v2.17
	 *
	 * Fetches point cloud frames from the device.
	 * This call already boots up the device.
	 * It fails if the device is in an errored state.
	 * Delete the shared reference to stop the stream.
	 *
	 * @param algorithms List of algorithms to enable for this stream.
	 * @return Shared pointer of stream instance. Use the recv_frame method to get Point Cloud Frames.
	 */
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > get_point_cloud_stream(const std::vector<protocol::config::Algorithm> algorithms);

	/**
	 * > Introduced in BSL v2.17
	 *
	 * Fetches point cloud frames from the device.
	 * This call already boots up the device.
	 * It fails if the device is in an errored state.
	 * Delete the shared reference to stop the stream.
	 *
	 * @param extend_subscription Reference subscription. These parameters can enable on-device algorithms, filter values and customize the data fields in the point cloud output.
	 * @return Shared pointer of stream instance. Use the recv_frame method to get Point Cloud Frames.
	 */
	std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > get_point_cloud_stream(const protocol::stream::Subscribe::PointCloud extend_subscription);

	/**
	 * > Introduced in BSL v2.18 and firmware v1.19
	 *
	 * Fetches IMU data from Cube.
	 * Delete the shared reference to stop the stream.
	 *
	 * @return Shared pointer of stream instance. Use the recv_burst method to get IMU data.
	 */
	std::shared_ptr<imu_stream> get_imu_stream();

#ifdef BSL_RECORDING

	/**
	 * Reads point cloud from a Blickfeld Point Cloud Frame
	 * The istream must be kept in scope until all shared references are deleted.
	 *
	 * @param istream Input stream with Point Cloud Recording. The file format is described in the technical documentation: "Blickfeld Scanner Library : File Format".
	 * @return Shared pointer of stream instance. Use the recv_frame method to read Point Cloud Frames.
	 */
	static std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > file_point_cloud_stream(std::istream* istream);
#endif

	/**
	 * Get current status of the device.
	 * @return The status definiton can be found in the protocol section of the technical documentation.
	 */
	const protocol::Status get_status();
	void subscribe(subscribe_status_callback_t cb);

	/**
	 * Send hello to device and retrieve device, software, and hardware information
	 * @return The hello definiton can be found in the protocol section of the technical documentation.
	 */
	const protocol::Response::Hello hello();

	/// Use this function to get the current <a href="../protobuf_protocol.html#blickfeld/config/scan_pattern.proto">ScanPattern</a>
	const protocol::config::ScanPattern get_scan_pattern();

	/// If you want to set a new ScanPattern, first call this function with the new ScanPattern and then pass the returned ScanPattern to
	/// set_scan_pattern(const protocol::config::ScanPattern& config)
	///
	/// @param config ScanPattern to fill by the BSS
	/// @returns A filled ScanPattern to send via set_scan_pattern(const protocol::config::ScanPattern& config)
	const protocol::config::ScanPattern fill_scan_pattern(const protocol::config::ScanPattern& config);

	/// Set a ScanPattern, by providing a scan pattern
	///
	/// @param config ScanPattern to be set by the BSS
	/// @param persist persist flag. The device will have this scan pattern set after the next reboot
	/// @warning First fill the ScanPattern with fill_scan_pattern(const protocol::config::ScanPattern& config)
	void set_scan_pattern(const protocol::config::ScanPattern& config, bool persist = false);

	/// Set a ScanPattern, by providing a name of a named scan pattern, see the protocol definitions of the Named Scan patterns
	///
	/// @param name Name of the named scan pattern to be set by the BSS
	/// @param persist persist flag. The device will have this scan pattern set after the next reboot
	void set_scan_pattern(const std::string name, bool persist = false);

	/// > Introduced in BSL v2.15 and firmware v1.16
	///
	/// Get protobuf list of named scan patterns. There are two types of named scan patterns:
	/// 1. Default scan patterns, which are not changeable.
	/// 2. User defined named scan patterns, which are changeable.
	///
	/// @returns List of named scan patterns, see protobuf_protocol Response.GetNamedScanPatterns
	std::list<protocol::config::NamedScanPattern> get_named_scan_patterns();

	/// > Introduced in BSL v2.15 and firmware v1.16
	///
	/// Store a named scan patterns.
	/// The default scan patterns can't be overwritten.
	///
	/// @param name: Name of the scan pattern
	/// @param config: Config of the scan pattern, see protobuf_protocol ScanPattern
	void store_named_scan_pattern(std::string name, const protocol::config::ScanPattern& config);

	/// > Introduced in BSL v2.15 and firmware v1.16
	///
	/// Delete a named scan patterns.
	/// The default scan patterns can't be deleted.
	///
	/// @param name: Name of the scan pattern
	void delete_named_scan_pattern(std::string name);

	/// > Introduced in BSL v2.11 and firmware v1.11
	///
	/// Returns the currently set advanced config, see: protobuf_protocol.
	///
	/// @returns Currently set advanced config, see protobuf_protocol advanced config
	const protocol::config::Advanced get_advanced_config();

	/// > Introduced in BSL v2.11 and firmware v1.11
	///
	/// Function to set advanced config, see: protobuf_protocol AdvancedConfig.
	/// Expert parameters: It is not recommended to adapt this calibrated configuration without understanding the influences on the resulting point cloud quality.
	///
	/// @param config: advanced config to be set
	/// @param persist: Persist advanced config on device and reload it after a power-cycle
	void set_advanced_config(protocol::config::Advanced config, bool persist=false);

	/// > Introduced in BSL v2.18 and firmware v1.19
	///
	/// Function to set time synchronization
	/// The existing config will be overwritten.
	///
	/// @param config: Configuration, see protobuf_protocol advanced config
	/// @param persist: Persist advanced config on device and reload it after a power-cycle
	/// @param wait_for_sync: Wait until device is synchronized. Raises exception if device is not synchronized within max_sync_duration.
	/// @param max_sync_duration: Specify maximum time in seconds for synchronization.
	void set_time_synchronization(const protocol::config::Advanced::TimeSynchronization config, bool persist=true, bool wait_for_sync=true, int max_sync_duration=60);

	/// > Introduced in BSL v2.18 and firmware v1.19
	///
	/// Function to set ntp time synchronization, a vector of servers can be provided.
	/// The old servers and config will be overwritten.
	///
	/// @param servers: vector of servers to connect to for time synchronization
	/// @param persist: Persist advanced config on device and reload it after a power-cycle
	void set_ntp_time_synchronization(std::vector<std::string> servers={});

	/// > Introduced in BSL v2.18 and firmware v1.19
	///
	/// Function to set ptp time synchronization.
	/// A vector of unicast destinations can be provided, this will also activate unicast mode and deactivate multicast mode.
	/// If no destinations are given the standard configuration (multicast will be used).
	/// The old servers and config will be overwritten.
	///
	/// @param servers: vector of unicast destinations to connect to for time synchronization (if not given or vector is empty, multicast will be used)
	/// @param persist: Persist advanced config on device and reload it after a power-cycle
	void set_ptp_time_synchronization(std::vector<std::string> unicast_destinations={});

	/// Start self test on device
	const protocol::Response::RunSelfTest run_self_test();
#endif

};

}

std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::frame_t& frame);
#ifndef BSL_STANDALONE
std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Frame& frame);
std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Point& frame);
std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Point::Return& frame);
#endif
