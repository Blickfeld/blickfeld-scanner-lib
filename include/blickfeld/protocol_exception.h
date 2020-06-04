/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <regex>
#include "blickfeld/exception.h"
#include "blickfeld/error.pb.h"
#include "blickfeld/string_utils.h"

namespace blickfeld {

class protocol_exception_base : public std::exception {
public:
	const protocol::Error error;

	explicit protocol_exception_base(const protocol::Error error) : error(error) {
	}

	virtual ~protocol_exception_base() noexcept {
	}

	virtual const char* what() const noexcept {
		return protocol::Error::descriptor()->FindFieldByNumber(static_cast<int>(error.error_case()))->message_type()->full_name().c_str();
	}

};

template<class T>
class protocol_exception : public protocol_exception_base {
	T instance;
	std::string whatmessage;

	const protocol::Error reflect(T error) {
		protocol::Error err;
		auto desc = err.GetDescriptor();
		bool found = false;
		for (int i = 0; i < desc->field_count(); i++) {
			auto* fd = desc->field(i);
			// TODO Refactor this string comparison. Can this be done more efficient?
			if(fd->message_type()->full_name() == error.GetTypeName()) {
				err.GetReflection()->SetAllocatedMessage(&err, new T(error), fd);
				found = true;
				break;
			}
		}
		if (!found) {
			err.mutable_server_implementation();
		}
		return err;
	}

	void prepare() {
		const std::regex regex("\\{([a-z_]+)\\:(\\%\\d*\\.*\\d*[fsdu])\\}");
		std::smatch match;
		std::stringstream message;

		auto type = protocol::Error::descriptor()->FindFieldByNumber(static_cast<int>(error.error_case()))->message_type();
		auto refl = instance.GetReflection();
		std::string e_desc = type->options().GetExtension(protocol::e_desc);

		message << type->full_name() << ": (errno: " << static_cast<int>(error.error_case()) << ")" << std::endl << "\t";
		while(std::regex_search(e_desc, match, regex)) {
			message << match.prefix().str();
			if(type->FindFieldByName(match[1])) {
				switch(type->FindFieldByName(match[1])->cpp_type()) {
				case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
					message << format_string(match[2].str().c_str(), refl->GetString(instance, type->FindFieldByName(match[1])).c_str());
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
					message << format_string(match[2].str().c_str(), refl->GetInt32(instance, type->FindFieldByName(match[1])));
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
					message << format_string(match[2].str().c_str(), refl->GetUInt32(instance, type->FindFieldByName(match[1])));
					break;
				case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
					message << format_string(match[2].str().c_str(), refl->GetFloat(instance, type->FindFieldByName(match[1])));
					break;
				default:
					break;
				}
			} else {
				message << "{" << match[1].str() << ":absent}";
			}
			e_desc = match.suffix().str();
		}
		message << e_desc << std::endl;
		whatmessage = std::string(message.str().c_str());
	}

public:
	explicit protocol_exception(T error) : protocol_exception_base(reflect(error)), instance(error) {
		prepare();
	}

	explicit protocol_exception() : protocol_exception(T()) {
	}

	virtual const char* what() const noexcept {
		return whatmessage.c_str();
	}

};

} // namespace blickfeld
