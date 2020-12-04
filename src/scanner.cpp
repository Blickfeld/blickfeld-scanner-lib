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
#include "point_cloud_record.h"

#include <string>
#include <chrono>
#include <iostream>
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
scanner::point_cloud_stream<frame_t>::point_cloud_stream(std::shared_ptr<connection> conn, const protocol::config::ScanPattern_Filter* filter, const protocol::data::Frame* reference_frame, const protocol::stream::Subscribe::PointCloud* extend_subscription) :
	conn(conn),
	resp(new protocol::Response()),
	metadata(new protocol::file::PointCloud::Metadata()),
	frame(new frame_t())
{
	protocol::Request req;
	protocol::Response resp;
	req.mutable_subscribe()->mutable_point_cloud();
	if (filter)
		req.mutable_subscribe()->mutable_point_cloud()->mutable_filter()->CopyFrom(*filter);
	if (reference_frame)
		req.mutable_subscribe()->mutable_point_cloud()->mutable_reference_frame()->CopyFrom(*reference_frame);
	if (extend_subscription)
		req.mutable_subscribe()->mutable_point_cloud()->MergeFrom(*extend_subscription);
	conn->send_request(req, resp);
	metadata->mutable_header()->mutable_device()->CopyFrom(resp.event().point_cloud().header());
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
	if (record)
		delete record;
#endif
}

template<class frame_t>
const frame_t& scanner::point_cloud_stream<frame_t>::recv_frame() {
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
void scanner::point_cloud_stream<frame_t>::subscribe(subscribe_callback_t cb) {
	conn->subscribe(protocol::stream::Event::DataCase::kPointCloud, [this, cb](const protocol::stream::Event& event) {
			convert_point_cloud(event.point_cloud().frame(), *frame);
			cb(*frame);
		});
	conn->async_run();
}

#ifdef BSL_RECORDING

template<class frame_t>
void scanner::point_cloud_stream<frame_t>::record_to_stream(std::ostream* ostream, int compression_level) {
	stop_recording();

	this->ostream = ostream;
	record = new point_cloud_record(ostream, compression_level);
	record->start(metadata->header().device());
}

template<class frame_t>
void scanner::point_cloud_stream<frame_t>::stop_recording() {
	if (record) {
		delete record;
		record = nullptr;
		this->ostream->flush();;
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
scanner::scanner(std::string hostname_or_ip, std::string cert_key_file) :
	logged_object(hostname_or_ip),
	#ifndef BSL_STANDALONE
	REF_FRAME_XYZ([] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		ref.add_scanlines()->add_points()->add_returns()->add_cartesian(0);
		return ref;
	} ()),
	REF_FRAME_XYZ_I([] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		auto ref_return = ref.add_scanlines()->add_points()->add_returns();
		ref_return->add_cartesian(0);
		ref_return->set_intensity(0);
		return ref;
	} ()),
	REF_FRAME_XYZ_I_ID([] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		ref.set_id(0);
		auto ref_scanline = ref.add_scanlines();
		ref_scanline->set_id(0);
		auto ref_point = ref_scanline->add_points();
		ref_point->set_id(0);
		auto ref_return = ref_point->add_returns();
		ref_return->set_id(0);
		ref_return->add_cartesian(0.0f);
		ref_return->set_intensity(0.0f);
		return ref;
	} ()),
	REF_FRAME_XYZ_I_ID_TS([] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		ref.set_id(0);
		ref.set_start_time_ns(0);
		auto ref_scanline = ref.add_scanlines();
		ref_scanline->set_id(0);
		ref_scanline->set_start_offset_ns(0);
		auto ref_point = ref_scanline->add_points();
		ref_point->set_id(0);
		ref_point->set_start_offset_ns(0);
		auto ref_return = ref_point->add_returns();
		ref_return->set_id(0);
		ref_return->add_cartesian(0.0f);
		ref_return->set_intensity(0.0f);
		return ref;
	} ()),
	REF_FRAME_DEPTH_MAP([] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		ref.set_id(0);
		auto ref_scanline = ref.add_scanlines();
		ref_scanline->set_id(0);
		auto ref_point = ref_scanline->add_points();
		ref_point->set_id(0);
		ref_point->set_ambient_light_level(0.0f);
		auto ref_return = ref_point->add_returns();
		ref_return->set_intensity(0.0f);
		ref_return->set_range(0.0f);
		return ref;
	} ()),
	#endif
	io_context(new asio::io_context()),
	hostname_or_ip(hostname_or_ip) {
	if (cert_key_file != "") {
#ifdef HAVE_OPENSSL
		ssl_context = new asio::ssl::context(asio::ssl::context::sslv23_client);
		ssl_context->set_options(asio::ssl::context::default_workarounds|asio::ssl::context::no_sslv2|asio::ssl::context::single_dh_use);
		ssl_context->use_certificate_file(cert_key_file, asio::ssl::context::pem);
		ssl_context->use_private_key_file(cert_key_file, asio::ssl::context::pem);
		ssl_context->set_verify_mode(asio::ssl::verify_peer);
		ssl_context->load_verify_file(cert_key_file);
		conn = new scanner_connection(*io_context, hostname_or_ip, *ssl_context);
#else
		throw protocol_exception<protocol::Error::NotImplemented>();
#endif
	} else {
		conn = new scanner_connection(*io_context, hostname_or_ip);
	}

	protocol::Request req;
	protocol::Response resp;
	req.mutable_hello()->set_protocol_version(scanner_connection::protocol_version);
	conn->send_request(req, resp);
}

