#pragma once

#include <string>
#include <vector>
#include <windows.h>


class StringUtilities {
private:
	static char charBuffer[1024];
public:
	static std::vector<std::wstring> splitString(std::wstring str, std::wstring sep);
	static std::wstring trim(std::wstring str);
	static std::wstring wcharToStr(const wchar_t* str);
	static const char* wstrToChars(std::wstring str);
};

