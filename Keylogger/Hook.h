#pragma once

#include <Windows.h>
#include <fstream>
#include <iostream>
#include <functional>


class Hook {
public:
	void SetHook(std::function<void(int)> keyboardCallback);
private:
	HHOOK hHook;

	static std::function<void(int)> callback;
	static LRESULT CALLBACK hookProc(const int nCode, const WPARAM wParam, const LPARAM lParam);
};

