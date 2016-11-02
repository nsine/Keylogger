#pragma once

#include <string>
#include <vector>


class StringUtilities {
public:
	static std::vector<std::string> splitString(std::string str, std::string sep);
	static std::string trim(std::string str);
};

