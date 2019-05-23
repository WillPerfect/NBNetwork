#pragma once

#include <functional>
#include <memory>

namespace nbnetwork {
	class NBBuffer
	{
	public:
		NBBuffer() = default;
		NBBuffer(char *_buf, unsigned bufSize)
			:m_buffer(_buf)
			, m_bufSize(bufSize) {}

		char *buffer() { return m_buffer; }
		char const *buffer() const { return m_buffer; }
		unsigned size() const { return m_bufSize; }

		void setSize(unsigned _size) { m_bufSize = _size; }
	protected:
	private:
		char *m_buffer = nullptr;
		unsigned m_bufSize = 0;
	};
	class NBServer
	{
#define DEFAULT_BUFFER_SIZE		(256 * 1024)
	public:
		static std::shared_ptr<NBServer> create(char const *_localip, short _port,
			std::function<void(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)> const &_handlerCallback,
			unsigned _inBufferSize = DEFAULT_BUFFER_SIZE, unsigned _outBufferSize = DEFAULT_BUFFER_SIZE);
		virtual ~NBServer() = default;

		virtual void start() = 0;
		virtual void stop() = 0;
	};
}


