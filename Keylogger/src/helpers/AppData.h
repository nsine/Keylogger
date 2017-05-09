#pragma once
class AppData {
private:
	AppData();
public:
	std::wstring ComputerId;

	static AppData& getInstance();

	bool save();

	AppData(AppData const&) = delete;
	void operator=(AppData const&) = delete;
};

