#include "stdafx.h"
#include "KeyBlockService.h"

#include "helpers/KeyboardHelper.h"

KeyBlockService* KeyBlockService::instance;

KeyBlockService* KeyBlockService::getInstance() {
    if (instance == nullptr) {
        instance = new KeyBlockService();
    }

    return instance;
}

bool KeyBlockService::isBlocked(int keyCode) {
    auto found = this->blockedKeys.find(keyCode);
    if (found == this->blockedKeys.end()) {
        return false;
    }
    return true;
}

bool KeyBlockService::isBlocked(std::wstring keyName) {
    int keyCode = KeyboardHelper::getInstance()->getCodeByName(keyName);
    return this->isBlocked(keyCode);
}

void KeyBlockService::addBlockedKey(int keyCode) {
    this->blockedKeys.insert(keyCode);
}

void KeyBlockService::addBlockedKey(std::wstring keyName) {
    if (keyName == L"") {
        return;
    }
    int keyCode = KeyboardHelper::getInstance()->getCodeByName(keyName);
    this->addBlockedKey(keyCode);
}

void KeyBlockService::removeBlockedKey(int keyCode) {
    this->blockedKeys.erase(keyCode);
}

void KeyBlockService::removeBlockedKey(std::wstring keyName) {
    int keyCode = KeyboardHelper::getInstance()->getCodeByName(keyName);
    this->removeBlockedKey(keyCode);
}

std::vector<std::wstring> KeyBlockService::getBlockedList() {
    auto result = std::vector<std::wstring>();
    for (auto it = this->blockedKeys.begin(); it != this->blockedKeys.end(); it++) {
        result.push_back(KeyboardHelper::getInstance()->getKeyByCode(*it, *it, nullptr));
    }
    return result;
}

KeyBlockService::KeyBlockService() {
    this->blockedKeys = {};
}
