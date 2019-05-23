// NClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include "NBClient.h"

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

int main()
{
	{
		auto client = NBClient::create();
		bool success = client->connect("127.0.0.1", 7195, 5000);
		if (success)
		{
			std::cout << "success" << std::endl;

			std::vector<char> buf(512);
			MyHead *pHead = (MyHead *)&buf[0];
			pHead->m_command = MyCommand::Ping;
			MyBody *pBody = (MyBody *)(pHead + 1);
			strcpy(pBody->m_value, "hello");
			auto ec = client->sendData(&buf[0], sizeof(MyHead) + sizeof(MyBody));
			if (!ec)
			{
				unsigned recvSize = 0;
				ec = client->recvData(&buf[0], recvSize);
				if (!ec)
				{
					std::cout << "recv : " << pBody->m_value << std::endl;
				}
			}
		}
		else {
			std::cout << "failed" << std::endl;
		}

		system("pause");
	}
	system("pause");
	return 0;
}

