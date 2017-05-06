#pragma once

class KeyboardHelper {
private:
	static KeyboardHelper* instance;
	KeyboardHelper();
	std::map<int, std::wstring> keys;
public:
	static KeyboardHelper* getInstance();

	std::wstring getKeyByCode(int vkCode, int scanCode, const BYTE* keyState);
	int getCodeByName(std::wstring keyName);
};	