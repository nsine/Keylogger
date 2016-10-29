// Keylogger.Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER 1024

// argv[1] - server name
// argv[2] - port
int main(int argc, char *argv[])
{
	SOCKET clientSocket;

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << "\n";
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
		std::cerr << "getaddrinfo failed: " << result << "\n";
		WSACleanup();
	}

	clientSocket = socket(addr->ai_family, addr->ai_socktype,
		addr->ai_protocol);

	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Error at socket: " << WSAGetLastError() << "\n";
		freeaddrinfo(addr);
		WSACleanup();
	}

	auto bindResult = bind(clientSocket, addr->ai_addr, (int)addr->ai_addrlen);

	if (listen(clientSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(clientSocket);
		WSACleanup();
	}

	return 0;
}

