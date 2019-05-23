#include "stdafx.h"
#include "NBClientImpl.h"


using namespace boost::asio;
namespace nbnetwork {

	std::shared_ptr<NBClient> NBClient::create()
	{
		return std::make_shared<NBClientImpl>();
	}

	NBClientImpl::NBClientImpl()
		:m_socket(m_ioService)
	{
	}

	bool NBClientImpl::connect(char const *_ip, short _port, unsigned _timeout)
	{
		ip::tcp::endpoint ep(ip::address_v4::from_string(_ip), _port);
		bool connect_success = false;

		m_socket.async_connect(ep, [&connect_success](boost::system::error_code ec) { connect_success = !ec; });

		bool timeout = false;
		deadline_timer	timer(m_ioService);
		if (_timeout != 0)
		{
			timer.expires_from_now(boost::posix_time::milliseconds(_timeout));
			timer.async_wait([&timeout](boost::system::error_code ec) { timeout = !ec; });
		}

		do
		{
			m_ioService.run_one();
		} while (!connect_success && !timeout);
		if (!connect_success)
		{
			m_socket.close();
		}
		else {
			timer.cancel();
		}
		return connect_success;
	}

	boost::system::error_code NBClientImpl::sendData(char const *_buffer, unsigned size)
	{
		if (size != 0)
		{
			m_head.setDataSize(size);
			auto ec = send((char const *)&m_head, sizeof(NBHead));
			if (!ec)
			{
				ec = send(_buffer, size);
			}
			return ec;
		}
		else {
			return boost::system::error_code();
		}
	}

	boost::system::error_code NBClientImpl::recvData(char *_buffer, unsigned &size)
	{
		auto ec = recv((char *)&m_head, sizeof(NBHead));
		if (!ec)
		{
			ec = recv(_buffer, m_head.datasize());
			if (!ec)
			{
				size = m_head.datasize();
			}
		}
		return ec;
	}

	boost::system::error_code NBClientImpl::send(char const *_buffer, unsigned size)
	{
		boost::system::error_code ec;
		write(m_socket, buffer(_buffer, size), transfer_all(), ec);
		return ec;
	}

	boost::system::error_code NBClientImpl::recv(char *_buffer, unsigned size)
	{
		boost::system::error_code ec;
		read(m_socket, buffer(_buffer, size), transfer_all(), ec);
		return ec;
	}

}