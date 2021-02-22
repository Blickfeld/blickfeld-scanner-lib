/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#include "blickfeld/point_cloud_stream.h"
#include "blickfeld/connection.pb.h"
#include "blickfeld/utils.h"
#include "blickfeld/exception.h"
#include "blickfeld/error.custom.pb.h"
#include "point_cloud_record.h"
#include "connection.h"
#include "portable_endian.h"

#include <string>
#include <chrono>
#include <iomanip>
#include <math.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/util/message_differencer.h>
#ifdef BSL_RECORDING
#include <google/protobuf/io/gzip_stream.h>
#endif

using namespace std;

namespace blickfeld {

void convert_point_cloud(const protocol::data::Frame& frame_i, protocol::data::frame_t& frame_o) {
	frame_o.id = frame_i.id();
	frame_o.frame_rate = static_cast<float>(frame_i.scan_pattern().frame_rate().maximum());
	frame_o.start_time_ns = frame_i.start_time_ns();

	// Pre-allocate
	frame_o.points.resize(frame_i.has_packed() ? frame_i.packed().length() : frame_i.total_number_of_returns());

	// Either use packed format or scanlines structure to fill simple frame
	if (frame_i.has_packed()) {
		for (uint32_t i = 0; i < frame_i.packed().length(); i++) {
			if (frame_i.packed().has_point_id())
				frame_o.points[i].id = be32toh(((uint32_t*)frame_i.packed().point_id().data())[i]);
			if (frame_i.packed().has_cartesian()) {
				frame_o.points[i].x = os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 3 + 0]);
				frame_o.points[i].y = os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 3 + 1]);
				frame_o.points[i].z = os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 3 + 2]);
			}
			if (frame_i.packed().has_range())
				frame_o.points[i].range = os::be32tofloat(((uint32_t*)frame_i.packed().range().data())[i]);
			if (frame_i.packed().has_intensity())
				frame_o.points[i].intensity = be32toh(((uint32_t*)frame_i.packed().intensity().data())[i]);
			if (frame_i.packed().has_ambient_light_level())
				frame_o.points[i].ambient_light_level = be32toh(((uint32_t*)frame_i.packed().ambient_light_level().data())[i]);
		}
	} else {
		size_t i = 0;
		for (int s_ind = 0; s_ind < frame_i.scanlines_size(); s_ind++) {
			for (int p_ind = 0; p_ind < frame_i.scanlines(s_ind).points_size(); p_ind++) {
				auto& point = frame_i.scanlines(s_ind).points(p_ind);
				for (int r_ind = 0; r_ind < point.returns_size(); r_ind++) {
					auto& ret = point.returns(r_ind);
					frame_o.points[i].id = static_cast<unsigned int>(point.id());
					frame_o.points[i].x = ret.cartesian(0);
					frame_o.points[i].y = ret.cartesian(1);
					frame_o.points[i].z = ret.cartesian(2);
					frame_o.points[i].range = ret.range();
					frame_o.points[i].intensity = static_cast<uint16_t>(ret.intensity());
					frame_o.points[i].ambient_light_level = static_cast<uint16_t>(point.ambient_light_level());

					i++;
				}
			}
		}
	}
}

