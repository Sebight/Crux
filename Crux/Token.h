#pragma once
#include <string>
#include <variant>
#include <optional>
#include "Literal.h"

enum TokenType {
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
	COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

	BANG, BANG_EQUAL,
	EQUAL, EQUAL_EQUAL,
	GREATER, GREATER_EQUAL,
	LESS, LESS_EQUAL, PLUS_EQUAL, MINUS_EQUAL,

	IDENTIFIER, STRING, NUMBER,

	AND, CLASS, ELSE, FALSE, FUNC, FOR, IF, NIL, OR,
	PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

	END_OF_FILE
};

class Token {
public:
	TokenType type;
	std::string lexeme;
	int line;
	OptLiteral literal;

	Token(TokenType type, const std::string& lexeme, OptLiteral literal, int line) : type(type), lexeme(lexeme), line(line), literal(literal) {}

	std::string toString() const {
		printf("Token Debug: lexeme=%s, line=%d, type=%d \n", lexeme.c_str(), line, type);
		return std::to_string(type) + " " + lexeme+"\n";
	}
};