/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include <asio.h>
#include "blickfeld/scanner-config.h"
#include "blickfeld/scanner.h"
#include "blickfeld/file/point_cloud.pb.h"

#ifdef BSL_RECORDING
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>
#include <google/protobuf/io/gzip_stream.h>

namespace blickfeld {

class streambuf_point_cloud_record;

class point_cloud_record {
	friend class streambuf_point_cloud_record;

	protocol::file::PointCloud_Metadata metadata;
	protocol::config::ScanPattern scan_pattern;

	std::ostream* ostream = nullptr;
	google::protobuf::io::OstreamOutputStream pb_ostream;
	google::protobuf::io::GzipOutputStream pb_ozstream;
	bool finished = false;

public:
	point_cloud_record(std::ostream* ostream, int compression_level = 0);
	virtual ~point_cloud_record();

	void start(const protocol::data::PointCloud::Header& header);
	void record_data(const protocol::file::PointCloud_Data* data);
	void finish();

	bool is_finished() {
		return finished;
	}

};

class streambuf_point_cloud_record {
	std::ostream out_stream;
	asio::streambuf out_buffer;
	point_cloud_record* record;

public:
	streambuf_point_cloud_record(int compression_level = 0);
	virtual ~streambuf_point_cloud_record();

	void start(const protocol::data::PointCloud::Header& header) {
		record->start(header);
	}

	void record_data(const protocol::file::PointCloud_Data* data) {
		record->record_data(data);
	}

	void finish();
	bool is_finished() {
		return record->finished;
	}

	size_t available();
	void consume(void* data);
};

}
#endif // #ifdef BSL_RECORDING
