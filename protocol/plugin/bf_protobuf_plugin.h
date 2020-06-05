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
#include <string>
#include <regex>
#include <list>
#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/compiler/plugin.h>
#include <google/protobuf/compiler/code_generator.h>

class bf_protobuf_plugin : public google::protobuf::compiler::CodeGenerator {
public:
	bf_protobuf_plugin();
	~bf_protobuf_plugin();
	bool Generate(const google::protobuf::FileDescriptor* file,
                const std::string& parameter,
				google::protobuf::compiler::GeneratorContext* generator_context,
                std::string* error) const;
private:
	bool replace_string(std::string& str, const std::string& from, const std::string& to);
	GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(bf_protobuf_plugin);
};
