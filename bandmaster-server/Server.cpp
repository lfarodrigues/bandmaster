#include "SocketManager.h"

const int SERVER_PORT = 5000;

int main() {
	if (!gSocketManager.StartServer(SERVER_PORT)) {
		std::cout << "Could not start, port in use\n";
	}
	else {
		std::cout << "Server started.\n";
	}

	gSocketManager.mThread.join();

	PMSG_TESTE_SEND msg;

	msg.TickCount = GetTickCount();
	
	std::cout << "Server Disconnected\n";

	gSocketManager.PacketSendAll(ProtocolHead::SERVER_DISCONNECT, (uint8_t*)&msg, sizeof(msg));

	return 0;
}