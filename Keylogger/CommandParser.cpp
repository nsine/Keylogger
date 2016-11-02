#include "stdafx.h"
#include "CommandParser.h"

std::map<std::string, std::function<std::string(std::string)>> CommandParser::commands;

void CommandParser::addCommand(std::string name, std::function<std::string(std::string)> handler) {
	commands.insert(std::pair<std::string, std::function<std::string(std::string)>>(name, handler));
}

std::string CommandParser::act(std::string commandString) {
	auto tokens = StringUtilities::splitString(commandString, " ");
	auto commandName = tokens.size() > 0 ? tokens[0] : "";
	auto command = commands.find(commandName);
	if (command == commands.end()) {
		return "Command not found";
	}
	
	auto argumentsLine = StringUtilities::trim(commandString.substr(commandName.size(),
		commandString.size() - commandName.size()));
	auto commandFunc = (*command).second;
	return commandFunc(argumentsLine);
}
