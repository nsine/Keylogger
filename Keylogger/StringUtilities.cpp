#include "stdafx.h"
#include "StringUtilities.h"

#include <algorithm>
#include <iterator>
#include <regex>

char StringUtilities::charBuffer[1024];

std::vector<std::string> StringUtilities::splitString(std::string str, std::string sep) {
	std::vector<std::string> tokens;
	std::regex separator(sep);

	//start/end points of tokens in str
	std::sregex_token_iterator
		begin(str.begin(), str.end(), separator, -1),
		end;

	std::copy(begin, end, std::back_inserter(tokens));
	return tokens;
}

std::string StringUtilities::trim(std::string str) {
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

std::string StringUtilities::wcharToStr(const wchar_t* str) {
	CharToOemW(str, charBuffer);
	return std::string(charBuffer);
}
