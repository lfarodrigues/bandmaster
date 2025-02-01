#include <cstdint>
#include "SocketManager.h"
#include "User.h"

CSocketManager gSocketManager;

bool CSocketManager::StartServer(uint16_t nPort) {
	if (!this->CheckPortUse(nPort)) {
		this->connection = new CSocketConnection(nPort);

		this->connection->Start();

		this->RunServerThread();

		return true;
	}

	std::cout << "Port already in use!\n";

	return false;
}
 
bool CSocketManager::CheckPortUse(unsigned short port) {
	using namespace asio;
	using ip::tcp;

	io_service svc;
	tcp::acceptor a(svc);

	asio::error_code ec;

	a.open(tcp::v4(), ec) || a.bind({ tcp::v4(), port}, ec);

	return ec == error::address_in_use;
}

void CSocketManager::ListenServer() {
	while (1) {
		gSocketManager.connection->Update(-1, true);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void CSocketManager::DisconenctClient(uint16_t aIndex) {
	this->connection->DisconnectClient(aIndex);
}

void CSocketManager::PacketSend(uint16_t aIndex, ProtocolHead head, uint8_t* message, uint16_t size)
{
	if (size > MAX_MAIN_PACKET_SIZE)
	{
		//LogAdd(LOG_RED, "[SocketManager] ERROR: Max msg size %d (Msg size:%d)", MAX_MAIN_PACKET_SIZE, size);
		return;
	}

	PacketMsg lpMsg;

	lpMsg.header.id = head;

	lpMsg.body.resize(lpMsg.body.size() + size);

	std::memcpy(lpMsg.body.data(), message, size);

	lpMsg.header.size = lpMsg.size();

	this->connection->ProtocolSend(aIndex, lpMsg);
}

void CSocketManager::PacketSendAll(ProtocolHead head, uint8_t* message, uint16_t size) {
	PacketMsg lpMsg;
	
	lpMsg.header.id = head;

	lpMsg.body.resize(lpMsg.body.size() + size);
	
	std::memcpy(lpMsg.body.data(), message, size);

	lpMsg.header.size = lpMsg.size();

	this->connection->MessageAllClients(lpMsg);
}


void CSocketManager::DataReceived(uint16_t aIndex, olc::net::message<ProtocolHead> msg) {
	std::cout << (int)msg.header.id << std::endl;

	switch(msg.header.id) {
		case ProtocolHead::CLIENT_LIVE_CLIENT:
			PMSG_TESTE_RECV lpMsg;
			msg >> lpMsg;

			std::cout << "[" << aIndex << "]: [" << lpMsg.TickCount << "][" << lpMsg.PhysiSpeed << "][" << lpMsg.MagicSpeed << "]:Data Recv\n";

			//auto lpObj = &gObj[aIndex];

			//lpObj->ConnectTickCount = GetTickCount();

			std::cout << "DataSend [" << GetTickCount() << "]\n";

			PMSG_TESTE_SEND TesteSend;

			TesteSend.TickCount = GetTickCount();

			this->PacketSend(aIndex, ProtocolHead::SERVER_DISCONNECT, (uint8_t*)&TesteSend, sizeof(TesteSend));

			break;
	}
}