void convert_point_cloud(const protocol::data::Frame& frame_i, protocol::data::Frame& frame_o) {
	frame_o = frame_i;

	// If frame contains packed data, fill normal structure with information.
	if (frame_i.has_packed()) {
		protocol::data::Scanline* scanline = nullptr;
		protocol::data::Point* point = nullptr;

		if (!frame_i.packed().has_point_id()) {
			scanline = frame_o.add_scanlines();
			point = scanline->add_points();
		}

		for (uint32_t i = 0; i < frame_i.total_number_of_returns(); i++) {
			if (frame_i.packed().has_point_id()) {
				uint32_t id = be32toh(((uint32_t*)frame_i.packed().point_id().data())[i]);
				uint32_t scanline_id = protocol::data::get_scanline_id_by_point_id(frame_i, id);

				if (scanline == nullptr || scanline_id != scanline->id()) {
					scanline = frame_o.add_scanlines();
					scanline->set_id(scanline_id);
					point = nullptr;
				}
				if (point == nullptr || id != point->id()) {
					point = scanline->add_points();
					point->set_id(id);

					if (frame_i.packed().has_direction()) {
						point->mutable_direction()->set_azimuth(os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 2 + 0]));
						point->mutable_direction()->set_elevation(os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 2 + 1]));
					}
					if (frame_i.packed().has_ambient_light_level())
						point->set_ambient_light_level(be32toh(((uint32_t*)frame_i.packed().ambient_light_level().data())[i]));
					if (frame_i.packed().has_start_offset_ns())
						point->set_start_offset_ns(be64toh(((uint64_t*)frame_i.packed().start_offset_ns().data())[i]));
					if (frame_i.packed().has_channel_id())
						point->set_channel_id(((uint8_t*)frame_i.packed().channel_id().data())[i]);
				}
			}

			auto ret = point->add_returns();
			if (frame_i.packed().has_return_id())
				ret->set_id(((uint8_t*)frame_i.packed().return_id().data())[i]);

			if (frame_i.packed().has_cartesian()) {
				ret->mutable_cartesian()->Add(os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 3 + 0]));
				ret->mutable_cartesian()->Add(os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 3 + 1]));
				ret->mutable_cartesian()->Add(os::be32tofloat(((uint32_t*)frame_i.packed().cartesian().data())[i * 3 + 2]));
			}
			if (frame_i.packed().has_range())
				ret->set_range(os::be32tofloat(((uint32_t*)frame_i.packed().range().data())[i]));
			if (frame_i.packed().has_intensity())
				ret->set_intensity(be32toh(((uint32_t*)frame_i.packed().intensity().data())[i]));

		}

		// Clear packed structure
		frame_o.clear_packed();
	}
}

template<class frame_t>
point_cloud_stream<frame_t>::point_cloud_stream(std::shared_ptr<connection> conn, const protocol::stream::Subscribe_PointCloud* extend_subscription) :
	conn(conn),
	resp(new protocol::Response()),
	metadata(new protocol::file::PointCloud::Metadata()),
	frame(new frame_t())
{
	protocol::Request req;
	protocol::Response resp;
	req.mutable_subscribe()->mutable_point_cloud();
	if (extend_subscription)
		req.mutable_subscribe()->mutable_point_cloud()->MergeFrom(*extend_subscription);
	conn->send_request(req, resp);
	metadata->mutable_header()->mutable_device()->CopyFrom(resp.event().point_cloud().header());
}

#ifdef BSL_RECORDING
template<class frame_t>
point_cloud_stream<frame_t>::point_cloud_stream(std::istream* istream) :
	metadata(new protocol::file::PointCloud::Metadata()),
	frame(new frame_t()),
	stream_data(new protocol::file::PointCloud::Data()),
	istream(istream) {
	if(!istream->good())
		throw str_exception("Input file stream is not valid. Failed to open stream.");

	// Helper methods
	stream_start = istream->tellg();

	// Read header
	stream_init_and_skip();
	auto limit = pb_icstream->ReadLengthAndPushLimit();
	metadata->mutable_header()->MergeFromCodedStream(pb_icstream);
	if (!pb_icstream->ConsumedEntireMessage())
		throw str_exception("Failed to parse input. Stream may be corrupted.");
	pb_icstream->PopLimit(limit);

	// Get offset of footer
	offset_data = pb_icstream->CurrentPosition();
	int offset_footer = offset_data;
	int offset_d = offset_footer;

	limit = pb_icstream->ReadLengthAndPushLimit();
	while (pb_icstream->BytesUntilLimit() > 0) {
		pb_icstream->Skip(pb_icstream->BytesUntilLimit());
		pb_icstream->PopLimit(limit);
		offset_footer = offset_d;
		offset_d = pb_icstream->CurrentPosition();
		limit = pb_icstream->ReadLengthAndPushLimit();
	}

	// Read footer
	stream_init_and_skip(offset_footer);
	limit = pb_icstream->ReadLengthAndPushLimit();
	stream_data->MergeFromCodedStream(pb_icstream);
	if (pb_icstream->ConsumedEntireMessage() && stream_data->has_footer()) {
		metadata->mutable_footer()->CopyFrom(stream_data->footer());
	}
	pb_icstream->PopLimit(limit);

	// Go back to data
	stream_init_and_skip(offset_data);
}

