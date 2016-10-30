// Keylogger.Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <regex>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 1024

struct connectionParams {
	std::string address;
	std::string port;
};

connectionParams* parseConnectionParams(std::string connectString);
void connectToServer(connectionParams params);
void disconnectFromServer();

SOCKET serverSocket;
connectionParams* params;

// argv[1] - server name
// argv[2] - port
int main(int argc, char *argv[])
{
	serverSocket = INVALID_SOCKET;

	// Specify input welcome header
	std::string computerName = "";

	while (true) {
		std::cout << computerName << " $ ";
		std::string userInput;
		char userInputBuffer[BUFFER_SIZE];
		std::cin.getline(userInputBuffer, BUFFER_SIZE);
		userInput = std::string(userInputBuffer);

		if (serverSocket == INVALID_SOCKET) {
			if (userInput.substr(0, 7).compare("connect") != 0) {
				std::cout << "You should connect to server" << std::endl <<
					"Use connect <server_address> <port>" << std::endl;
				continue;
			}

			params = parseConnectionParams(userInput);
			if (params == nullptr) {
				std::cout << "Use connect <server_address> <port>" << std::endl;
				continue;
			}

			connectToServer(*params);
			if (serverSocket != INVALID_SOCKET) {
				computerName = params->address + ":" + params->port;
			}
			continue;
		}

		int sendResult;
		int tries = 0;
		// Try to send data 2 times
		do {
			sendResult = send(serverSocket, userInput.c_str(), userInput.size(), 0);
			if (sendResult == SOCKET_ERROR) {
				tries++;
				std::cout << "Reconnecting..." << std::endl;
				disconnectFromServer();
				connectToServer(*params);
				if (serverSocket == INVALID_SOCKET) {
					computerName = "";
				}
			} else {
				break;
			}
		} while (tries < 2);
		
		if (sendResult == SOCKET_ERROR) {
			std::cout << "Disconnected." << std::endl;
			continue;
		}

		char responseBuffer[BUFFER_SIZE];

		int receivedSize = recv(serverSocket, responseBuffer, BUFFER_SIZE, 0);
		if (receivedSize == -1) {
			std::cerr << "Error receiving" << std::endl;
		}

		responseBuffer[receivedSize] = '\0';

		std::cout << responseBuffer << std::endl;

		if (userInput == "exit" && receivedSize != -1) {
			disconnectFromServer();
			computerName = "";
		}
	}

	return 0;
}

connectionParams* parseConnectionParams(std::string connectString) {
	std::vector<std::string> tokens;
	std::regex separator(" ");

	//start/end points of tokens in str
	std::sregex_token_iterator
		begin(connectString.begin(), connectString.end(), separator, -1),
		end;

	std::copy(begin, end, std::back_inserter(tokens));

	if (tokens.size() != 3) {
		return nullptr;
	}

	auto params = new connectionParams();
	params->address = tokens[1];
	params->port = tokens[2];
	return params;
}

void connectToServer(connectionParams params) {
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << "\n";
		return;
	}

	struct addrinfo* addr = nullptr;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(params.address.c_str(), params.port.c_str(), &hints, &addr);

	if (result != 0) {
		std::cerr << "Can't find this address. Error code: " << result << "\n";
		WSACleanup();
		return;
	}

	serverSocket = socket(addr->ai_family, addr->ai_socktype,
		addr->ai_protocol);

	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Error at socket: " << WSAGetLastError() << "\n";
		freeaddrinfo(addr);
		WSACleanup();
		return;
	}

	int connectResult = connect(serverSocket, addr->ai_addr, (int)addr->ai_addrlen);
	if (connectResult == SOCKET_ERROR) {
		std::cerr << "Can't find socket on this address. Error code: " << WSAGetLastError() << "\n";
		serverSocket = INVALID_SOCKET;
		WSACleanup();
		return;
	}

	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "Error at socket: " << WSAGetLastError() << "\n";
		WSACleanup();
		return;
	}
}

void disconnectFromServer() {
	closesocket(serverSocket);
	serverSocket = INVALID_SOCKET;
	WSACleanup();
}