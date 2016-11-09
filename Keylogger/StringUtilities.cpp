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

std::string StringUtilities::ws2s(std::wstring& wstr) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::wstring StringUtilities::s2ws(const std::string& str) {
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::wstring StringUtilities::toLower(std::wstring str) {
	std::wstring resultStr;
	std::transform(str.begin(), str.end(), resultStr.begin(), ::tolower);
	return resultStr;
}