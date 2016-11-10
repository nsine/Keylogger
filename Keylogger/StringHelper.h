#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <codecvt>
#include <locale>


class StringHelper {
private:
	static char charBuffer[1024];
public:
	static std::vector<std::wstring> splitString(std::wstring str, std::wstring sep);
	static std::wstring trim(std::wstring str);
	static std::wstring wcharToStr(const wchar_t* str);
	static std::string ws2s(std::wstring& wstr);
	static std::wstring s2ws(const std::string& str);
	static std::wstring toLower(std::wstring str);
};

