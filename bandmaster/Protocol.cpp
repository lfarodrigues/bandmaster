#include "Protocol.h"

CProtocol gProtocol;

CProtocol::CProtocol() {

}

bool CProtocol::ConnectServer(char* IP, uint16_t Port) {
	this->SocketConnect = new CustomClient;
	if (this->SocketConnect->Connect(IP, Port)) return true;

	return false;
}

void CProtocol::DisconnectServer() {
	if (this->SocketConnect) {
		this->SocketConnect->Disconnect();
	}
}

bool CProtocol::CheckConnected()
{
	if (this->SocketConnect && this->SocketConnect->IsConnected())
		return true;
	return false;
}

void CProtocol::RecvMessage() {
	if (gProtocol.SocketConnect && gProtocol.SocketConnect->IsConnected()) {
		while (true) {
			if (!gProtocol.SocketConnect->Incoming().empty())
			{
				std::cout << "Reading a message.\n";

				auto msg = gProtocol.SocketConnect->Incoming().pop_front().msg;
				
				std::cout << "Message Header ID: " << (uint16_t)msg.header.id << std::endl;

				switch (msg.header.id) {
					case ProtocolHead::SERVER_DISCONNECT:
					{
						// Server has responded to a ping request	
						DWORD clientID;
						msg >> clientID;
						std::cout << "Hello from [" << clientID << "]\n";
						break;
					}
					case ProtocolHead::SERVER_WELCOME:
					{
						char* msgContent = new char[msg.body.size()];
						std::memcpy(msgContent, msg.body.data(), msg.body.size());
						// Read the string
						std::cout << "Conteudo da mensagem: " << msgContent << std::endl;
						break;
					}
				}
			}
			else {
				break;
			}
		}
	}
}

void CProtocol::SendPingTest()
{
	this->SocketConnect->PingServer();
}