/*--------------------------------------------------------------------------------
* Protocol.h
* This file contains the interface of the messages protocol
* --------------------------------------------------------------------------------
*/

#pragma once
#include <cstdint>
#include "ProtocolAsio.h"

enum class ProtocolHead : uint16_t {
	CLIENT_ACCEPT,			// sent by client
	CLIENT_LIVE_CLIENT,

	SERVER_CONNECT,			// sent by server
	SERVER_DISCONNECT,
	SERVER_WELCOME,

	BOTH_CONNECT_LOGIN,		// sent by client and server
	BOTH_POSITION,
	BOTH_MOVE,
	BOTH_MESSAGE
};

struct PMSG_CHAR_BYTES {
	char bytes[50];
};

class CustomClient : public olc::net::client_interface<ProtocolHead> {
public:
	void PingServer() {
		olc::net::message<ProtocolHead> msg;
		msg.header.id = ProtocolHead::CLIENT_LIVE_CLIENT;

		DWORD dwTick = GetTickCount();
		msg << dwTick;
		WORD teste1 = 1;
		WORD teste2 = 2;
		msg << teste1;
		msg << teste2;

		std::cout << "[" << dwTick << "]" << "[" << teste1 << "]" << "[" << teste2 << "]Send data.\n";
		
		Send(msg);
	}

	void MessageAll()
	{
		olc::net::message<ProtocolHead> msg;
		msg.header.id = ProtocolHead::CLIENT_ACCEPT;
		Send(msg);
	}

	void DataSend(ProtocolHead head, uint8_t* message, uint16_t size)
	{
		olc::net::message<ProtocolHead> lpMsg;

		lpMsg.header.id = head;

		lpMsg.body.resize(lpMsg.body.size() + size);

		std::memcpy(lpMsg.body.data(), message, size);

		lpMsg.header.size = lpMsg.size();

		Send(lpMsg);
	}

	void DataSend(uint8_t* lpMsg, uint16_t size)
	{
		if (IsConnected())
		{
			olc::net::message<ProtocolHead> msg;

			msg.header.id = ProtocolHead::BOTH_MESSAGE;

			msg.body.resize(msg.body.size() + size);

			std::memcpy(msg.body.data(), lpMsg, size);

			msg.header.size = msg.size();

			Send(msg);
		}
	}
};

class CProtocol {
public:
	CProtocol();
	bool ConnectServer(char* IP, uint16_t Port);
	void DisconnectServer();
	bool CheckConnected();
	void RecvMessage();

	void SendPingTest();
private:
	CustomClient* SocketConnect;
	HANDLE m_ClientAcceptThread;

};

extern CProtocol gProtocol;