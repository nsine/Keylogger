#pragma once

class Hook {
public:
	void setHook(std::function<void(const wchar_t[])> keyboardCallback);
	void unsetHook();

	void enable();
	void disable();
	bool checkEnabled();
private:
	static HHOOK hHook;
	static std::vector<int> blockedKeys;
	static bool isEnabled;
	static std::function<void(const wchar_t[])> callback;

	static LRESULT CALLBACK hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam);
	static std::wstring getKeyNameByVkCode(PKBDLLHOOKSTRUCT p, BYTE* keyState, bool escaped);
};
