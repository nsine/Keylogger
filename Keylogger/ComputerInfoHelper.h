#pragma once

#define PORT L"8123"

class ComputerInfoHelper {
private:
	ComputerInfoHelper();
	static ComputerInfoHelper* instance;

	std::wstring hostName;
	std::wstring ip;
	std::wstring port;
public:
	static ComputerInfoHelper* getInstance();
	std::wstring getHostName();
	std::wstring getIp();
	std::wstring getPort();
};

