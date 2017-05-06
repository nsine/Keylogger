#include "stdafx.h"
#include "ComputerInfoHelper.h"

#include "StringHelper.h"

ComputerInfoHelper* ComputerInfoHelper::instance;

ComputerInfoHelper::ComputerInfoHelper() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    char ac[80];
    if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
        std::cerr << "Error " << WSAGetLastError() <<
            " when getting local host name." << std::endl;
    } else {
        this->hostName = StringHelper::s2ws(ac);
    }

    struct hostent *phe = gethostbyname(ac);
    if (phe == 0) {
        std::cerr << "Yow! Bad host lookup." << std::endl;
    }

    std::string ip = "";
    for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
        struct in_addr addr;
        memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));

        std::string possibleIp = inet_ntoa(addr);
        if (possibleIp.find('.') == 2 && possibleIp.at(0) == '1' || ip == "") {
            ip = possibleIp;
        }
    }

    this->ip = StringHelper::s2ws(ip);
    this->port = PORT;
    WSACleanup();
}


ComputerInfoHelper* ComputerInfoHelper::getInstance() {
    if (instance == nullptr) {
        instance = new ComputerInfoHelper();
    }
    return instance;
}

std::wstring ComputerInfoHelper::getHostName() {
    return this->hostName;
}

std::wstring ComputerInfoHelper::getIp() {
    return this->ip;
}

std::wstring ComputerInfoHelper::getPort() {
    return this->port;
}