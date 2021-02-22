/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include "blickfeld/scanner-config.h"

#include <memory>
#include <functional>
#include <iostream>

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
namespace stream {
class Subscribe_PointCloud;
}
}

class connection;
class point_cloud_record;

/**
 * Point Cloud Stream class for requesting and recording Blickfeld Point Clouds.
 *
 * Use scanner::get_point_cloud_stream or scanner::get_simple_point_cloud_stream for live streams.
 * Use the static methods scanner::file_point_cloud_stream or scanner::simple_file_point_cloud_stream for reading recorded point clouds.
 */
template<class frame_t>
class point_cloud_stream {
public:
/**
 * @brief Processor can be used to perform processing operations on the frames.
 *
 * The processor is called when a client requests a new point-cloud stream.
 * The provided protocol::stream::Subscribe::PointCloud can be used to react to on client requests.
 * The initial call should return a frame processor method, which is called on every frame, before it is passed to the client.
 */
	using processor_t = std::function<std::function<void (frame_t&)> (const protocol::stream::Subscribe_PointCloud)>;

private:
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

	std::streampos stream_start;
	void stream_init_and_skip(int offset = 0);
	int offset_data;
#endif

public:
	/**
	 * Internal use. Use scanner::get_point_cloud_stream or scanner::get_simple_point_cloud_stream.
	 *
	 * @param conn Scanner connection which should be used for stream.
	 * @param reference_frame Frame representing the desired data. To request a field, set it to any value (also in submessages). For a repeated field, add at least one element.
	 */
	point_cloud_stream(std::shared_ptr<connection> conn, const protocol::stream::Subscribe_PointCloud* extend_subscription = nullptr);
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

	/**
	 * Jumps to the first frame of the stream.
	 * Only works if point_cloud_stream has been initialized with an stream input.
	 */
	void jump_to_first_frame();
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

}
