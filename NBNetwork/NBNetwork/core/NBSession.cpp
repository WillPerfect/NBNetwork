#include "stdafx.h"
#include <boost/bind.hpp>
#include <iostream>
#include "NBSession.h"
#include "NBProtocol.h"
#include "NBServerImpl.h"

using namespace boost::asio;
namespace nbnetwork {
	NBSession::NBSession(boost::asio::io_service &_ioService, std::shared_ptr<NBServerImpl> const &_pServer,
		std::function<void(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)> &_handlerCallback,
		unsigned _inBufferSize, unsigned _outBufferSize)
		:m_socket(_ioService)
		, m_inBuffer(_inBufferSize)
		, m_outBuffer(_outBufferSize)
		,m_handlerCallback(_handlerCallback)
		,m_pServer(_pServer)
	{
	}

	NBSession::~NBSession()
	{
		std::cout << "NBSession close" << std::endl;
	}

	void NBSession::start()
	{
		m_strClientIp = m_socket.remote_endpoint().address().to_string();
		m_port = m_socket.remote_endpoint().port();
		doRead();
	}

	void NBSession::stop()
	{
		m_socket.close();
	}
	
	void NBSession::doRead(bool _bReadHead)
	{
		m_state = state::ReadHead;
		auto buf = buffer(&m_head, m_headSize);
		if (!_bReadHead)
		{
			buf = buffer(&m_inBuffer[0], m_head.datasize());
			m_state = state::ReadBody;
		}
		async_read(m_socket, buf, transfer_all(), boost::bind(&NBSession::dataComplete, this, boost::asio::placeholders::error));
	}

	void NBSession::doWrite(bool _bWriteHead)
	{
		m_state = state::SendHead;
		auto buf = buffer(&m_head, m_headSize);
		if (!_bWriteHead)
		{
			buf = buffer(m_writeDataInfo.buffer(), m_writeDataInfo.size());
			m_state = state::SendBody;
		}
		async_write(m_socket, buf, transfer_all(), boost::bind(&NBSession::dataComplete, this, boost::asio::placeholders::error));
	}

	void NBSession::dataComplete(boost::system::error_code ec)
	{
		m_lastIOTime = time(nullptr);
		if (!ec)
		{
			if (m_state == state::ReadHead)
			{
				if (checkHead(&m_head))
				{
					if (m_head.datasize() != 0)
					{
						doRead(false);
					}
					else {
						// error
						m_pServer->removeSession(shared_from_this());
					}
				}
				else {
					// error
					m_pServer->removeSession(shared_from_this());
				}
			}
			else if (m_state == state::ReadBody)
			{
				dealPackage();
			}
			else if (m_state == state::SendHead){
				doWrite(false);
			}
			else {
				doRead();
			}
		}
		else {
			// error
			m_pServer->removeSession(shared_from_this());
		}
	}

	bool NBSession::checkHead(NBHead *pHead)
	{
		return true;
	}

	void NBSession::dealPackage()
	{
		// do package
		unsigned sendSize = m_headSize;
		NBBuffer inBuf(&m_inBuffer[0], m_head.datasize()), outBuf(&m_outBuffer[0], 0);

		m_handlerCallback(m_strClientIp.c_str(), m_port, inBuf, outBuf);
		if (outBuf.size() != 0)
		{
			m_head.setDataSize(outBuf.size());
			m_writeDataInfo = outBuf;
			doWrite(); // send head
		}
		else {
			doRead();
		}
	}

}
