#pragma once

#include <string>
#include <map>
#include <windows.h>
#include <algorithm>
#include <string>
#include "StringUtilities.h"


class KeyboardHelper {
private:
	static KeyboardHelper* instance;
	KeyboardHelper();
	std::map<int, std::wstring> keys;
public:
	static KeyboardHelper* getInstance();

	std::wstring getKeyByCode(int keyCode);
	std::wstring getNameOrUnicode(int vkCode, int scanCode, const BYTE* keyState);
	int getCodeByName(std::wstring keyName);
};	