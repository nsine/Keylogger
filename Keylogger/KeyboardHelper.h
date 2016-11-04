#pragma once

#include <string>
#include <map>
#include <windows.h>

class KeyboardHelper {
private:
	static std::map<int, std::string> keys;
public:
	static std::string getKeyByCode(int keyCode);
	static int getCodeByName(std::string keyName);
};

