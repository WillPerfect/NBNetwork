#pragma once

#include <boost/asio.hpp>
#include <memory>

namespace nbnetwork {

	class NBClient
	{
	public:
		static std::shared_ptr<NBClient> create();
		virtual ~NBClient() = default;

		virtual bool connect(char const *_ip, short _port, unsigned _timeout = 0) = 0;
		virtual boost::system::error_code sendData(char const *_buffer, unsigned size) = 0;
		virtual boost::system::error_code recvData(char *_buffer, unsigned &size) = 0;
	};

}
