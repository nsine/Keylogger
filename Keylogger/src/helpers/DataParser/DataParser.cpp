#include "stdafx.h"
#include "DataParser.h"

std::pair<Token, int> DataParser::getToken(std::wstring str) {
	if (str.find(L"<ARR_UP><BS>") == 0) {
		auto a = 5;
	}
	if (str.size() == 0) {
		return std::pair<Token, int>(Token(), -1);
	}

	for (int i = 0; i < this->matchers.size(); i++) {
		std::wregex tokenRegex(matchers[i]);
		
		std::wsmatch m;
		std::regex_search(str, m, tokenRegex);
		if (m.size() != 0 && m.prefix() == L"") {
			Token token = {
				i,
				m[0]
			};

			return std::pair<Token, int>(token, m[0].str().size());
		}
	}

	return std::pair<Token, int>();
}

DataParser::DataParser(std::wstring source) {
	this->matchers.push_back(L"\r\n###(.+)?###(.+)?###\r\n");
	this->matchers.push_back(L"<ALT-SHIFT>");
	this->matchers.push_back(L"<BS>");
	this->matchers.push_back(L"<(.+?)>");
	this->matchers.push_back(L"(.|\n)");

	auto str = std::wstring(source);

	while (true) {
		auto result = this->getToken(str);
		int position = result.second;

		if (position == -1) {
			break;
		} else {
			str = str.substr(position);
		}

		tokens.push_back(result.first);
	}
}

std::wstring DataParser::getEscaped() {
	// TODO: optimize this shit

	std::vector<Token> resultTokens;

	for (auto token : tokens) {
		switch (token.type) {
		case HEADER:
		case SYM:
		case COMBINATION:
			resultTokens.push_back(token);
			break;
		case ALTSHIFT:
			break;
		case BS:
			if (resultTokens[resultTokens.size() - 1].value == L"к") {
				auto a = 5;
			}
			if (resultTokens.size() == 0) {
				break;
			} else if (resultTokens[resultTokens.size() - 1].type == SYM) {
				resultTokens.pop_back();
			} else {
				resultTokens.push_back(token);
			}
		}
	}

	std::wstringstream resultString;
	for (auto token : resultTokens) {
		resultString << token.value;
	}

	return resultString.str();
}


DataParser::~DataParser() {}
