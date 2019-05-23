#pragma once

#include "../NBServer.h"
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_set>

namespace nbnetwork {
	class NBSession;
	class NBServerImpl : public NBServer, public std::enable_shared_from_this<NBServerImpl>
	{
#define DEFAULT_BUFFER_SIZE		(256 * 1024)
	public:
		NBServerImpl(char const *_localip, short _port,
			std::function<void(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)> const &_handlerCallback,
			unsigned _inBufferSize = DEFAULT_BUFFER_SIZE, unsigned _outBufferSize = DEFAULT_BUFFER_SIZE);
		virtual ~NBServerImpl();

		void start() override;
		void stop() override;

		void removeSession(std::shared_ptr<NBSession> const &_session);
	private:
		void doAccept();

		boost::asio::io_service m_ioService;
		boost::asio::ip::tcp::acceptor m_acceptor;
		std::unordered_set<std::shared_ptr<NBSession>> m_sessions;
		std::vector<std::thread> m_workThreads;
		std::mutex m_mutex;
		std::thread m_checkSessionThread;
		bool m_stop = false;

		unsigned m_inBufferSize;
		unsigned m_outBufferSize;

		std::function<void(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)> m_handlerCallback;
	};
}


