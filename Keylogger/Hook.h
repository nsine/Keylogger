#pragma once

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif // !_WINSOCKAPI_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>

#include "KeyBlockService.h"
#include "KeyboardHelper.h"

class Hook {
public:
	void setHook(std::function<void(const wchar_t[])> keyboardCallback);
	void unsetHook();

	void blockKey(std::wstring keyName);
	void unblockKey(std::wstring keyName);
private:
	static HHOOK hHook;
	static std::vector<int> blockedKeys;
	static std::function<void(const wchar_t[])> callback;
	static LRESULT CALLBACK hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam);
	static std::wstring getKeyNameByVkCode(PKBDLLHOOKSTRUCT p, BYTE* keyState, bool escaped);
};
