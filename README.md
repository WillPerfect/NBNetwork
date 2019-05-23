# NBNetwork
基于boost::asio的一个轻量级网络框架

## 服务端编程

	auto s = NBServer::create("127.0.0.1", 7195, dealPackage);
	s->start();
	system("pause");
	s->stop();

只需要提供ip，端口和一个处理器参数即可。
处理器是一个回调函数，服务端收到客户端的每一个数据包会通过这个回调函数来通知你。
处理器函数的形式如下：

    void dealPackage(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)
    {
    }

`_clientIp`和`_clientPort`表明该数据包来自哪个客户端

`_inBuffer`是输入缓冲区，用于读取客户端的数据

`_outBuffer`是输出缓冲区，如果有需要向客户端发送的数据，则写到这里。

`NBBuffer`定义如下：

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

## 客户端编程

    auto client = NBClient::create();
    bool success = client->connect("127.0.0.1", 7195, 5000);
    if (success)
    {
        std::cout << "success" << std::endl;
    }
    else {
        std::cout << "failed" << std::endl;
    }

`connect()`函数的参数是服务端ip，端口和连接超时时间(毫秒)，如果达到超时时间还没有收到服务端回应，则连接失败。

发送数据和接收数据使用`MBClient`的接口：

    virtual boost::system::error_code sendData(char const *_buffer, unsigned size) = 0;
    virtual boost::system::error_code recvData(char *_buffer, unsigned &size) = 0;
