#pragma once

#include "../NBClient.h"
#include "NBProtocol.h"

namespace nbnetwork {
	class NBClientImpl : public NBClient
	{
	public:
		NBClientImpl();

		bool connect(char const *_ip, short _port, unsigned _timeout = 0) override;
		boost::system::error_code sendData(char const *_buffer, unsigned size) override;
		boost::system::error_code recvData(char *_buffer, unsigned &size) override;

	private:
		boost::system::error_code send(char const *_buffer, unsigned size);
		boost::system::error_code recv(char *_buffer, unsigned size);

		boost::asio::io_service m_ioService;
		boost::asio::ip::tcp::socket m_socket;

		NBHead m_head;
		unsigned const m_headSize = sizeof(NBHead);
	};
}


