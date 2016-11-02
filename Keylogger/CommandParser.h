#pragma once

#include "StringUtilities.h"

#include <map>
#include <string>
#include <functional>
#include <vector>


class CommandParser {
private:
	static std::map<std::string, std::function<std::string(std::string)>> commands;
public:
	static void addCommand(std::string name, std::function<std::string(std::string)> handler);
	static std::string act(std::string commandString);
};

