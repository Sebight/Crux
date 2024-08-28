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
			statements.push_back(declaration());
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
	return assignment();
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

	while (match({ TokenType::MINUS, TokenType::PLUS})) {
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
	return call();
}

Ptr<Expr> Parser::primary()
{
	if (match({ TokenType::FALSE })) return std::make_shared<LiteralExpr>(std::make_shared<Token>(TokenType::FALSE, "false", "", 0));
	if (match({ TokenType::TRUE })) return std::make_shared<LiteralExpr>(std::make_shared<Token>(TokenType::TRUE, "true", "", 0));
	if (match({ TokenType::NIL })) return std::make_shared<LiteralExpr>(std::make_shared<Token>(TokenType::NIL, "nil", "", 0));

	if (match({ TokenType::NUMBER, TokenType::STRING })) {
		return std::make_shared<LiteralExpr>(previous());
	}

	if (match({ TokenType::IDENTIFIER })) {
		return std::make_shared<VariableExpr>(previous());
	}

	if (match({ TokenType::LEFT_PAREN })) {
		Ptr<Expr> expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
		return std::make_shared<GroupingExpr>(expr);
	}

	// Expected expression

	throw ParseError("Expected expression.", peek()->line);
}

Ptr<Expr> Parser::assignment()
{
	Ptr<Expr> expr = logicalOr();
	int mode = -1;

	if (match({ TokenType::EQUAL })) mode = 1;
	if (match({ TokenType::PLUS_EQUAL })) mode = 2;
	if (match({ TokenType::MINUS_EQUAL })) mode = 3;

	if (mode != -1) {
		Ptr<Token> equals = previous();
		Ptr<Expr> value = assignment();

		try {
			VariableExpr& var = dynamic_cast<VariableExpr&>(*expr);
			return std::make_shared<AssignExpr>(var.name, value, mode);
		}
		catch (const std::bad_cast& e) {
			throw ParseError("Invalid assignment target.", equals->line);
		}
	}

	return expr;
}

Ptr<Expr> Parser::logicalOr()
{
	Ptr<Expr> expr = logicalAnd();

	while (match({ TokenType::OR })) {
		Ptr<Token> op = previous();
		Ptr<Expr> right = logicalAnd();
		expr = std::make_shared<LogicalExpr>(expr, op, right);
	}

	return expr;
}

Ptr<Expr> Parser::logicalAnd()
{
	Ptr<Expr> expr = equality();

	while (match({ TokenType::AND })) {
		Ptr<Token> op = previous();
		Ptr<Expr> right = equality();
		expr = std::make_shared<LogicalExpr>(expr, op, right);
	}

	return expr;
}

Ptr<Expr> Parser::call()
{
	Ptr<Expr> expr = primary();

	while (true) {
		if (match({ TokenType::LEFT_PAREN })) {
			expr = finishCall(expr);
		}
		else {
			break;
		}
	}

	return expr;
}

Ptr<Expr> Parser::finishCall(Ptr<Expr> callee)
{
	std::vector<Ptr<Expr>> arguments;

	if (!check({ TokenType::RIGHT_PAREN })) {
		do {
			if (arguments.size() >= 127) {
				throw ParseError("Cannot have more than 127 arguments.", peek()->line);
			}

			arguments.push_back(expression());
		} while (match({ TokenType::COMMA }));
	}

	Ptr<Token> paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");
	return std::make_shared<CallExpr>(callee, paren, arguments);
}

Ptr<Stmt> Parser::statement()
{
	if (match({ TokenType::FOR })) return forStatement();
	if (match({ TokenType::IF })) return ifStatement();
	if (match({ TokenType::PRINT })) return printStatement();
	if (match({ TokenType::RETURN })) return returnStatement();
	if (match({ TokenType::WHILE })) return whileStatement();
	if (match({ TokenType::LEFT_BRACE })) return std::make_shared<BlockStmt>(block());

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

Ptr<Stmt> Parser::declaration()
{
	try {
		if (match({ TokenType::FUNC })) return function("function");
		if (match({ TokenType::VAR })) return varDeclaration();
		return statement();
	}
	catch (const ParseError e) {
		printf("Error parsing declaration: %s\n", e.what());
		synchronize();
		return nullptr;
	}
}

Ptr<Stmt> Parser::varDeclaration()
{
	Ptr<Token> name = consume(TokenType::IDENTIFIER, "Expected variable name.");

	Ptr<Expr> initializer = nullptr;

	if (match({ TokenType::EQUAL })) {
		initializer = expression();
	}
	consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");

	return std::make_shared<VarStmt>(name, initializer);
}

std::vector<Ptr<Stmt>> Parser::block()
{
	std::vector<Ptr<Stmt>> statements;

	while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
		statements.push_back(declaration());
	}

	consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
	return statements;
}

