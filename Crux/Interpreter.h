#pragma once

#include "Visitor.h"
#include "Types.h"
#include "AST.h"
#include <stack>
#include "CruxObject.h"
#include "Literal.h"

class CruxRuntimeError : public std::exception {
public:
	CruxRuntimeError(std::string message, int line = -1, std::string token = "") : m_message(message) {
		if (line != -1 && !token.empty()) {
			m_message = "Line " + std::to_string(line) + ": " + message + " at token: " + token;
		}
		else {
			m_message = message;
		}
	}
	const char* what() const noexcept override {
		return m_message.c_str();
	}

private:
	std::string m_message;
};

class Interpreter : public Visitor {
public:
	void visitBinary(Ptr<BinaryExpr> expr) override;
	void visitLiteral(Ptr<LiteralExpr> expr) override;
	void visitUnary(Ptr<UnaryExpr> expr) override;
	void visitGrouping(Ptr<GroupingExpr> expr) override;

	void visitVariable(Ptr<VariableExpr> expr) override {};
	void visitAssign(Ptr<AssignExpr> expr) override {};
	void visitLogical(Ptr<LogicalExpr> expr) override {};

	void visitExprStmt(Ptr<ExprStmt> stmt) override;
	void visitPrintStmt(Ptr<PrintStmt> stmt) override;

	void interpret(std::vector<Ptr<Stmt>> statements);
	void execute(Ptr<Stmt> stmt);
private:
	std::stack<Ptr<CruxObject>> m_results;

	void eval(Ptr<Expr> expr);
	bool isTruthy(Ptr<CruxObject> obj);
	bool isEqual(Ptr<CruxObject> a, Ptr<CruxObject> b);

	void typeCheck(Ptr<Token>& token, Ptr<CruxObject>& obj, TokenType target) {
		if (obj->type != target) {
			// TODO: Improve
			throw CruxRuntimeError("Type mismatch", token->line, token->lexeme);
		}
	}

	void assert(bool condition, std::string message) {
		if (!condition) {
			throw CruxRuntimeError(message);
		}
	}
};