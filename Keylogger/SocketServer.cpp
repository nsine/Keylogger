#include "stdafx.h"
#include "SocketServer.h"

#include <iostream>

SocketServer::SocketServer(Keylogger* logger) {
	std::cout << "socket";
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cerr << "WSAStartup failed: " << result << "\n";
	}

	struct addrinfo* addr = nullptr;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_protocol = IPPROTO_TCP; 

	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo("127.0.0.1", "8123", &hints, &addr);

	if (result != 0) {
		cerr << "getaddrinfo failed: " << result << "\n";
		WSACleanup();
	}

	this->listenSocket = socket(addr->ai_family, addr->ai_socktype,
		addr->ai_protocol);

	if (this->listenSocket == INVALID_SOCKET) {
		cerr << "Error at socket: " << WSAGetLastError() << "\n";
		freeaddrinfo(addr);
		WSACleanup();
	}

	auto bindResult = bind(this->listenSocket, addr->ai_addr, (int)addr->ai_addrlen);

	if (listen(this->listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cerr << "listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(this->listenSocket);
		WSACleanup();
	}
}

void SocketServer::start() {
	char requestBuffer[BUFFER_SIZE];
	int clientSocket = INVALID_SOCKET;

	for (;;) {
		// Принимаем входящие соединения
		clientSocket = accept(this->listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			cerr << "accept failed: " << WSAGetLastError() << "\n";
			closesocket(this->listenSocket);
			WSACleanup();
		}

		int result = recv(clientSocket, requestBuffer, BUFFER_SIZE, 0);

		std::stringstream response;
		std::stringstream response_body;

		if (result == SOCKET_ERROR) {
			cerr << "recv failed: " << result << "\n";
			closesocket(clientSocket);
		} else if (result == 0) {
			cerr << "connection closed...\n";
		} else if (result > 0) {
			requestBuffer[result] = '\0';

			response_body << requestBuffer << " ok";

			result = send(clientSocket, response.str().c_str(),
				response.str().length(), 0);

			if (result == SOCKET_ERROR) {
				cerr << "send failed: " << WSAGetLastError() << "\n";
			}

			closesocket(clientSocket);
		}
	}
}


SocketServer::~SocketServer() {}
