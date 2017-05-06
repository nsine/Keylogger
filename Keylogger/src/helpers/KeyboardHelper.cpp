#include "stdafx.h"
#include "KeyboardHelper.h"

#include "StringHelper.h"

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
        { VK_NEXT, L"<PGDOWN>" },
        { VK_PRIOR, L"<PGUP>" },
        { VK_END, L"<END>" },
        { VK_HOME, L"<HOME>" },
        { VK_VOLUME_MUTE, L"<VOLUME_MUTE>" },
        { VK_VOLUME_DOWN, L"<VOLUME_DOWN>" },
        { VK_VOLUME_UP, L"<VOLUME_UP>" },
        { VK_MEDIA_PLAY_PAUSE, L"<MEDIA_PLAY_PAUSE>" },
        { VK_MEDIA_STOP, L"<MEDIA_STOP>" },
        { VK_LEFT, L"<ARR_LEFT>" },
        { VK_RIGHT, L"<ARR_RIGHT>" },
        { VK_UP, L"<ARR_UP>" },
        { VK_DOWN, L"<ARR_DOWN>" },
    };
}

KeyboardHelper* KeyboardHelper::getInstance() {
    if (instance == nullptr) {
        instance = new KeyboardHelper();
    }
    return instance;
}

std::wstring KeyboardHelper::getKeyByCode(int vkCode, int scanCode, const BYTE* keyState) {
    auto result = this->keys.find(vkCode);

    if (result == this->keys.end()) {
        if (keyState == nullptr) {
            wchar_t resultChar = vkCode;
            return std::wstring(1, resultChar);
        }

        wchar_t buffer[16];
        HKL hkl;
        DWORD dwThreadId;
        DWORD dwProcessId;
        HWND hWindowHandle;

        hWindowHandle = GetForegroundWindow();
        dwThreadId = GetWindowThreadProcessId(hWindowHandle, &dwProcessId);
        hkl = GetKeyboardLayout(dwThreadId);

        ToUnicodeEx(vkCode, scanCode, keyState,
            buffer, _countof(buffer), 0, hkl);
        return std::wstring(buffer);
    }
    return result->second;
}

int KeyboardHelper::getCodeByName(std::wstring keyName) {
    std::wstring name = keyName;
    auto result = std::find_if(this->keys.begin(), this->keys.end(), [name](std::pair<int, std::wstring> it) -> bool {
        return it.second.compare(name) == 0;
    });

    if (result == this->keys.end()) {
        return name.c_str()[0];
    }
    return result->first;
}
