/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#ifndef BLICKFELD_LOGGED_OBJECT_H
#define BLICKFELD_LOGGED_OBJECT_H

#include "blickfeld/scanner-config.h"

#include <string>
#include <list>
#include <mutex>
#include <functional>

namespace blickfeld {

#define log_error(...)   log(LEVEL_ERROR,   __VA_ARGS__)
#define log_warning(...) log(LEVEL_WARNING, __VA_ARGS__)
#define log_info(...)    log(LEVEL_INFO,    __VA_ARGS__)
#define log_verbose(...) log(LEVEL_VERBOSE, __VA_ARGS__)
#define log_debug(...)   log(LEVEL_DEBUG,   __VA_ARGS__)

class logged_object  {
	std::mutex msg_buffer_lock;
	char* msg_buffer;
	unsigned int msg_buffer_size;
	unsigned int msg_buffer_len;
	std::list<std::string> kept;
	bool did_openlog;
	std::string syslog_ident;
protected:
	std::string ident;
public:
	enum level {
		LEVEL_NONE,

		LEVEL_DEBUG,
		LEVEL_VERBOSE,
		LEVEL_INFO,
		LEVEL_WARNING,
		LEVEL_ERROR,

		LEVEL_NEVER, // use this as a disable-output-threshold
	};

	static level print_to_screen_default;

	// configured thresholds
	level send_to_syslog;
	level print_to_screen;
	level keep_in_mem;
	level send_to_listeners;
	unsigned int max_kept; // how many messages to keep in memory

	logged_object(std::string identifier);
	~logged_object();
	void log(level ll, const char* format, ...);    // NOT reentrant!

	static void set_modules_loglevel(level ll, std::string modules);
	std::string get_ident() {
		return ident;
	}

	// ident, level, ts-str, msg
	using listener_t = std::function<void (const std::string&, logged_object::level, const std::string&, const std::string&)>;
	static unsigned int add_log_listener(listener_t l);
	static void remove_log_listener(unsigned int listener_id);
	void notify_listeners(logged_object::level ll, const std::string& ts, std::string msg);
};

}

#endif // BLICKFELD_LOGGED_OBJECT_H
