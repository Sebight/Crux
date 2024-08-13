#include "Parser.h"

Parser::Parser(std::vector<Ptr<Token>> tokens)
{
	m_tokens = tokens;
	m_currentTokenIndex = 0;
}

std::vector<Ptr<Stmt>> Parser::parse()
{
	std::vector<Ptr<Stmt>> statements;
	while (!isAtEnd()) {
		try {
			statements.push_back(statement());
		}
		catch (const ParseError e) {
			printf("Error parsing statement: %s\n", e.what());
			break;
		}
	}

	return statements;
}

Ptr<Expr> Parser::expression()
{
	return equality();
}

Ptr<Expr> Parser::equality() {
	Ptr<Expr> expr = comparison();

	while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL })) {
		Ptr<Token> op = previous();
		Ptr<Expr> right = comparison();
		expr = std::make_shared<BinaryExpr>(expr, op, right);
	}
	return expr;
}

Ptr<Expr> Parser::comparison() {
	Ptr<Expr> expr = term();

	while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL })) {
		Ptr<Token> op = previous();
		Ptr<Expr> right = term();
		expr = std::make_shared<BinaryExpr>(expr, op, right);
	}
	return expr;
}

Ptr<Expr> Parser::term()
{
	Ptr<Expr> expr = factor();

	while (match({ TokenType::MINUS, TokenType::PLUS })) {
		Ptr<Token> op = previous();
		Ptr<Expr> right = factor();
		expr = std::make_shared<BinaryExpr>(expr, op, right);
	}

	return expr;
}

Ptr<Expr> Parser::factor()
{
	Ptr<Expr> expr = unary();

	while (match({ TokenType::SLASH, TokenType::STAR })) {
		Ptr<Token> op = previous();
		Ptr<Expr> right = unary();
		expr = std::make_shared<BinaryExpr>(expr, op, right);
	}

	return expr;
}

Ptr<Expr> Parser::unary()
{
	if (match({ TokenType::BANG, TokenType::MINUS })) {
		Ptr<Token> op = previous();
		Ptr<Expr> right = unary();
		return std::make_shared<UnaryExpr>(op, right);
	}

	return primary();
}

Ptr<Expr> Parser::primary()
{
	if (match({ TokenType::FALSE })) return std::make_shared<LiteralExpr>(std::make_shared<Token>(TokenType::FALSE, "false", "", 0));
	if (match({ TokenType::TRUE })) return std::make_shared<LiteralExpr>(std::make_shared<Token>(TokenType::TRUE, "true", "", 0));
	if (match({ TokenType::NIL })) return std::make_shared<LiteralExpr>(std::make_shared<Token>(TokenType::NIL, "nil", "", 0));

	if (match({ TokenType::NUMBER, TokenType::STRING })) {
		return std::make_shared<LiteralExpr>(previous());
	}

	if (match({ TokenType::LEFT_PAREN })) {
		Ptr<Expr> expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
		return std::make_shared<GroupingExpr>(expr);
	}

	// Expected expression

	throw ParseError("Expected expression.", peek()->line);
}

Ptr<Stmt> Parser::statement()
{
	if (match({ TokenType::PRINT })) return printStatement();

	return expressionStatement();
}

Ptr<Stmt> Parser::printStatement()
{
	Ptr<Expr> value = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after value.");
	return std::make_shared<PrintStmt>(value);
}

Ptr<Stmt> Parser::expressionStatement()
{
	Ptr<Expr> expr = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after expression.");
	return std::make_shared<ExprStmt>(expr);
}

void Parser::synchronize() {
	advance();

	while (!isAtEnd()) {
		if (previous()->type == TokenType::SEMICOLON) return;

		switch (peek()->type) {
		case TokenType::CLASS:
		case TokenType::FUN:
		case TokenType::VAR:
		case TokenType::FOR:
		case TokenType::IF:
		case TokenType::WHILE:
		case TokenType::PRINT:
		case TokenType::RETURN:
			return;
		}

		advance();
	}
}
