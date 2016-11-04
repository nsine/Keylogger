#include "stdafx.h"
#include "KeyboardHelper.h"

std::map<int, std::string> KeyboardHelper::keys = {
	{ VK_RETURN, L"<ENTER>" },
	{ VK_TAB, L"<TAB>" },
	{ VK_LWIN, L"<WIN>" },
	{ VK_BACK, L"<BS>" },
	{ VK_DELETE, L"<DEL>" },
	{ VK_ESCAPE, L"<ESC>" },
	{ VK_F1, L"<F1>" },
	{ VK_F2, L"<F2>" },
	{ VK_F3, L"<F3>" },
	{ VK_F4, L"<F4>" },
	{ VK_F5, L"<F5>" },
	{ VK_F6, L"<F6>" },
	{ VK_F7, L"<F7>" },
	{ VK_F8, L"<F8>" },
	{ VK_F9, L"<F9>" },
	{ VK_F10, L"<F10>" },
	{ VK_F11, L"<F11>" },
	{ VK_F12, L"<F12>" },
};

std::string KeyboardHelper::getKeyByCode(int keyCode) {
	return std::string();
}

int KeyboardHelper::getCodeByName(std::string keyName) {
	return 0;
}
