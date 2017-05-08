#pragma once

enum TokenType {
	HEADER,
	ALTSHIFT,
	BS,
	COMBINATION,
	SYM
};

struct Token {
	int type;
	std::wstring value;
};

class DataParser {
private:
	std::vector<Token> tokens;
	std::vector<std::wstring> matchers;

	std::pair<Token, int> getToken(std::wstring source);
public:
	DataParser(std::wstring source);
	std::wstring getEscaped();
	~DataParser();
};