Ptr<Stmt> Parser::ifStatement()
{
	consume(LEFT_PAREN, "Expected '(' after 'if'.");
	Ptr<Expr> condition = expression();
	consume(RIGHT_PAREN, "Expected ')' after if condition.");

	Ptr<Stmt> thenBranch = statement();
	Ptr<Stmt> elseBranch = nullptr;
	if (match({ TokenType::ELSE })) {
		elseBranch = statement();
	}

	return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
}

Ptr<Stmt> Parser::whileStatement()
{
	consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
	Ptr<Expr> condition = expression();
	consume(TokenType::RIGHT_PAREN, "Expected ')' after while condition.");

	Ptr<Stmt> body = statement();

	return std::make_shared<WhileStmt>(condition, body);
}

Ptr<Stmt> Parser::forStatement()
{
	consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");

	Ptr<Stmt> initializer = nullptr;
	if (match({ TokenType::SEMICOLON })) {
		initializer = nullptr;
	}
	else if (match({ TokenType::VAR })) {
		initializer = varDeclaration();
	}
	else {
		initializer = expressionStatement();
	}

	Ptr<Expr> condition = nullptr;
	if (!check(TokenType::SEMICOLON)) {
		condition = expression();
	}
	consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");

	Ptr<Expr> increment = nullptr;
	if (!check(TokenType::RIGHT_PAREN)) {
		increment = expression();
	}
	consume(TokenType::RIGHT_PAREN, "Expected ')' after for clause.");

	Ptr<Stmt> body = statement();

	if (increment != nullptr) {
		body = std::make_shared<BlockStmt>(std::vector<Ptr<Stmt>>{ body, std::make_shared<ExprStmt>(increment) });
	}

	if (condition == nullptr) condition = std::make_shared<LiteralExpr>(std::make_shared<Token>(TokenType::TRUE, "true", "", 0));
	body = std::make_shared<WhileStmt>(condition, body);

	if (initializer != nullptr) {
		body = std::make_shared<BlockStmt>(std::vector<Ptr<Stmt>>{ initializer, body });
	}

	return body;
}

Ptr<FunctionStmt> Parser::function(std::string kind)
{
	Ptr<Token> name = consume(TokenType::IDENTIFIER, "Expected " + kind + " name.");
	consume(TokenType::LEFT_PAREN, "Expected '(' after " + kind + " name.");

	std::vector<Ptr<Token>> parameters;
	if (!check(TokenType::RIGHT_PAREN)) {
		do {
			if (parameters.size() >= 127) {
				throw ParseError("Cannot have more than 127 parameters.", peek()->line);
			}

			parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name."));
		} while (match({ TokenType::COMMA }));
	}

	consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

	consume(TokenType::LEFT_BRACE, "Expected '{' before " + kind + " body.");
	std::vector<Ptr<Stmt>> body = block();
	return std::make_shared<FunctionStmt>(name, parameters, body);
}

Ptr<Stmt> Parser::returnStatement()
{
	Ptr<Token> keyword = previous();
	Ptr<Expr> value = nullptr;

	if (!check(TokenType::SEMICOLON)) {
		value = expression();
	}

	consume(TokenType::SEMICOLON, "Expected ';' after return value.");
	return std::make_shared<ReturnStmt>(keyword, value);
}

void Parser::synchronize() {
	advance();

	while (!isAtEnd()) {
		if (previous()->type == TokenType::SEMICOLON) return;

		switch (peek()->type) {
		case TokenType::CLASS:
		case TokenType::FUNC:
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
