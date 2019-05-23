// NServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NBServer.h"
#include <cstdint>
#include <iostream>

using namespace nbnetwork;

#pragma pack(1)
enum MyCommand
{
	Ping = 1,
	Pong
};

struct MyHead 
{
	std::uint32_t m_version;
	std::uint32_t m_command;
	std::uint32_t m_status;
};

struct MyBody
{
	char m_value[16]{};
};
#pragma pack()

void dealPing(NBBuffer const& _inBuffer, NBBuffer &_outBuffer)
{
	MyHead const* pHead = (MyHead const*)_inBuffer.buffer();
	MyBody *pBody = (MyBody *)(pHead + 1);

	std::cout << "data : " << pBody->m_value << std::endl;
	_outBuffer.setSize(sizeof(MyHead) + sizeof(MyBody));
	memcpy(_outBuffer.buffer(), _inBuffer.buffer(), sizeof(MyHead));
	MyHead *pOutHead = (MyHead *)_outBuffer.buffer();
	pOutHead->m_status = 1;
	pOutHead->m_command = MyCommand::Pong;
	MyBody *pOutBody = (MyBody *)(pOutHead + 1);
	strcpy(pOutBody->m_value, "world");
}

void dealPackage(char const *_clientIp, unsigned _clientPort, NBBuffer const& _inBuffer, NBBuffer &_outBuffer)
{
	std::cout << "client ip : " << _clientIp << ", client port : " << _clientPort << std::endl;
	MyHead const* pHead = (MyHead const*)_inBuffer.buffer();
	switch (pHead->m_command)
	{
	case MyCommand::Ping:
		dealPing(_inBuffer, _outBuffer);
		break;
	default:
		break;
	}
}

int main()
{
	auto s = NBServer::create("127.0.0.1", 7195, dealPackage);

	s->start();

	system("pause");

	s->stop();

    return 0;
}

