/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */

#pragma once

#include <iostream>
#include <stdint.h>
#include <functional>
#include <numeric>

#include <asio.h>

#include "blickfeld/discover.h"

namespace blickfeld {
namespace impl {

class discover {

#pragma pack(push, 1)

	// Reference: https://tools.ietf.org/html/rfc1035

	struct dns_header_t {
		uint16_t id;
		struct {
			uint16_t is_response : 1;
			uint16_t _r : 15;
		} flags;
		uint16_t question_count;
		uint16_t answer_count;
		uint32_t _r;
	};

	struct dns_query_t {
		uint8_t end_of_name;
		uint16_t type;
		uint16_t class_;
	};

	struct dns_answer_t {
		uint16_t type;
		uint16_t class_;
		uint32_t ttl;
		uint16_t data_size;
	};

#pragma pack(pop)

	asio::io_context io_context;

	class safe_buffer {
		std::vector<uint8_t> data;
		size_t offset = 0;

public:
		safe_buffer(size_t data_len) : data(data_len) {
		}

		char* mutate(size_t len) {
			if (data.size() < len)
				throw std::out_of_range("Buffer too small.");
			return reinterpret_cast<char*>(data.data());
		}

		template<class T>
		void read(T& obj, size_t offset) {
			if (data.size() < offset +  sizeof(T))
				throw std::out_of_range("Attempted to read too large object.");
			memcpy(&obj, data.data() + offset, sizeof(T));
		}

		template<class T>
		void read(T& obj) {
			read(obj, offset);
			offset += sizeof(T);
		}

		template<class T>
		T read(size_t offset) {
			T val;
			read(val, offset);
			return val;
		}

		void skip(size_t len) {
			if (data.size() < offset + len)
				throw std::out_of_range("Not enough bytes to skip.");
			offset += len;
		}

		uint8_t read_u8() {
			uint8_t val;
			read(val);
			return val;
		}

		uint16_t read_u16() {
			uint16_t val;
			read(val);
			return val;
		}

		const std::string read_string(size_t len, size_t offset) {
			if (data.size() < offset + len)
				throw std::out_of_range("Attempted to read string with length " + std::to_string(len) + " (available " + std::to_string(data.size() - offset) + ").");
			std::string str(data.data() + offset, data.data() + offset + len);
			return str;
		}

		const std::string read_string(size_t len) {
			auto str = read_string(len, offset);
			offset += len;
			return str;
		}

		const std::vector<std::string> read_name(size_t &offset) {
			uint8_t len;
			read(len, offset);
			offset += 1;

			std::vector<std::string> out;
			while (len) {
				if (len & 0xC0) {
					size_t coffset = (len & 0x3F) + read<uint8_t>(offset);
					offset += 1;
					auto ptr = read_name(coffset);
					out.insert(out.end(), ptr.begin(), ptr.end());
					return out;
				} else {
					out.push_back(read_string(len, offset));
					offset += len;
				}

				read(len, offset);
				offset += 1;
			}
			return out;
		}

		const std::vector<std::string> read_name() {
			return read_name(offset);
		}

	};

	asio::ip::udp::endpoint multicast;
	asio::ip::udp::socket socket;

	asio::streambuf input_buffer;
	asio::streambuf output_buffer;

	std::vector<std::string> search;
	asio::ip::udp::endpoint sender_endpoint_;

	void send() {
		auto header = dns_header_t();
		header.question_count = htons(1);

		std::ostream send_stream(&output_buffer);
		send_stream.write(reinterpret_cast<const char*>(&header), sizeof(dns_header_t));

		for (std::string service : search) {
			uint8_t service_str_len = static_cast<uint8_t>(service.size());
			send_stream.write(reinterpret_cast<const char*>(&service_str_len), 1);
			send_stream.write(service.data(), static_cast<long>(service.size()));
		}

		auto query = dns_query_t();
		query.type = htons(0x0C); // PTR
		query.class_ = htons(0x1); // IN
		send_stream.write(reinterpret_cast<const char*>(&query), sizeof(dns_query_t));

		socket.async_send_to(output_buffer.data(), multicast, []( asio::error_code ec,  size_t bytes) {
				});
	}

	void recv() {
		socket.async_receive_from(asio::buffer(input_buffer.prepare(512)), sender_endpoint_, [&](const asio::error_code& ec, size_t bytes) {
					if (ec)
						return;

					input_buffer.commit(bytes);

					std::istream recv_stream(&input_buffer);

					safe_buffer buffer(bytes);
					recv_stream.read(buffer.mutate(bytes), bytes);

					try {
						dns_header_t recv_header;
						buffer.read(recv_header);

						bool is_bf = false;
						blickfeld::discover::result_t result;

						int count = ntohs(recv_header.answer_count);
						while (count--) {
							auto name = buffer.read_name();

							dns_answer_t answer;
							buffer.read(answer);

							if (ntohs(answer.type) == 0xc) { // PTR
								buffer.read_name();
							} else if (ntohs(answer.type) == 0x21) { // SRV
								buffer.skip(4);
								result.port = ntohs(buffer.read_u16());
								result.hostname = concat_name(buffer.read_name());

								if (name.size() > 0) {
									result.service = name[0];
									name.erase(name.begin());
									is_bf = concat_name(name) == concat_name(search);
								}
							} else if (ntohs(answer.type) == 0x1) { // A
								buffer.read(result.ipv4);
							} else if (ntohs(answer.type) == 0x1c) { // AAAA
								buffer.read(result.ipv6);
								for (int i = 0; i < 8; i++)
									result.ipv6[i] = ntohs(result.ipv6[i]);
							} else {
								buffer.skip(ntohs(answer.data_size));
							}
						}

						if (is_bf) {
							results.push_back(result);
						}
					} catch(std::exception &ex) {
					}
					recv();
				});
	}

	const std::string concat_name(std::vector<std::string> name) {
		return std::accumulate(name.begin(), name.end(), std::string(), [](std::string a, std::string b) {
					if (a != "")
						return a + "." + b;
					return b;
				});
	}

public:
	std::list<blickfeld::discover::result_t> results;

	discover() :
		multicast(asio::ip::address::from_string("224.0.0.251"), 5353),
		socket(io_context, asio::ip::udp::v4()),
		search{ "_blickfeld-lidar", "_tcp", "local" }
	{
		socket.set_option(asio::socket_base::reuse_address(true));
		socket.set_option(asio::ip::multicast::join_group(multicast.address()));
		socket.set_option(asio::ip::multicast::hops(255));
		socket.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), 5353));
	}

	virtual ~discover() {
	}

	void run_for(float duration) {
		recv();
		send();

		io_context.run_for(std::chrono::milliseconds(static_cast<int>(duration * 1000)));
		socket.close();
		io_context.run();
	}

};

}
}
