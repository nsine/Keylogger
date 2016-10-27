#include "stdafx.h"
#include "SocketServer.h"
#include <WS2spi.h>


SocketServer::SocketServer() {
	WSADATA WsaData;
	int err = WSAStartup(0x0101, &WsaData);
	if (err == SOCKET_ERROR) {
		printf("WSAStartup() failed: %ld\n", GetLastError());
	}
}


SocketServer::~SocketServer() {}
