#pragma once

#include <string>
#include <vector>
#include <windows.h>


class StringUtilities {
private:
	static char charBuffer[1024];
public:
	static std::vector<std::string> splitString(std::string str, std::string sep);
	static std::string trim(std::string str);
	static std::string wcharToStr(const wchar_t* str);
};

