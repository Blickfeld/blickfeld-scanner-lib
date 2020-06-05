/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include "asio.h"
#include "blickfeld/connection.pb.h"
#include "blickfeld/scanner.h"
#include "blickfeld/utils.h"
#include "blickfeld/exception.h"
#include "blickfeld/file/point_cloud.pb.h"
#include "blickfeld/protocol_exception.h"

#include "scanner_connection.h"

#include <string>
#include <chrono>
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

	frame_o.points.resize(frame_i.total_number_of_returns());

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

void convert_point_cloud(const protocol::data::Frame& frame_i, protocol::data::Frame& frame_o) {
	frame_o = frame_i;
}

template<class frame_t>
scanner::point_cloud_stream<frame_t>::point_cloud_stream(std::shared_ptr<connection> conn) :
	conn(conn),
	resp(new protocol::Response()),
	metadata(new protocol::file::PointCloud::Metadata()),
	frame(new frame_t())
{
	protocol::Request req;
	protocol::Response resp;
	req.mutable_subscribe()->mutable_point_cloud();
	conn->send_request(req, resp);
	metadata->mutable_header()->mutable_device()->CopyFrom(resp.event().point_cloud().header());

	// Write client header
	auto client_header = metadata->mutable_header()->mutable_client();
	client_header->set_library_version(BSL_VERSION);
	client_header->set_file_time_ns(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
	client_header->set_language(protocol::file::Client::CPP);
}

#ifdef BSL_RECORDING
template<class frame_t>
scanner::point_cloud_stream<frame_t>::point_cloud_stream(std::istream* istream) :
	metadata(new protocol::file::PointCloud::Metadata()),
	frame(new frame_t()),
	stream_data(new protocol::file::PointCloud::Data()),
	istream(istream) {
	// Helper methods
	auto stream_start = istream->tellg();
	auto stream_init_and_skip = [this, stream_start] (int offset = 0) {
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
		};

	// Read header
	stream_init_and_skip();
	auto limit = pb_icstream->ReadLengthAndPushLimit();
	metadata->mutable_header()->MergeFromCodedStream(pb_icstream);
	if (!pb_icstream->ConsumedEntireMessage())
		throw str_exception("Failed to parse input. Stream may be corrupted.");
	pb_icstream->PopLimit(limit);

	// Get offset of footer
	int offset_data = pb_icstream->CurrentPosition();
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

#endif

template<class frame_t>
scanner::point_cloud_stream<frame_t>::~point_cloud_stream() {
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
	if (pb_icstream)
		delete pb_icstream;
	if (pb_izstream)
		delete pb_izstream;
	if (pb_istream)
		delete pb_istream;
#endif
}

template<class frame_t>
const frame_t& scanner::point_cloud_stream<frame_t>::recv_frame() {
	if (conn) {
		// Previous frame
		auto frame_prev = (static_cast<protocol::Response&>(*resp)).event().point_cloud().frame();

		// Receive and extract frame
		conn->recv(*resp);
		auto frame_i = (static_cast<protocol::Response&>(*resp)).event().point_cloud().frame();

#ifdef BSL_RECORDING
		// Record frame to stream
		if (pb_ocstream) {
			auto data = protocol::file::PointCloud_Data();
			data.mutable_frame()->CopyFrom(frame_i);
			pb_ocstream->WriteVarint32(static_cast<unsigned int>(data.ByteSizeLong()));
			data.SerializeWithCachedSizes(pb_ocstream);

			// Add events for changed scan pattern to footer
			if (!google::protobuf::util::MessageDifferencer::ApproximatelyEquals(frame_prev.scan_pattern(), frame_i.scan_pattern())) {
				auto event = metadata->mutable_footer()->add_events();
				event->set_from_frame_id(frame_i.id());
				event->mutable_scan_pattern()->CopyFrom(frame_i.scan_pattern());
			}

			// Update stats in footer
			auto mut_counter = metadata->mutable_footer()->mutable_stats()->mutable_counter();
			mut_counter->set_frames(mut_counter->frames() + 1);
			mut_counter->set_points(mut_counter->points() + frame_i.total_number_of_points());
			mut_counter->set_returns(mut_counter->returns() + frame_i.total_number_of_returns());
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
void scanner::point_cloud_stream<frame_t>::subscribe(subscribe_callback_t cb) {
	conn->subscribe(protocol::stream::Event::DataCase::kPointCloud, [this, cb](const protocol::stream::Event& event) {
			convert_point_cloud(event.point_cloud().frame(), *frame);
			cb(*frame);
		});
	conn->async_run();
}

#ifdef BSL_RECORDING

template<class frame_t>
void scanner::point_cloud_stream<frame_t>::record_to_stream(std::ostream* ostream) {
	if (pb_ocstream) {
		delete pb_ocstream;
		delete pb_ozstream;
		delete pb_ostream;
		this->ostream->flush();
	}

	this->ostream = ostream;
	pb_ostream = new google::protobuf::io::OstreamOutputStream(ostream);
	pb_ozstream = new google::protobuf::io::GzipOutputStream(pb_ostream);
	pb_ocstream = new google::protobuf::io::CodedOutputStream(pb_ozstream);

	pb_ocstream->WriteVarint32(metadata->header().ByteSizeLong());
	metadata->header().SerializeWithCachedSizes(pb_ocstream);
}

template<class frame_t>
void scanner::point_cloud_stream<frame_t>::stop_recording() {
	if (pb_ocstream) {
		auto data = protocol::file::PointCloud_Data();
		data.mutable_footer()->CopyFrom(metadata->footer());
		pb_ocstream->WriteVarint32(static_cast<unsigned int>(data.ByteSizeLong()));
		data.SerializeWithCachedSizes(pb_ocstream);

		delete pb_ocstream;
		delete pb_ozstream;
		delete pb_ostream;
		ostream->flush();
		metadata->mutable_footer()->Clear();
	}
}

template<class frame_t>
bool scanner::point_cloud_stream<frame_t>::end_of_stream() {
	if (conn)
		throw str_exception("Cannot use end_of_stream() with a live connection.");

	if (!stream_buffered) {
		read_stream();
	}

	return !stream_buffered || stream_data->has_footer();
}

template<class frame_t>
void scanner::point_cloud_stream<frame_t>::read_stream() {
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
const protocol::file::PointCloud_Metadata scanner::point_cloud_stream<frame_t>::get_metadata() {
	return *metadata;
}

#endif

// Explicit template instantiation
template class scanner::point_cloud_stream<protocol::data::frame_t>;
template class scanner::point_cloud_stream<protocol::data::Frame>;

// SCANNER
scanner::scanner(string hostname_or_ip) :
	logged_object(hostname_or_ip),
	hostname_or_ip(hostname_or_ip) {
	io_context = new asio::io_context();
	conn = new scanner_connection(*io_context, hostname_or_ip);

	protocol::Request req;
	protocol::Response resp;
	req.mutable_hello()->set_protocol_version(scanner_connection::protocol_version);
	conn->send_request(req, resp);
}

#ifdef HAVE_OPENSSL
scanner::scanner(std::string hostname_or_ip, std::string cert_key_file) :
	logged_object(hostname_or_ip),
	hostname_or_ip(hostname_or_ip)
{
	io_context = new asio::io_context();
	ssl_context = new asio::ssl::context(asio::ssl::context::sslv23_client);
	ssl_context->set_options(asio::ssl::context::default_workarounds|asio::ssl::context::no_sslv2|asio::ssl::context::single_dh_use);
	ssl_context->use_certificate_file(cert_key_file, asio::ssl::context::pem);
	ssl_context->use_private_key_file(cert_key_file, asio::ssl::context::pem);
	ssl_context->set_verify_mode(asio::ssl::verify_peer);
	ssl_context->load_verify_file(cert_key_file);
	conn = new scanner_connection(*io_context, hostname_or_ip, *ssl_context);
	protocol::Request req;
	protocol::Response resp;
	req.mutable_hello()->set_protocol_version(scanner_connection::protocol_version);
	conn->send_request(req, resp);
}

#endif

scanner::~scanner() {
	if(conn)
		delete conn;
	if(io_context)
		delete io_context;
#ifdef HAVE_OPENSSL
	if(ssl_context)
		delete ssl_context;
#endif
}

std::shared_ptr<scanner> scanner::connect(string hostname_or_ip) {
	return std::shared_ptr<scanner>(new scanner(hostname_or_ip));
}

#ifdef HAVE_OPENSSL
std::shared_ptr<scanner> scanner::connect(std::string hostname_or_ip, std::string cert_key_file)
{
	return std::shared_ptr<scanner>(new scanner(hostname_or_ip, cert_key_file));
}

#endif

const std::string scanner::name() {
	return hostname_or_ip;
}

void scanner::async_run_blocking() {
	asio::make_work_guard(io_context);
	io_context->run();
}

std::shared_ptr<std::thread> scanner::async_run_thread() {
	return std::make_shared<std::thread>([this]() {
			async_run_blocking();
		});
}

std::shared_ptr<connection> scanner::create_connection() {
#ifndef HAVE_OPENSSL
	return std::make_shared<scanner_connection>(*io_context, hostname_or_ip);
#else
	if(ssl_context) {
		return std::make_shared<scanner_connection>(*io_context, hostname_or_ip, *ssl_context);
	} else {
		return std::make_shared<scanner_connection>(*io_context, hostname_or_ip);
	}
#endif
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::frame_t> > scanner::get_simple_point_cloud_stream() {
	return std::make_shared<point_cloud_stream<protocol::data::frame_t> >(create_connection());
}

#ifdef BSL_RECORDING
std::shared_ptr<scanner::point_cloud_stream<protocol::data::frame_t> > scanner::simple_file_point_cloud_stream(std::istream* istream) {
	return std::make_shared<point_cloud_stream<protocol::data::frame_t> >(istream);
}

#endif

#ifndef BSL_STANDALONE

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream() {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection());
}

#ifdef BSL_RECORDING
std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::file_point_cloud_stream(std::istream* istream) {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(istream);
}

#endif

const protocol::Status scanner::get_status() {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_status();
	conn->send_request(req, resp);
	return resp.status();
}

const protocol::config::ScanPattern scanner::get_scan_pattern() {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_get_scan_pattern();
	conn->send_request(req, resp);
	return resp.get_scan_pattern().config();
}

const protocol::config::ScanPattern scanner::fill_scan_pattern(const protocol::config::ScanPattern& config) {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_fill_scan_pattern()->mutable_config()->MergeFrom(config);
	conn->send_request(req, resp);
	return resp.fill_scan_pattern().config();
}

void scanner::set_scan_pattern(const protocol::config::ScanPattern& config, bool persist) {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_set_scan_pattern()->mutable_config()->MergeFrom(config);
	req.mutable_set_scan_pattern()->set_persist(persist);
	conn->send_request(req, resp);
}

void scanner::subscribe(subscribe_status_callback_t cb) {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_subscribe()->mutable_status();
	conn->send_request(req, resp);

	conn->subscribe(protocol::stream::Event::DataCase::kStatus, [cb](const protocol::stream::Event& event) {
			cb(event.status());
		});
	conn->async_run();
}

#endif

std::ostream& operator<<(std::ostream &strm, const protocol::data::frame_t& frame) {
	return strm << "<Blickfeld Frame " << frame.id << ">";
}

#ifndef BSL_STANDALONE
std::ostream& operator<<(std::ostream &strm, const protocol::data::Frame& frame) {
	return strm << "<Blickfeld Frame " << frame.id() << ">";
}

#endif

}
