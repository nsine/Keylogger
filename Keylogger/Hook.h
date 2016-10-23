#pragma once

#include <Windows.h>
#include <iostream>
#include <functional>


class Hook {
public:
	void SetHook(std::function<void(wchar_t[])> keyboardCallback);
private:
	HHOOK hHook;

	static std::function<void(wchar_t[])> callback;
	static LRESULT CALLBACK hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam);
};

