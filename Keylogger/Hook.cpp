#include "stdafx.h"
#include "Hook.h"

using namespace std;

std::function<void(const wchar_t[])> Hook::callback;
HHOOK Hook::hHook;

void Hook::setHook(std::function<void(const wchar_t[])> keyboardCallback) {
	callback = keyboardCallback;
	std::cout << "set hook" << std::endl;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, hookProc, NULL, 0);
}

void Hook::unsetHook() {
	UnhookWindowsHookEx(hHook);
}

LRESULT CALLBACK Hook::hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	BYTE keyState[256] = { 0 };
	bool keyStateResult;
	std::wstring result;
	bool isKeyCombination = false;

	GetKeyState(NULL);
	keyStateResult = GetKeyboardState(keyState);

	PMSG pmsg = (PMSG)lParam;

	if (pmsg->message == WM_CHAR || wParam == WM_CHAR) {
		std::cout << "laalal " << pmsg->wParam << std::endl;
	}

	// Check alt combinations
	if (p->flags == 32 && p->vkCode == VK_TAB) {
		result = L"<ALT-TAB>";
		isKeyCombination = true;
	} else if (p->flags == 32 && p->vkCode == 160) {
		result = L"<ALT_SHIFT>";
		isKeyCombination = true;
	}

	if (!isKeyCombination && wParam != WM_KEYDOWN) {
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	}

	// Check control combinations
	int ctrlState = GetKeyState(VK_LCONTROL);
	if (ctrlState) {
		if (p->flags && LLKHF_ALTDOWN && p->vkCode == VK_DELETE) {
			result = L"<CTRL-ALT-DEL>";
			isKeyCombination = true;
		}
	}

	if (isKeyCombination) {
		callback(result.c_str());
		return CallNextHookEx(hHook, nCode, wParam, lParam);
	}

	result = getKeyNameByVkCode(p, keyState, true);
	callback(result.c_str());

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

std::wstring Hook::getKeyNameByVkCode(PKBDLLHOOKSTRUCT p, BYTE* keyState, bool escaped) {
	std::wstring result;

	switch (p->vkCode) {
	case VK_RETURN:
		if (escaped) {
			result = L"\n";
		} else {
			result = L"<ENTER>";
		}
		break;
	case VK_TAB:
		if (escaped) {
			result = L"\t";
		} else {
			result = L"<TAB>";
		}
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
		wchar_t buffer[5];

		ToUnicodeEx(p->vkCode, p->scanCode, keyState,
			buffer, _countof(buffer), 0, NULL);
		result = buffer;
	}
	return result;
}