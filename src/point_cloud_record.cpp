/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "point_cloud_record.h"

#ifdef BSL_RECORDING
namespace blickfeld {

point_cloud_record::point_cloud_record(std::ostream* ostream, int compression_level)
	: ostream(ostream), pb_ostream(ostream), pb_ozstream(&pb_ostream, [compression_level]() {
		struct google::protobuf::io::GzipOutputStream::Options options;
		options.compression_level = compression_level;
		return options;
	} ()) {
}

point_cloud_record::~point_cloud_record() {
	finish();
}

void point_cloud_record::start(const protocol::data::PointCloud::Header& header) {
	metadata.mutable_header()->mutable_device()->CopyFrom(header);

	// Write client header
	auto client_header = metadata.mutable_header()->mutable_client();
	client_header->set_library_version(BSL_VERSION);
	client_header->set_file_time_ns(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	client_header->set_language(protocol::Language::CPP);

	// Write header
	google::protobuf::io::CodedOutputStream pb_ocstream(&pb_ozstream);
	pb_ocstream.WriteVarint32(metadata.header().ByteSizeLong());
	metadata.header().SerializeWithCachedSizes(&pb_ocstream);
}

void point_cloud_record::record_data(const protocol::file::PointCloud_Data* stream_data) {
	google::protobuf::io::CodedOutputStream pb_ocstream(&pb_ozstream);
	pb_ocstream.WriteVarint32(stream_data->ByteSizeLong());
	stream_data->SerializeWithCachedSizes(&pb_ocstream);

	// Add events for changed scan pattern to footer
	if (!google::protobuf::util::MessageDifferencer::ApproximatelyEquals(scan_pattern, stream_data->frame().scan_pattern())) {
		auto event = metadata.mutable_footer()->add_events();
		event->set_from_frame_id(stream_data->frame().id());
		event->mutable_scan_pattern()->CopyFrom(stream_data->frame().scan_pattern());

		scan_pattern.CopyFrom(stream_data->frame().scan_pattern());
	}

	// Update stats in footer
	auto mut_counter = metadata.mutable_footer()->mutable_stats()->mutable_counter();
	mut_counter->set_frames(mut_counter->frames() + 1);
	mut_counter->set_points(mut_counter->points() + stream_data->frame().total_number_of_points());
	mut_counter->set_returns(mut_counter->returns() + stream_data->frame().total_number_of_returns());
}

void point_cloud_record::finish() {
	if (finished)
		return;

	finished = true;

	google::protobuf::io::CodedOutputStream pb_ocstream(&pb_ozstream);
	protocol::file::PointCloud_Data stream_data;
	stream_data.mutable_footer()->CopyFrom(metadata.footer());
	pb_ocstream.WriteVarint32(stream_data.ByteSizeLong());
	stream_data.SerializeWithCachedSizes(&pb_ocstream);
}

streambuf_point_cloud_record::streambuf_point_cloud_record(int compression_level)
	: out_stream(&out_buffer), record(new point_cloud_record(&out_stream, compression_level)) {
}

streambuf_point_cloud_record::~streambuf_point_cloud_record() {
	finish();
}

void streambuf_point_cloud_record::finish() {
	if (!record || record->finished)
		return;
	record->finish();
	delete record;
	record = nullptr;
}

size_t streambuf_point_cloud_record::available() {
	return out_buffer.data().size();
}

void streambuf_point_cloud_record::consume(void* data) {
	memcpy(data, out_buffer.data().data(), out_buffer.data().size());
	out_buffer.consume(out_buffer.data().size());
}

} // namespace blickfeld
#endif // #ifdef BSL_RECORDING
