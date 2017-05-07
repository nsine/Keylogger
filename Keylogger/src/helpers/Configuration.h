#pragma once

class Configuration {
private:
	static std::wstring logFilename;
	static std::wstring programFilename;
public:
	static std::wstring GetLogFilePath();
	static std::wstring GetProgramPath();
	static std::wstring GetProgramName();

	Configuration();
	~Configuration();
};

