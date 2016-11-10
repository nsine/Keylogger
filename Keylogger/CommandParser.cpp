#include "stdafx.h"
#include "CommandParser.h"

std::map<std::wstring, std::function<std::wstring(std::wstring)>> CommandParser::commands;

void CommandParser::addCommand(std::wstring name, std::function<std::wstring(std::wstring)> handler) {
	commands.insert(std::pair<std::wstring, std::function<std::wstring(std::wstring)>>(name, handler));
}

std::wstring CommandParser::act(std::wstring commandString) {
	auto tokens = StringHelper::splitString(commandString, L" ");
	auto commandName = tokens.size() > 0 ? tokens[0] : L"";
	auto command = commands.find(commandName);
	if (command == commands.end()) {
		return L"Command not found";
	}
	
	auto argumentsLine = StringHelper::trim(commandString.substr(commandName.size(),
		commandString.size() - commandName.size()));
	auto commandFunc = (*command).second;
	return commandFunc(argumentsLine);
}
