#include "stdafx.h"
#include "SocketServer.h"

SocketServer::SocketServer(Keylogger* logger) {
	listenSocket = INVALID_SOCKET;

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

	int bindResult = ::bind(this->listenSocket, addr->ai_addr, (int)addr->ai_addrlen);

	if (listen(this->listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		cerr << "listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(this->listenSocket);
		this->listenSocket = INVALID_SOCKET;
		WSACleanup();
	}
}

void SocketServer::start() {
	if (this->listenSocket == INVALID_SOCKET) {
		cerr << "socket is not active" << std::endl;
		return;
	}

	char requestBuffer[BUFFER_SIZE];
	int clientSocket = INVALID_SOCKET;

	while (true) {
		clientSocket = accept(this->listenSocket, NULL, NULL);
		std::cout << "accepted client: " << clientSocket << std::endl;

		if (clientSocket == INVALID_SOCKET) {
			cerr << "accept failed: " << WSAGetLastError() << std::endl;
			closesocket(this->listenSocket);
			WSACleanup();
			continue;
		}

		while (true) {
			int receivedSize = recv(clientSocket, requestBuffer, BUFFER_SIZE, 0);
			
			
			std::string response;

			if (receivedSize == SOCKET_ERROR) {
				cerr << "recv failed: " << receivedSize << std::endl;
				break;
			} else if (receivedSize == 0) {
				cerr << "connection closed..." << std::endl;
			} else if (receivedSize > 0) {
				requestBuffer[receivedSize] = '\0';
				std::string request = std::string(requestBuffer);
				std::cout << "received: " << request << std::endl;

				response = this->getResponse(request);

				int sendResult = send(clientSocket, response.c_str(),
					response.size(), 0);

				if (request.compare("exit") == 0) {
					break;
				}

				if (sendResult == SOCKET_ERROR) {
					cerr << "send failed: " << WSAGetLastError() << std::endl;
				}
			}
		}

		closesocket(clientSocket);
	}

}

std::string SocketServer::getResponse(std::string request) {
	std::stringstream responseStream;
	responseStream << request << " ok";
	return responseStream.str();
}


SocketServer::~SocketServer() {}
