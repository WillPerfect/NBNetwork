#pragma once

#include <boost/asio.hpp>
#include <vector>
#include "NBProtocol.h"
#include "../NBServer.h"

namespace nbnetwork {
	struct NBHead;
	class NBServerImpl;
	class NBSession : public std::enable_shared_from_this<NBSession>
	{
	public:
		NBSession(boost::asio::io_service &_ioService, std::shared_ptr<NBServerImpl> const &_pServer,
			std::function<void(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)> &_handlerCallback,
			unsigned _inBufferSize, unsigned _outBufferSize);

		~NBSession();

		boost::asio::ip::tcp::socket &socket() { return m_socket; }
		void start();
		void stop();
		time_t ioTime() const { return m_lastIOTime; }

		NBSession(NBSession const&) = delete;
		NBSession &operator=(NBSession const &) = delete;
	private:
		enum class state {
			ReadHead,
			ReadBody,
			SendHead,
			SendBody
		};

		void doRead(bool _bReadHead = true);
		void doWrite(bool _bWriteHead = true);
		void dataComplete(boost::system::error_code _ec);
		bool checkHead(NBHead *_pHead);
		void dealPackage();

		boost::asio::ip::tcp::socket m_socket;
		state m_state{ state::ReadHead };

		std::string m_strClientIp;
		unsigned short m_port;

		std::atomic<time_t> m_lastIOTime{ 0 };

		NBHead	m_head;
		unsigned const m_headSize = sizeof(NBHead);

		std::vector<char> m_inBuffer;
		std::vector<char> m_outBuffer;

		NBBuffer m_writeDataInfo;

		std::function<void(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)> &m_handlerCallback;
		std::shared_ptr<NBServerImpl> m_pServer;
	};

}