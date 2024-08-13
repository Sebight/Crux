#pragma once
#include "Token.h"
#include <vector>
#include "AST.h"

class ParseError : public std::exception {
public:
	ParseError(std::string message, int line = -1) : m_line(line), m_message(message) {
		message = "";

		if (m_line != -1) {
			message = "Parse Error at line " + std::to_string(m_line) + ": " + m_message;
		}
		else {
			message = "Parse Error: " + m_message;
		}

		m_message = message;
	}
	const char* what() const noexcept override {
		return m_message.c_str();
	}

private:
	int m_line;
	std::string m_message;
};

class Parser {
public:
	Parser(std::vector<Ptr<Token>> tokens);
	std::vector<Ptr<Stmt>> parse();

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

	Ptr<Stmt> statement();
	Ptr<Stmt> printStatement();
	Ptr<Stmt> expressionStatement();


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
		throw ParseError(message);
	}

	void synchronize();
};