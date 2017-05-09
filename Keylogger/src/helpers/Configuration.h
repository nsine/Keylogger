#pragma once

class Configuration {
public:
	static void InitConfiguration();

	static std::wstring ProgramFilename;
	static std::wstring ConnectorAddress;
	static std::wstring ConnectorPort;
	static std::wstring AppDataPath;
	static std::wstring LogFilePath;
	static std::wstring ProgramPath;
};

