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

#include <chrono>
#include <iomanip>
#include <math.h>

using namespace std;

namespace blickfeld {

#ifndef BSL_STANDALONE

const blickfeld::protocol::data::Frame scanner::REF_FRAME_XYZ = [] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		ref.add_scanlines()->add_points()->add_returns()->add_cartesian(0);
		return ref;
	} ();

const blickfeld::protocol::data::Frame scanner::REF_FRAME_XYZ_I = [] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		auto ref_return = ref.add_scanlines()->add_points()->add_returns();
		ref_return->add_cartesian(0);
		ref_return->set_intensity(0);
		return ref;
	} ();

const blickfeld::protocol::data::Frame scanner::REF_FRAME_XYZ_I_ID = [] {
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
	} ();

const blickfeld::protocol::data::Frame scanner::REF_FRAME_XYZ_I_ID_TS = [] {
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
	} ();

const blickfeld::protocol::data::Frame scanner::REF_FRAME_FULL = [] {
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
		ref_point->set_ambient_light_level (0.0f);
		ref_point->set_start_offset_ns(0);
		ref_point->mutable_direction()->set_azimuth(0.0f);
		ref_point->mutable_direction()->set_elevation(0.0f);
		auto ref_return = ref_point->add_returns();
		ref_return->set_id(0);
		ref_return->add_cartesian(0.0f);
		ref_return->set_range(0.0f);
		ref_return->set_intensity(0.0f);
		return ref;
	} ();

const blickfeld::protocol::data::Frame scanner::REF_FRAME_DEPTH_MAP = [] {
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
	} ();

const blickfeld::protocol::data::Frame scanner::REF_FRAME_PACKED = [] {
		protocol::data::Frame ref;
		ref.mutable_scan_pattern();
		ref.set_total_number_of_points(0);
		ref.set_total_number_of_returns(0);
		ref.set_id(0);
		ref.set_start_time_ns(0);
		auto ref_scanline = ref.add_scanlines();
		auto ref_point = ref_scanline->add_points();
		auto ref_return = ref_point->add_returns();
		ref_point->set_id(0);
		ref_point->set_ambient_light_level(0);
		ref_point->set_start_offset_ns(0);
		ref_point->mutable_direction()->set_azimuth(0);
		ref_point->mutable_direction()->set_elevation(0);
		ref_return->set_id(0);
		ref_return->set_range(0);
		ref_return->set_intensity(0);
		ref_return->add_cartesian(0.0f);
		ref.mutable_packed();
		return ref;
	} ();

#endif

// SCANNER
scanner::scanner(std::string hostname_or_ip, std::string cert_key_file) :
	logged_object("bf::" + hostname_or_ip),
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

	protocol::Response::Hello _hello;
	__hello(&_hello);
	if (std::string(BSL_VERSION) != _hello.library_version())
		log_warning("Warning: The client BSL version does not match the server BSL version. Client has '%s', server has '%s'.\n",
			    BSL_VERSION,
			    _hello.library_version().c_str());
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

void scanner::__hello(protocol::Response::Hello* hello) {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_hello()->set_protocol_version(scanner_connection::protocol_version);
	req.mutable_hello()->set_library_version(BSL_VERSION);
	req.mutable_hello()->set_language(protocol::Language::CPP);
	conn->send_request(req, resp);
	hello->CopyFrom(resp.hello());
}

void scanner::attempt_error_recovery() {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_attempt_error_recovery();
	conn->send_request(req, resp);
}

const std::string scanner::serial_number() {
	protocol::Response::Hello _hello;
	__hello(&_hello);
	return _hello.has_serial_number() ? _hello.serial_number() : _hello.legacy_serial_number();
}

#ifndef BSL_STANDALONE

const protocol::config::Product scanner::product() {
	return hello().product();
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream() {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection());
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::data::Frame reference_frame) {
	protocol::stream::Subscribe::PointCloud extend_subscription;
	extend_subscription.mutable_reference_frame()->MergeFrom(reference_frame);
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &extend_subscription);
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::config::ScanPattern::Filter filter) {
	protocol::stream::Subscribe::PointCloud extend_subscription;
	extend_subscription.mutable_filter()->MergeFrom(filter);
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &extend_subscription);
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::config::ScanPattern::Filter filter, const protocol::data::Frame reference_frame) {
	protocol::stream::Subscribe::PointCloud extend_subscription;
	extend_subscription.mutable_filter()->MergeFrom(filter);
	extend_subscription.mutable_reference_frame()->MergeFrom(reference_frame);
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &extend_subscription);
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const std::vector<protocol::config::Algorithm> algorithms) {
	protocol::stream::Subscribe::PointCloud extend_subscription;
	for(auto a : algorithms) {
		extend_subscription.mutable_algorithms()->Add()->CopyFrom(a);
	}
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &extend_subscription);
}

std::shared_ptr<scanner::point_cloud_stream<protocol::data::Frame> > scanner::get_point_cloud_stream(const protocol::stream::Subscribe::PointCloud extend_subscription) {
	return std::make_shared<point_cloud_stream<protocol::data::Frame> >(create_connection(), &extend_subscription);
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

const protocol::Response::Hello scanner::hello() {
	protocol::Request req;
	protocol::Response resp;
	req.mutable_hello()->set_protocol_version(scanner_connection::protocol_version);
	req.mutable_hello()->set_library_version(BSL_VERSION);
	req.mutable_hello()->set_language(protocol::Language::CPP);
	conn->send_request(req, resp);
	return resp.hello();
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
	int scanlines = blickfeld::protocol::data::get_total_number_of_scanlines(frame);
	return strm << "<Blickfeld Frame " << frame.id() << ": " << frame.total_number_of_returns() << " returns, "
		    << setprecision(1) << fixed << frame.scan_pattern().horizontal().fov() * 180.0f / M_PI << "x" << frame.scan_pattern().vertical().fov() * 180.0f / M_PI << " FoV, "
		    << setprecision(0) << fixed << scanlines << " scanlines>";
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
