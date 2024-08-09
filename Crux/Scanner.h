#pragma once

#include <iostream>
#include "Token.h"
#include <vector>
#include <map>

class Scanner {
public:
	Scanner(const std::string& source) : m_source(source) {}
	std::vector<Token> scanTokens();
	void scanToken();
	void addToken(TokenType type);
	void addToken(TokenType type, Literal literal);
	void string();
	void number();
	void identifier();

private:
	const std::string& m_source;
	int m_start = 0;
	int m_current = 0;
	int m_line = 1;

	std::map<std::string, TokenType> m_keywords{
		{"and", TokenType::AND},
		{"class", TokenType::CLASS},
		{"else", TokenType::ELSE},
		{"false", TokenType::FALSE},
		{"for", TokenType::FOR},
		{"fun", TokenType::FUN},
		{"if", TokenType::IF},
		{"nil", TokenType::NIL},
		{"or", TokenType::OR},
		{"print", TokenType::PRINT},
		{"return", TokenType::RETURN},
		{"super", TokenType::SUPER},
		{"this", TokenType::THIS},
		{"true", TokenType::TRUE},
		{"var", TokenType::VAR},
		{"while", TokenType::WHILE}
	};

	std::vector<Token> m_tokens;

	bool isAtEnd() const {
		return m_current >= m_source.size();
	}

	char advance() {
		return m_source[m_current++];
	}

	bool match(char expected) {
		if (isAtEnd()) return false;
		if (m_source[m_current] != expected) return false;

		m_current++;
		return true;
	}

	char peek() const {
		if (isAtEnd()) return '\0';
		return m_source[m_current];
	}

	char peekNext() const {
		if (m_current + 1 >= m_source.size()) return '\0';
		return m_source[m_current + 1];
	}

	bool isDigit(char c) const {
		return c >= '0' && c <= '9';
	}

	bool isAlpha(char c) const {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}

	bool isAlphaNumeric(char c) const {
		return isAlpha(c) || isDigit(c);
	}
};