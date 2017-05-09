#include "stdafx.h"
#include "AppData.h"

#include "Configuration.h"

AppData::AppData() {
	std::wifstream configFile(Configuration::AppDataPath);
	if (!configFile.good()) {
		std::wofstream newConfigFile;
		newConfigFile.open(Configuration::AppDataPath, std::fstream::out);
		newConfigFile << L"NEW";
		newConfigFile.close();
	}

	//configFile.open(Configuration::AppDataPath);

	std::wstring lineFromFile;
	configFile >> lineFromFile;
	this->ComputerId = lineFromFile;
}

bool AppData::save() {
	std::wofstream configFile;
	configFile.open(Configuration::AppDataPath, std::fstream::out);
	configFile << this->ComputerId;
	configFile.close();
	return true;
}

AppData & AppData::getInstance() {
	static AppData instance;
	return instance;
}