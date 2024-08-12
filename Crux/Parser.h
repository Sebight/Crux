#pragma once
#include "Token.h"
#include <vector>
#include "AST.h"

class ParseError : public std::exception {};

class Parser {
public:
	Parser(std::vector<Ptr<Token>> tokens);
	Ptr<Expr> parse();

private:
	std::vector<Ptr<Token>> m_tokens;
	int m_currentTokenIndex;


	// Rules
	Ptr<Expr> expression();
	Ptr<Expr> equality();
	Ptr<Expr> comparison();
	Ptr<Expr> term();
	Ptr<Expr> factor();
	Ptr<Expr> unary();
	Ptr<Expr> primary();


	// Helper functions
	bool match(std::vector<TokenType> types) {
		for (auto type : types) {
			if (check(type)) {
				advance();
				return true;
			}
		}
		return false;

	}

	bool check(TokenType type) {
		if (isAtEnd()) return false;
		return peek()->type == type;
	}

	Ptr<Token> advance() {
		if (!isAtEnd()) m_currentTokenIndex++;
		return previous();
	}
	Ptr<Token> previous() {
		return m_tokens[m_currentTokenIndex - 1];
	}
	Ptr<Token> peek() {
		return m_tokens[m_currentTokenIndex];
	}
	bool isAtEnd() {
		return peek()->type == TokenType::END_OF_FILE;
	}

	Ptr<Token> consume(TokenType type, std::string message) {
		if (check(type)) return advance();
		
		// TODO: Error handling
		printf("Error: %s\n", message.c_str());
		throw ParseError();
	}

	void synchronize();
};