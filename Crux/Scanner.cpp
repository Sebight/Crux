#include "Scanner.h"

std::vector<Token> Scanner::scanTokens()
{
	while (!isAtEnd())
	{
		m_start = m_current;
		scanToken();
	}

	m_tokens.push_back(Token(TokenType::END_OF_FILE, "", "", m_line));
	return m_tokens;
}

void Scanner::scanToken()
{
	char c = advance();

	switch (c) {
	case '(': addToken(TokenType::LEFT_PAREN); break;
	case ')': addToken(TokenType::RIGHT_PAREN); break;
	case '{': addToken(TokenType::LEFT_BRACE); break;
	case '}': addToken(TokenType::RIGHT_BRACE); break;
	case ',': addToken(TokenType::COMMA); break;
	case '.': addToken(TokenType::DOT); break;
	case '-': addToken(TokenType::MINUS); break;
	case '+': addToken(TokenType::PLUS); break;
	case ';': addToken(TokenType::SEMICOLON); break;
	case '*': addToken(TokenType::STAR); break;
	case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
	case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
	case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
	case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

	case '/':
		if (match('/'))
		{
			while (peek() != '\n' && !isAtEnd()) advance();
		}
		else
		{
			addToken(TokenType::SLASH);
		}
		break;

	case ' ':
	case '\r':
	case '\t':
		break;

	case '\n':
		m_line++;
		break;

	case '"':
		string(); break;

	default:
		if (isDigit(c)) {
			number();
		}
		else if (isAlpha(c)) {
			identifier();
		}
		else {
			// TODO: Pass in error handler
			printf("Unexpected character: %c\n", c);
		}
		break;
	}
}

OptLiteral makeOptLiteral(TokenType t, const std::string& text)
{
	switch (t) {
	case TokenType::NUMBER:
		return makeOptLiteral(std::stod(text));
	case TokenType::STRING:
		return makeOptLiteral(text);
	default:
		return std::nullopt;
	}
}

void Scanner::addToken(TokenType type)
{
	std::string text = m_source.substr(m_start, m_current - m_start);

	if (type == TokenType::STRING) {
		text = text.substr(1, text.size() - 2);
	}

	m_tokens.emplace_back(type, text, makeOptLiteral(type, text), m_line);
}

void Scanner::string()
{
	while (peek() != '"' && !isAtEnd()) {
		if (peek() == '\n') m_line++;

		advance();
	}

	if (isAtEnd())
	{
		// Unterminated string
		// TODO: Pass in error handler
	}

	advance();

	addToken(TokenType::STRING);
}

void Scanner::number()
{
	while (isDigit(peek())) advance();

	if (peek() == '.' && isDigit(peekNext()))
	{
		advance();

		while (isDigit(peek())) advance();
	}

	addToken(TokenType::NUMBER);
}

void Scanner::identifier()
{
	while (isAlphaNumeric(peek())) advance();

	std::string text = m_source.substr(m_start, m_current - m_start);

	auto keyword = m_keywords.find(text);
	TokenType type = keyword != m_keywords.end() ? keyword->second : TokenType::IDENTIFIER;

	addToken(type);
}