#include "Protocol.h"

BOOL g_bGameServerConnected = FALSE;
const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 5000;

int main() {
	if (gProtocol.ConnectServer((char*)SERVER_IP, SERVER_PORT)) {
		g_bGameServerConnected = TRUE;
		
		gProtocol.SendPingTest();
	}


	while (1) {
		gProtocol.RecvMessage();
	}

	return 0;
}