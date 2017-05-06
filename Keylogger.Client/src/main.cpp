#include "stdafx.h"

#define BUFFER_SIZE 100000

struct connectionParams {
	std::wstring address;
	std::wstring port;
};

connectionParams* parseConnectionParams(std::wstring connectString);
void connectToServer(connectionParams params);
void disconnectFromServer();
std::string ws2s(const std::wstring& wstr);
std::wstring s2ws(const std::string& str);

SOCKET serverSocket;
connectionParams* params;

// argv[1] - server name
// argv[2] - port
int main(int argc, char *argv[])
{
	_setmode(_fileno(stdout), _O_U8TEXT);
	serverSocket = INVALID_SOCKET;

	// Specify input welcome header
	std::wstring computerName = L"";

	while (true) {
		std::wcout << computerName << " $ ";
		std::wstring userInput;
		wchar_t userInputBuffer[BUFFER_SIZE];
		std::wcin.getline(userInputBuffer, BUFFER_SIZE);
		userInput = std::wstring(userInputBuffer);

		if (serverSocket == INVALID_SOCKET) {
			if (userInput.substr(0, 7).compare(L"connect") != 0) {
				std::wcout << L"You should connect to server" << std::endl <<
					L"Use connect <server_address> <port>" << std::endl;
				continue;
			}

			params = parseConnectionParams(userInput);
			if (params == nullptr) {
				std::wcout << L"Use connect <server_address> <port>" << std::endl;
				continue;
			}

			connectToServer(*params);
			if (serverSocket != INVALID_SOCKET) {
				computerName = params->address + L":" + params->port;
			}
			continue;
		}

		int sendResult;
		int tries = 0;
		// Try to send data 2 times
		do {
			sendResult = send(serverSocket, (char*)userInput.c_str(), sizeof(wchar_t) * userInput.size(), 0);
			if (sendResult == SOCKET_ERROR) {
				tries++;
				std::wcout << L"Reconnecting..." << std::endl;
				disconnectFromServer();
				connectToServer(*params);
				if (serverSocket == INVALID_SOCKET) {
					computerName = L"";
				}
			} else {
				break;
			}
		} while (tries < 2);
		
		if (sendResult == SOCKET_ERROR) {
			std::wcout << L"Disconnected." << std::endl;
			continue;
		}

		wchar_t responseBuffer[BUFFER_SIZE];

		int receivedSize = recv(serverSocket, (char*)responseBuffer, sizeof(wchar_t) * BUFFER_SIZE, 0);
		if (receivedSize == -1) {
			std::cerr << "Error receiving" << std::endl;
		}

		responseBuffer[receivedSize / 2] = '\0';

		std::wcout << responseBuffer << std::endl;

		if (userInput == L"exit" && receivedSize != -1) {
			disconnectFromServer();
			computerName = L"";
		}
	}

	return 0;
}

connectionParams* parseConnectionParams(std::wstring connectString) {
	std::vector<std::wstring> tokens;
	std::wregex separator(L" ");

	//start/end points of tokens in str
	std::wsregex_token_iterator
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

	const char* ad = ws2s(params.address).c_str();

	//result = getaddrinfo("127.0.0.1", "8123", &hints, &addr);
	result = getaddrinfo(ws2s(params.address).c_str(), ws2s(params.port).c_str(), &hints, &addr);

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

std::wstring s2ws(const std::string& str) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}