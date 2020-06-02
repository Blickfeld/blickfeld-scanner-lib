/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#include "bf_protobuf_plugin.h"

using namespace std;
using namespace google::protobuf;

const string file_template = "#pragma once\n\
#include <blickfeld/protocol_exception.h>\n\
@namespace_top@\n\
    @construct@\n\
\n\
    static inline void throw_typed_protocol_exception (Error err) {\n\
        switch(err.error_case()) {\n\
            @case@\n\
            case Error::ERROR_NOT_SET:\n\
            default:\n\
            break;\n\
        }\n\
        auto unknown = Error::UnknownErrorCode();\n\
        unknown.set_error_code(err.error_case());\n\
        throw protocol_exception<decltype(unknown)>(unknown);\n\
    }\n\
@namespace_bottom@\n\
\n";

const string case_template = "case @number@: throw protocol_exception<@cpptype@>(err.@name@()); break; \n";
const string construct_template = "static inline protocol_exception<@cpptype@> @type@(@arguments@) { auto err = @cpptype@(); @assignments@ return protocol_exception<decltype(err)>(err); }";

bf_protobuf_plugin::bf_protobuf_plugin() {}
bf_protobuf_plugin::~bf_protobuf_plugin() {}

bool bf_protobuf_plugin::Generate(const FileDescriptor* file,
                            const std::string& parameter,
                            compiler::GeneratorContext* generator_context,
                            std::string* error) const {

	if(file->name() != "blickfeld/error.proto") {
		*error = string("file " + file->name() + " not supported by plugin!");
		return false;
	}


	string basename = regex_replace(file->name(), regex("\\.proto"), "");
	std::unique_ptr<io::ZeroCopyOutputStream> output(generator_context->Open(basename + ".custom.pb.h"));
	io::Printer printer(output.get(), '@');

	stringstream namespace_top;
	stringstream namespace_bottom;

	istringstream iss(file->package());
	std::string ns;
	while (std::getline(iss, ns, '.')) {
		if(!ns.empty()) {
			namespace_top << "namespace " << ns << " {" << endl;
			namespace_bottom << "}" << endl;
		}
	}

	stringstream case_stream;
	stringstream constructor_stream;

	for(int i=0; i<file->message_type_count(); i++) {
		auto embedded_type = file->message_type(i);

		// generate case statements
		for(int j=0; j<embedded_type->field_count(); j++) {
			auto field = embedded_type->field(j);
			string line = string(case_template);
			line = regex_replace(line, regex("@number@"), to_string(field->number()).c_str());
			line = regex_replace(line, regex("@cpptype@"), string(embedded_type->name() + "::" + field->message_type()->name()));
			line = regex_replace(line, regex("@name@"), field->name().c_str());
			case_stream << line;
		}

		// generate constructors
		for(int j=0; j<embedded_type->nested_type_count(); j++) {
			auto nested_type = embedded_type->field(j);
			string line = string(construct_template);
			line = regex_replace(line, regex("@cpptype@"), string(embedded_type->name() + "::" + nested_type->message_type()->name()));
			line = regex_replace(line, regex("@type@"), string("Create" + embedded_type->name() + nested_type->message_type()->name()));

			stringstream assignments;
			stringstream arguments;

			list<const FieldDescriptor*> field_list;
			for(int k=0; k<nested_type->message_type()->field_count(); k++) {
				field_list.push_back(nested_type->message_type()->field(k));
			}
			field_list.sort([](const FieldDescriptor* a, const FieldDescriptor* b) {return !a->has_default_value() && b->has_default_value(); });

			list<const FieldDescriptor*>::iterator it;
			for(it=field_list.begin(); it!=field_list.end(); it++) {
				auto field = *it;
				switch(field->type()) {
				case FieldDescriptor::TYPE_INT32:
					arguments << (it!=field_list.begin() ? ", " : "") << "int " << field->name();
					if(field->has_default_value()) arguments << "=" << field->default_value_int32();
					break;
				case FieldDescriptor::TYPE_UINT32:
					arguments << (it!=field_list.begin() ? ", " : "") << "uint32_t " << field->name();
					if(field->has_default_value()) arguments << "=" << field->default_value_uint32();
					break;
				case FieldDescriptor::TYPE_FLOAT:
					arguments << (it!=field_list.begin() ? ", " : "") << "float " << field->name();
					if(field->has_default_value()) arguments << "=" << field->default_value_float();
					break;
				case FieldDescriptor::TYPE_STRING:
					arguments << (it!=field_list.begin() ? ", " : "") << "std::string " << field->name();
					if(field->has_default_value()) arguments << "=\"" << field->default_value_string() << "\"";
					break;
				default:
					// type not supported
					continue;
					break;
				}
				assignments << "err.set_" << field->name() << "(" << field->name() << "); ";
			}

			line = regex_replace(line, regex("@arguments@"), arguments.str().c_str());
			line = regex_replace(line, regex("@assignments@"), assignments.str().c_str());

			constructor_stream << line << endl;
		}
	}



	printer.Print(file_template.c_str(),
			"namespace_top", namespace_top.str().c_str(),
			"construct", constructor_stream.str().c_str(),
			"case", case_stream.str().c_str(),
			"namespace_bottom", namespace_bottom.str().c_str());

	return true;
}

int main(int argc, char** argv) {
	bf_protobuf_plugin generator;
  return google::protobuf::compiler::PluginMain(argc, argv, &generator);
}


