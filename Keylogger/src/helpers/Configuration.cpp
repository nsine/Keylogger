#include "stdafx.h"
#include "Configuration.h"

void Configuration::InitConfiguration() {
	// Init relative pathes
	std::wstring logFilename = L"service_log.txt";
	std::wstring configFilename = L"service_data.ini";

	const int buffSize = 1024;
	wchar_t buff[buffSize];
	GetTempPath(buffSize, buff);
	Configuration::LogFilePath = std::wstring(buff) + L"\\" + logFilename;
	Configuration::AppDataPath = std::wstring(buff) + configFilename;

	GetWindowsDirectory(buff, buffSize);
	Configuration::ProgramPath =  std::wstring(buff) + L"\\" + Configuration::ProgramFilename;
}

std::wstring Configuration::LogFilePath;
std::wstring Configuration::AppDataPath;
std::wstring Configuration::ProgramPath;

std::wstring Configuration::ProgramFilename = L"service.exe";
std::wstring Configuration::ConnectorAddress = L"127.0.0.1";
std::wstring Configuration::ConnectorPort = L"5000";