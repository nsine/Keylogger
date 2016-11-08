#include "stdafx.h"
#include "KeyboardHelper.h"

KeyboardHelper* KeyboardHelper::instance;

KeyboardHelper::KeyboardHelper() {
	this->keys = {
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

	for (int scanCode = 0x30; scanCode <= 0x39; scanCode++) {
		wchar_t c = scanCode;
		this->keys.insert(std::pair<int, std::wstring>(scanCode, std::wstring(1, c)));
	}

	for (int scanCode = 0x41; scanCode <= 0x5A; scanCode++) {
		wchar_t c = scanCode;
		this->keys.insert(std::pair<int, std::wstring>(scanCode, std::wstring(1, c)));
	}
}

KeyboardHelper* KeyboardHelper::getInstance() {
	if (instance == nullptr) {
		instance = new KeyboardHelper();
	}
	return instance;
}

std::wstring KeyboardHelper::getKeyByCode(int keyCode) {
	auto result = this->keys.find(keyCode);
	if (result == this->keys.end()) {
		return L"";
	}
	return result->second;
}

std::wstring KeyboardHelper::getNameOrUnicode(int vkCode, int scanCode, const BYTE* keyState) {
	// if letter or digit
	if (vkCode >= 0x41 && vkCode <= 0x5A || vkCode >= 0x30 && vkCode <= 0x39) {
		wchar_t buffer[5];
		ToUnicodeEx(vkCode, scanCode, keyState,
			buffer, _countof(buffer), 0, nullptr);
		return std::wstring(buffer);
	} else {
		return this->getKeyByCode(vkCode);
	}
}

int KeyboardHelper::getCodeByName(std::wstring keyName) {
	std::wstring name = StringUtilities::toLower(keyName);
	auto result = std::find_if(this->keys.begin(), this->keys.end(), [name](std::pair<int, std::wstring> it) -> bool {
		return it.second.compare(name) == 0;
	});

	if (result == this->keys.end()) {
		return 0;
	}
	return result->first;
}
