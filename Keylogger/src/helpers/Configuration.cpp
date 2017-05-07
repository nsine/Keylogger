#include "stdafx.h"
#include "Configuration.h"

std::wstring Configuration::logFilename = L"service_log.txt";
std::wstring Configuration::programFilename = L"service.exe";

std::wstring Configuration::GetLogFilePath() {
	const int buffSize = 1024;
	wchar_t buff[buffSize];
	GetTempPath(buffSize, buff);
	return std::wstring(buff) + L"\\" + logFilename;
}

std::wstring Configuration::GetProgramPath() {
	const int buffSize = 1024;
	wchar_t buff[buffSize];
	GetWindowsDirectory(buff, buffSize);
	return std::wstring(buff) + L"\\" + programFilename;
}

std::wstring Configuration::GetProgramName() {
	return programFilename;
}

Configuration::Configuration() {}


Configuration::~Configuration() {}
