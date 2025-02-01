#pragma once

enum class ProtocolHead : uint16_t
{
	CLIENT_ACCEPT,			//Enviado pelo cliente
	CLIENT_LIVE_CLIENT,

	SERVER_CONNECT,			//Enviado pelo servidor
	SERVER_DISCONNECT,
	SERVER_WELCOME,

	BOTH_CONNECT_LOGIN,			//Enviado pelo cliente e servidor
	BOTH_CONNECT_CHARACTER,
	BOTH_POSITION,
	BOTH_MOVE,

	BOTH_MESSAGE,
};