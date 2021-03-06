#pragma once

class KeyBlockService {
private:
	static KeyBlockService* instance;
	KeyBlockService();

	std::set<int> blockedKeys;
public:
	static KeyBlockService* getInstance();

	bool isBlocked(int keyCode);
	bool isBlocked(std::wstring keyName);
	void addBlockedKey(int keyCode);
	void addBlockedKey(std::wstring keyName);
	void removeBlockedKey(int keyCode);
	void removeBlockedKey(std::wstring keyName);
	std::vector<std::wstring> getBlockedList();
};

