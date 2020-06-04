/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include "blickfeld/scanner-config.h"
#include "blickfeld/string_utils.h"
#include "blickfeld/logged_object.h"
#include "blickfeld/utils.h"

#ifdef HAVE_SYSLOG_H
#  include <syslog.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <mutex>

using namespace std;

namespace blickfeld {

static std::map<string, logged_object::level> modules_loglevel;
logged_object::level logged_object::print_to_screen_default = LEVEL_INFO;

using listeners_t = std::map<unsigned int, logged_object::listener_t>;
static listeners_t listeners;
static unsigned int next_listener_id = 1;
static std::mutex listeners_lock;

void logged_object::set_modules_loglevel(logged_object::level ll, std::string modules) {
	vector<string> mods = split_string(modules, ",", 0);
	for(unsigned int i = 0; i < mods.size(); i++)
		modules_loglevel[strip(mods[i], " ")] = ll;
}

logged_object::logged_object(string ident) : ident(ident){
	msg_buffer = nullptr;
	msg_buffer_size = 0;
	msg_buffer_len = 0;
	did_openlog = false;

	send_to_syslog = LEVEL_INFO;
	send_to_listeners = LEVEL_VERBOSE;

	print_to_screen = logged_object::print_to_screen_default;
	if(modules_loglevel.find(ident) != modules_loglevel.end())
		print_to_screen = modules_loglevel[ident];

	keep_in_mem = LEVEL_VERBOSE;
	// keep_in_mem = LEVEL_INFO;

	max_kept = DEFAULT_LOG_MSGS_KEPT_IN_MEM;
}

logged_object::~logged_object() {
	// printf("logged_object %p detor: %s\n", this, ident.c_str());
	msg_buffer_lock.lock();
	if(msg_buffer) {
		free(msg_buffer);
		msg_buffer = nullptr;
		msg_buffer_size = 0;
	}
	msg_buffer_lock.unlock();
}

#ifdef HAVE_SYSLOG_H

unsigned int get_syslog_level(logged_object::level ll) {
	switch(ll) {
	case logged_object::LEVEL_NONE:    return LOG_DEBUG;
	case logged_object::LEVEL_DEBUG:   return LOG_DEBUG;
	case logged_object::LEVEL_VERBOSE: return LOG_INFO;
	case logged_object::LEVEL_INFO:    return LOG_NOTICE;
	case logged_object::LEVEL_WARNING: return LOG_WARNING;
	case logged_object::LEVEL_ERROR:   return LOG_ERR;
	case logged_object::LEVEL_NEVER:   return LOG_ERR;
	}
	return LOG_NOTICE;
}

#endif

void logged_object::log(level ll, const char* format, ...) {
	bool do_print_to_screen = ll >= print_to_screen;
	bool do_keep_in_mem = ll >= keep_in_mem;
#ifdef HAVE_SYSLOG_H
	bool do_send_to_syslog = ll >= send_to_syslog;
#else
	bool do_send_to_syslog = false;
#endif
	bool pass_to_listeners = ll >= send_to_listeners;

	if(!pass_to_listeners && !do_print_to_screen && !do_keep_in_mem && !do_send_to_syslog)
		return; // fastest, do nothing!

	msg_buffer_lock.lock();

#ifdef HAVE_SYSLOG_H
	if(do_send_to_syslog) {
		if(!did_openlog) {
			syslog_ident = "blickfeld.";
			syslog_ident += ident;
			openlog(syslog_ident.c_str(), LOG_PID | LOG_CONS, LOG_DAEMON);
			did_openlog = true;
		}
		va_list ap;
		va_start(ap, format);
		vsyslog(get_syslog_level(ll), format, ap);
		va_end(ap);
	}
#endif
	// have to format message for stdout or to keep in mem...
	double ts = os::get_time();
	string ts_str = os::get_time_string(ts);

	msg_buffer_len = 0;
	append_format_string(&msg_buffer, &msg_buffer_size, &msg_buffer_len, "%s %*.*s | ", ts_str.c_str(), 16, 16, ident.c_str());
	unsigned int only_message_len = msg_buffer_len;
	unsigned int org_len = msg_buffer_len;
	append_vsn_format(format, msg_buffer_size, msg_buffer_len, msg_buffer, org_len);

	if(pass_to_listeners) {
		const char* only_message = msg_buffer + only_message_len;
		notify_listeners(ll, ts_str, only_message);
	}

	if(do_print_to_screen)
		fwrite(msg_buffer, msg_buffer_len, 1, stdout);

	if(do_keep_in_mem) {
		kept.push_back(string(msg_buffer, msg_buffer_len));
		if(kept.size() > max_kept)
			kept.pop_front();
	}
	msg_buffer_lock.unlock();
}

unsigned int logged_object::add_log_listener(listener_t l) {
	lock_guard<mutex> g(listeners_lock);
	unsigned int listener_id = next_listener_id++;
	listeners[listener_id] = l;
	return listener_id;
}

void logged_object::remove_log_listener(unsigned int listener_id) {
	lock_guard<mutex> g(listeners_lock);
	for(auto listener = listeners.begin(); listener != listeners.end(); listener++) {
		if(listener->first == listener_id) {
			listeners.erase(listener);
			break;
		}
	}
}

void logged_object::notify_listeners(logged_object::level ll, const std::string& ts, std::string msg) {
	lock_guard<mutex> g(listeners_lock);
	for(auto& listener : listeners) {
		try {
			listener.second(ident, ll, ts, msg);
		}
		catch(const std::exception& e) {
			fprintf(stderr, "exception from log_listener:\n%s\n", e.what());
		}
	}
}

} // namespace blickfeld
