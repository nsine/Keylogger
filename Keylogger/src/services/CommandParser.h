#pragma once

class CommandParser {
private:
	static std::map<std::wstring, std::function<std::wstring(std::wstring)>> commands;
public:
	static void addCommand(std::wstring name, std::function<std::wstring(std::wstring)> handler);
	static std::wstring act(std::wstring commandString);
};