scanner::~scanner() {
	if(conn)
		delete conn;
#ifdef HAVE_OPENSSL
	if(ssl_context)
		delete ssl_context;
#endif
	if(io_context)
		delete io_context;
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

void scanner::attempt_error_recovery() {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_attempt_error_recovery();
	conn->send_request(req, resp);
}

#ifndef BSL_STANDALONE

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream() {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection());
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::data::Frame reference_frame) {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), nullptr, &reference_frame);
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::config::ScanPattern::Filter filter) {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &filter, nullptr);
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::config::ScanPattern::Filter filter, const protocol::data::Frame reference_frame) {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &filter, &reference_frame);
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::config::ScanPattern::Filter filter, const protocol::data::Frame reference_frame, const protocol::stream::Subscribe::PointCloud extend_subscription) {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &filter, &reference_frame, &extend_subscription);
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

const protocol::Response::RunSelfTest scanner::run_self_test() {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_run_self_test();
	conn->send_request(req, resp);
	return resp.run_self_test();
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

void scanner::set_scan_pattern(const std::string name, bool persist) {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_set_scan_pattern()->set_name(name);
	req.mutable_set_scan_pattern()->set_persist(persist);
	conn->send_request(req, resp);
}

std::list<protocol::config::NamedScanPattern> scanner::get_named_scan_patterns() {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_get_named_scan_patterns();
	conn->send_request(req, resp);
	std::list<protocol::config::NamedScanPattern> ret(resp.get_named_scan_patterns().configs().begin(), resp.get_named_scan_patterns().configs().end());
	return ret;
}

void scanner::store_named_scan_pattern(std::string name, const protocol::config::ScanPattern& config) {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_store_named_scan_pattern()->set_name(name);
	req.mutable_store_named_scan_pattern()->mutable_config()->MergeFrom(config);
	conn->send_request(req, resp);
}

void scanner::delete_named_scan_pattern(std::string name) {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_delete_named_scan_pattern()->set_name(name);
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

}

std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::frame_t& frame) {
	return strm << "<Blickfeld Frame " << frame.id << ">";
}

#ifndef BSL_STANDALONE
std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Frame& frame) {
	return strm << "<Blickfeld Frame " << frame.id() << ": " << frame.total_number_of_returns() << " returns, "
		    << setprecision(1) << fixed << frame.scan_pattern().horizontal().fov() * 180.0f / M_PI << "x" << frame.scan_pattern().vertical().fov() * 180.0f / M_PI << " FoV, "
		    << setprecision(0) << fixed << frame.scanlines_size() << " scanlines>";
}

std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Point& point) {
	return strm << "<Blickfeld Point " << point.id() << ": " << point.returns_size() << (point.returns_size() == 1 ? " return>" : " returns>");
}

std::ostream& operator<<(std::ostream &strm, const blickfeld::protocol::data::Point::Return& ret) {
	return strm << "<Blickfeld Point::Return " << ret.id() << ": "
		    << setprecision(2) << fixed
		    << "(x: " << ret.cartesian(0)
		    << ", y: " << ret.cartesian(1)
		    << ", z: " << ret.cartesian(0)
		    << ") [m], "
		    << setprecision(0) << "intensity of " << ret.intensity() << ">";
}

#endif
