#pragma once

class Hook {
public:
	void setHook(std::function<void(const wchar_t[])> keyboardCallback);
	void unsetHook();

	void blockKey(std::wstring keyName);
	void unblockKey(std::wstring keyName);
private:
	static HHOOK hHook;
	static std::vector<int> blockedKeys;
	static std::function<void(const wchar_t[])> callback;
	static LRESULT CALLBACK hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam);
	static std::wstring getKeyNameByVkCode(PKBDLLHOOKSTRUCT p, BYTE* keyState, bool escaped);
};
