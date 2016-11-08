#include "stdafx.h"
#include "StringUtilities.h"

#include <algorithm>
#include <iterator>
#include <regex>

char StringUtilities::charBuffer[1024];

std::vector<std::wstring> StringUtilities::splitString(std::wstring str, std::wstring sep) {
	std::vector<std::wstring> tokens;
	std::wregex separator(sep);

	//start/end points of tokens in str
	std::wsregex_token_iterator
		begin(str.begin(), str.end(), separator, -1),
		end;

	std::copy(begin, end, std::back_inserter(tokens));
	return tokens;
}

std::wstring StringUtilities::trim(std::wstring str) {
	return str;
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

std::wstring StringUtilities::wcharToStr(const wchar_t* str) {
	//CharToOemW(str, charBuffer);
	return std::wstring(str);
}

const char* StringUtilities::wstrToChars(std::wstring str) {
	return (std::string((const char*)&str[0], sizeof(wchar_t) / sizeof(char)*str.size())).c_str();
}