template<class frame_t>
void point_cloud_stream<frame_t>::stream_init_and_skip(int offset) {
	if (pb_icstream)
		delete pb_icstream;
	if (pb_istream)
		delete pb_istream;
	this->istream->clear();
	this->istream->seekg(stream_start);
	pb_istream = new google::protobuf::io::IstreamInputStream(this->istream);
	pb_izstream = new google::protobuf::io::GzipInputStream(pb_istream);
	pb_icstream = new google::protobuf::io::CodedInputStream(pb_izstream);
	pb_icstream->Skip(offset);
}

#endif

template<class frame_t>
point_cloud_stream<frame_t>::~point_cloud_stream() {
#ifdef BSL_RECORDING
	stop_recording();
#endif
	if (resp)
		delete resp;
	if (metadata)
		delete metadata;
	if (frame)
		delete frame;
#ifdef BSL_RECORDING
	if (stream_data)
		delete stream_data;
	if (record)
		delete record;
#endif
}

template<class frame_t>
const frame_t& point_cloud_stream<frame_t>::recv_frame() {
	if (conn) {
		// Receive and extract frame
		conn->recv(*resp);
		auto frame_i = (static_cast<protocol::Response&>(*resp)).event().point_cloud().frame();

#ifdef BSL_RECORDING
		if (record) {
			protocol::file::PointCloud_Data data;
			data.mutable_frame()->CopyFrom(frame_i);
			record->record_data(&data);
		}
#endif

		// Convert point cloud to requested format
		convert_point_cloud(frame_i, *frame);
	} else {
#ifdef BSL_RECORDING
		if (end_of_stream())
			throw str_exception("Reached end of stream. Use end_of_stream() before calling recv_frame().");
		stream_buffered = false;

		convert_point_cloud(stream_data->frame(), *frame);
#endif
	}
	return *frame;
}

template<class frame_t>
void point_cloud_stream<frame_t>::subscribe(subscribe_callback_t cb) {
	conn->subscribe(protocol::stream::Event::DataCase::kPointCloud, [this, cb](const protocol::stream::Event& event) {
			convert_point_cloud(event.point_cloud().frame(), *frame);
			cb(*frame);
		});
	conn->async_run();
}

#ifdef BSL_RECORDING

template<class frame_t>
void point_cloud_stream<frame_t>::record_to_stream(std::ostream* ostream, int compression_level) {
	stop_recording();

	this->ostream = ostream;
	record = new point_cloud_record(ostream, compression_level);
	record->start(metadata->header().device());
}

template<class frame_t>
void point_cloud_stream<frame_t>::stop_recording() {
	if (record) {
		delete record;
		record = nullptr;
		this->ostream->flush();;
		metadata->mutable_footer()->Clear();
	}
}

template<class frame_t>
bool point_cloud_stream<frame_t>::end_of_stream() {
	if (conn)
		throw str_exception("Cannot use end_of_stream() with a live connection.");

	if (!stream_buffered) {
		read_stream();
	}

	return !stream_buffered || stream_data->has_footer();
}

template<class frame_t>
void point_cloud_stream<frame_t>::jump_to_first_frame() {
	if (conn)
		throw str_exception("Cannot use jump_to_first_frame() with a live connection.");

	stream_init_and_skip(offset_data);
	read_stream();
}

template<class frame_t>
void point_cloud_stream<frame_t>::read_stream() {
	auto limit = pb_icstream->ReadLengthAndPushLimit();
	if (!pb_icstream->BytesUntilLimit()) {
		stream_buffered = false;
		return;
	}

	stream_data->Clear();
	stream_data->MergeFromCodedStream(pb_icstream);
	if (!pb_icstream->ConsumedEntireMessage())
		throw str_exception("Failed to parse input. Stream may be corrupted.");
	pb_icstream->PopLimit(limit);

	stream_buffered = true;
}

#endif

#ifndef BSL_STANDALONE

template<class frame_t>
const protocol::file::PointCloud_Metadata point_cloud_stream<frame_t>::get_metadata() {
	return *metadata;
}

#endif

// Explicit template instantiation
template class point_cloud_stream<protocol::data::frame_t>;
template class point_cloud_stream<protocol::data::Frame>;

} // namespace blickfeld
