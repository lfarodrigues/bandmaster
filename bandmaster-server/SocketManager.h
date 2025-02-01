#pragma once

#include "SocketConnection.h"

#define MAX_MAIN_PACKET_SIZE 8192
#define MAX_CHAR_MSG_SIZE 100
struct PMSG_TESTE_RECV
{
	DWORD TickCount;
	WORD PhysiSpeed;
	WORD MagicSpeed;
};

struct PMSG_TESTE_SEND
{
	DWORD TickCount;
};

struct PMSG_CHAR_BYTES {
	char bytes[MAX_CHAR_MSG_SIZE];
};

class CSocketManager
{
public:
	~CSocketManager() {
		mThread.detach();
		//mThread();
		//connection->Stop();
		// Tidy up the context thread
		//if (mThread.joinable()) mThread.join();
	}
	std::thread mThread;

private:
	void DataSend(uint16_t aIndex, olc::net::message<ProtocolHead> msg) {
		this->connection->ProtocolSend(aIndex, msg);
	}

public:
	bool StartServer(uint16_t nPort);
	bool CheckPortUse(unsigned short port);
	void static ListenServer();
	void DisconenctClient(uint16_t aIndex);

	//template<typename StructType>
	void PacketSend(uint16_t aIndex, ProtocolHead head, uint8_t* message, uint16_t size);
	void PacketSendAll(ProtocolHead head, uint8_t* message, uint16_t size);

	void DataReceived(uint16_t aIndex, olc::net::message<ProtocolHead> msg);

private:
	bool RunServerThread()
	{
		try
		{
			mThread = std::thread([this] { ListenServer(); });
			//mThread.join();
		}
		catch (...)
		{
			return false;
		}
		return true;
	}

private:
	CSocketConnection* connection;
	typedef typename olc::net::message<ProtocolHead> PacketMsg;
};

extern CSocketManager gSocketManager;

//namespace NewMsg = ;

