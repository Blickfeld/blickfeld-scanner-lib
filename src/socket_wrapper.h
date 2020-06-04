/*
 * Copyright (c) 2020 Blickfeld GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE.md file in the root directory of this source tree.
 */
#pragma once

#include <string>
#include <functional>
#include <utility>
#include "asio/ip/tcp.hpp"
#include "asio/ssl.hpp"
#include <blickfeld/logged_object.h>

namespace blickfeld {

template<class socket_t, template<class> class stream_t>
class socket_stream_merger
{
protected:
	std::shared_ptr<socket_t> socket = NULL;
	std::shared_ptr<stream_t<socket_t> > stream = NULL;

	#define OPERATE_ON_SOCKET(functor)                                                      \
	if(socket != NULL) {                                                                                    \
		return socket->functor;                                                                         \
	} else if(stream != NULL) {                                                                             \
		return stream->next_layer().functor;                                            \
	} else {                                                                                                                \
		throw std::runtime_error("socket_stream_merger in empty state");  \
	}

	#define OPERATE_ON_STREAM_T(functor) \
	if(stream != NULL) {                             \
		return stream->functor;                  \
	}                                                                        \

	#define OPERATE_ON_SOCKET_OR_STREAM(functor)                                    \
	if(socket != NULL) {                                                                                    \
		return socket->functor;                                                                         \
	} else if(stream != NULL) {                                                                             \
		return stream->functor;                                                                         \
	} else {                                                                                                                \
		throw std::runtime_error("socket_stream_merger in empty state");  \
	}

public:
	typedef typename socket_t::shutdown_type shutdown_type;
	static const typename socket_t::shutdown_type shutdown_both = socket_t::shutdown_both;

	explicit socket_stream_merger(std::shared_ptr<socket_t> socket) :
		socket(socket)
	{
	}

	explicit socket_stream_merger(std::shared_ptr<stream_t<socket_t> > stream) :
		stream(stream)
	{
	}

	template<typename ... Args>
	void connect(Args... args)
	{
		OPERATE_ON_SOCKET(connect(args ...));
	}

	template<typename ... Args>
	void handshake(Args... args)
	{
		OPERATE_ON_STREAM_T(handshake(args ...));
	}

	typename socket_t::endpoint_type remote_endpoint() // const
	{
		OPERATE_ON_SOCKET(remote_endpoint());
	}

	typename socket_t::endpoint_type remote_endpoint(asio::error_code& ec) // const
	{
		OPERATE_ON_SOCKET(remote_endpoint(ec));
	}

	void shutdown()
	{
		OPERATE_ON_STREAM_T(shutdown());
	}

	void shutdown(typename socket_t::shutdown_type what)
	{
		OPERATE_ON_SOCKET(shutdown(what));
	}

	void shutdown(typename socket_t::shutdown_type what, asio::error_code & ec)
	{
		OPERATE_ON_SOCKET(shutdown(what, ec));
	}

	void close()
	{
		OPERATE_ON_SOCKET(close());
	}

	void close(asio::error_code & ec)
	{
		OPERATE_ON_SOCKET(close(ec));
	}

	bool is_open() // const
	{
		OPERATE_ON_SOCKET(is_open());
	}

	typename socket_t::executor_type get_executor()
	{
		OPERATE_ON_SOCKET_OR_STREAM(get_executor());
	}

	template<typename ... Args>
	std::size_t write_some(Args&& ... args)
	{
		OPERATE_ON_SOCKET_OR_STREAM(write_some(std::forward<Args>(args) ...));
	}

	template<typename ConstBufferSequence, typename WriteHandler>
	// typename std::result_of<decltype(socket->async_write_some)(const typename ConstBufferSequence&, WriteHandler&&)>::type
	// asio::async_result<std::decay<CompletionToken>::type, async_write_some(const typename ConstBufferSequence&, WriteHandler&&)>::return_type
	void async_write_some(const ConstBufferSequence& buffers, WriteHandler && handler)
	{
		OPERATE_ON_SOCKET_OR_STREAM(async_write_some(std::forward<const ConstBufferSequence>(buffers), std::forward<WriteHandler>(handler)));
	}

	template<typename ... Args>
	std::size_t read_some(Args&& ... args)
	{
		OPERATE_ON_SOCKET_OR_STREAM(read_some(std::forward<Args>(args) ...));
	}

	template<typename MutableBufferSequence, typename ReadHandler>
	void async_read_some(const MutableBufferSequence& buffers, ReadHandler && handler)
	{
		OPERATE_ON_SOCKET_OR_STREAM(async_read_some(std::forward<const MutableBufferSequence>(buffers), std::forward<ReadHandler>(handler)));
	}

};

typedef socket_stream_merger<asio::ip::tcp::socket, asio::ssl::stream> socket_wrapper;

}  /* namespace blickfeld */
