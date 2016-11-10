#pragma once

#include "StringHelper.h"

#include <map>
#include <string>
#include <functional>
#include <vector>


class CommandParser {
private:
	static std::map<std::wstring, std::function<std::wstring(std::wstring)>> commands;
public:
	static void addCommand(std::wstring name, std::function<std::wstring(std::wstring)> handler);
	static std::wstring act(std::wstring commandString);
};

