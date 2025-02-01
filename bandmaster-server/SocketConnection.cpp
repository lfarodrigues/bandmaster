#include "SocketConnection.h"
#include "SocketManager.h"
#include "User.h"

bool CSocketConnection::OnClientConnect(std::shared_ptr<olc::net::connection<ProtocolHead>> client)
{
	std::cout << "Client ID: " << client->GetID() << std::endl;
	this->SendWelcomeMessage(client);
	return true;
}

void CSocketConnection::OnClientValidated(std::shared_ptr<olc::net::connection<ProtocolHead>> client)
{

}

void CSocketConnection::OnClientDisconnect(std::shared_ptr<olc::net::connection<ProtocolHead>> client)
{
	
}

void CSocketConnection::OnMessage(std::shared_ptr<olc::net::connection<ProtocolHead>> client, olc::net::message<ProtocolHead>& msg)
{
	uint16_t aIndex = client->GetID();

	gSocketManager.DataReceived(aIndex, msg);
}

void CSocketConnection::ProtocolSend(uint16_t aIndex, olc::net::message<ProtocolHead> msg)
{
	MessageClientByIndex(msg, aIndex);
}

void CSocketConnection::DisconnectClient(uint16_t aIndex)
{
	DisconnectClientByIndex(aIndex);
}

void CSocketConnection::SendWelcomeMessage(std::shared_ptr<olc::net::connection<ProtocolHead>> client) {
	PacketMsg lpMsg;
	PMSG_CHAR_BYTES pMsg = { 0 };

	const char* msg = "Welcome";

	strncpy(pMsg.bytes, msg, MAX_CHAR_MSG_SIZE - 1);
	pMsg.bytes[sizeof(pMsg.bytes) - 1] = '\0';

	std::cout << pMsg.bytes << std::endl;

	std::cout << "Send Welcome Message.\n";

	lpMsg.header.id = ProtocolHead::SERVER_WELCOME;

	lpMsg.body.resize(lpMsg.body.size() + sizeof(pMsg));

	std::memcpy(lpMsg.body.data(), (uint8_t*)&pMsg, sizeof(pMsg));

	lpMsg.header.size = lpMsg.size();
	
	std::cout << "Welcome Message: " << lpMsg.body.data() << std::endl;

	this->MessageClient(client, lpMsg);

	this->Update();
}