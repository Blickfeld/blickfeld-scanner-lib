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

#ifndef BSL_STANDALONE
#include "blickfeld/connection.pb.h"
#include "blickfeld/file/point_cloud.pb.h"
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

namespace google {
namespace protobuf {
namespace io {
class IstreamInputStream;
class GzipInputStream;
class CodedInputStream;
}
}
}

namespace blickfeld {

namespace protocol {
class Response;
namespace data {
class Frame;
}
namespace file {
class PointCloud_Metadata;
class PointCloud_Data;
}
namespace config {
class ScanPattern_Filter;
}
}

class connection;
class point_cloud_record;

/**
 * Blickfeld Scanner class for point cloud streams, status, and configuration requests.
 */
class scanner : public logged_object {
public:

#ifndef BSL_STANDALONE
	const protocol::data::Frame REF_FRAME_XYZ; // Reference frame: XYZ coordinates
	const protocol::data::Frame REF_FRAME_XYZ_I; // Reference frame: XYZ coordinates, intensity
	const protocol::data::Frame REF_FRAME_XYZ_I_ID; // Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id
	const protocol::data::Frame REF_FRAME_XYZ_I_ID_TS; // Reference frame: XYZ coordinates, intensity, frame id, scanline id, point id, return id, timestamps
	const protocol::data::Frame REF_FRAME_DEPTH_MAP; // Reference frame: ambient_light_level, intensity, range, frame id, scanline id, point id
#endif

	/**
	 * Point Cloud Stream class for requesting and recording Blickfeld Point Clouds.
	 *
	 * Use scanner::get_point_cloud_stream or scanner::get_simple_point_cloud_stream for live streams.
	 * Use the static methods scanner::file_point_cloud_stream or scanner::simple_file_point_cloud_stream for reading recorded point clouds.
	 */
	template<class frame_t>
	class point_cloud_stream {
		std::shared_ptr<connection> conn;
		protocol::Response* resp = nullptr;
		protocol::file::PointCloud_Metadata* metadata = nullptr;
		frame_t* frame = nullptr;

#ifdef BSL_RECORDING
		protocol::file::PointCloud_Data* stream_data = nullptr;
		bool stream_buffered = false;
		void read_stream();

		std::istream* istream = nullptr;
		google::protobuf::io::IstreamInputStream* pb_istream = nullptr;
		google::protobuf::io::GzipInputStream* pb_izstream = nullptr;
		google::protobuf::io::CodedInputStream* pb_icstream = nullptr;
		std::ostream* ostream = nullptr;
		point_cloud_record* record = nullptr;
#endif

public:
		/**
		 * Internal use. Use scanner::get_point_cloud_stream or scanner::get_simple_point_cloud_stream.
		 *
		 * @param conn Scanner connection which should be used for stream.
		 * @param filter Filter points and returns by point attributes during the post-processing on the device.
		 * @param reference_frame Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
		 */
		point_cloud_stream(std::shared_ptr<connection> conn, const protocol::config::ScanPattern_Filter* filter = nullptr, const protocol::data::Frame* reference_frame = nullptr);
#ifdef BSL_RECORDING
		/**
		 * Internal use. Use the static methods scanner::file_point_cloud_stream or scanner::simple_file_point_cloud_stream.
		 *
		 * @param istream Input stream with Point Cloud Recording. The file format is described in the technical documentation: "Blickfeld Scanner Library : File Format".
		 */
		point_cloud_stream(std::istream* istream);
#endif
		virtual ~point_cloud_stream();

#ifdef BSL_RECORDING
		/**
		 * Records the frames received with recv_frame to the provided ostream.
		 * The ostream must be kept in scope until stop_recording is called.
		 *
		 * @param ostream Output stream for point cloud file. The file format is described in the technical documentation: "Blickfeld Scanner Library : File Format".
		 */
		void record_to_stream(std::ostream* ostream, int compression_level = 1);

		/**
		 * Stops the recording, which was started with record_to_stream.
		 * Flushes the provided ostream.
		 */
		void stop_recording();

		/**
		 * Check if end of stream has been reached.
		 * Only works if point_cloud_stream has been initialized with an stream input.
		 *
		 * @return False if further frames can be fetch with recv_frame. True if the end of stream has been reached.
		 */
		bool end_of_stream();
#endif

		/**
		 * Receive frame from device or file.
		 * Blocks until frame next frame is available.
		 *
		 * @return Frame Point Cloud Frame.
		 */
		const frame_t& recv_frame();

		/** Frame subscription callback for asynchronous subscribes with subscribe. **/
		using subscribe_callback_t = std::function<void (const frame_t&)>;

		/**
		 * Asynchronously subscribe to frames.
		 * @param cb Callback handler for frames.
		 */
		void subscribe(subscribe_callback_t cb);

#ifndef BSL_STANDALONE
		/**
		 * Return metadata of the current stream.
		 *
		 * @return Metadata The metadata format is described in the technical documentation: "Blickfeld Scanner Library : File Format".
		 */
		const protocol::file::PointCloud_Metadata get_metadata();
#endif
	};

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
	 * > Introduced in BSL v2.13 and firmware v1.3
	 *
	 * Can be used to attempt a re-initialization of the device if it is errored.
	 * A self test is automatically triggered after a successful re-initialization.
	 */
	void attempt_error_recovery();

#ifndef BSL_STANDALONE
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

	/// Use this function to get the current <a href="../protobuf_protocol.html#blickfeld/config/scan_pattern.proto">ScanPattern</a>
	const protocol::config::ScanPattern get_scan_pattern();

	/// If you want to set a new ScanPattern, first call this function with the new ScanPattern and then pass the returned ScanPattern to
	/// set_scan_pattern(const protocol::config::ScanPattern& config)
	///
	/// @param config ScanPattern to fill by the BSS
	/// @returns A filled ScanPattern to send via set_scan_pattern(const protocol::config::ScanPattern& config)
	const protocol::config::ScanPattern fill_scan_pattern(const protocol::config::ScanPattern& config);

	/// Used for setting a ScanPattern
	///
	/// @warning First fill the ScanPattern with fill_scan_pattern(const protocol::config::ScanPattern& config)
	void set_scan_pattern(const protocol::config::ScanPattern& config, bool persist = false);

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
