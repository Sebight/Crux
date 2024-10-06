#pragma once

#include "Visitor.h"
#include "Types.h"
#include "AST.h"
#include <stack>
#include "Literal.h"
#include "Env.h"

// Runtime objects
#include "CruxNative/NativeClock.h"
#include "CruxObject.h"
#include "CruxCallable.h"
#include "CruxFunction.h"

class Interpreter : public Visitor {
public:
	Interpreter() {
		m_globals = std::make_shared<Env>();
		m_globals->define("clock", std::make_shared<NativeClock>());

		m_env = m_globals;
	}

	Interpreter(Ptr<Env> env, Ptr<Env> globals) {
		m_env = env;
		m_globals = globals;
	}

	~Interpreter() = default;

	void visitBinary(Ptr<BinaryExpr> expr) override;
	void visitLiteral(Ptr<LiteralExpr> expr) override;
	void visitUnary(Ptr<UnaryExpr> expr) override;
	void visitGrouping(Ptr<GroupingExpr> expr) override;

	void visitVariable(Ptr<VariableExpr> expr) override;
	void visitAssign(Ptr<AssignExpr> expr) override;
	void visitLogical(Ptr<LogicalExpr> expr) override;

	void visitCall(Ptr<CallExpr> expr) override;

	void visitExprStmt(Ptr<ExprStmt> stmt) override;
	void visitPrintStmt(Ptr<PrintStmt> stmt) override;
	void visitVarStmt(Ptr<VarStmt> stmt) override;
	void visitBlockStmt(Ptr<BlockStmt> stmt) override;
	void visitIfStmt(Ptr<IfStmt> stmt) override;
	void visitWhileStmt(Ptr<WhileStmt> stmt) override;
	void visitFunctionStmt(Ptr<FunctionStmt> stmt) override;
	void visitReturnStmt(Ptr<ReturnStmt> stmt) override;

	void interpret(std::vector<Ptr<Stmt>> statements);
	void execute(Ptr<Stmt> stmt);
	void executeBlock(std::vector<Ptr<Stmt>> statements, Ptr<Env> env);

	void pushResult(Ptr<CruxObject> obj) {
		m_results.push(std::move(obj));
	}

	Ptr<Env> getEnv() {
		return std::make_shared<Env>(m_env);
	}

	void resolve(Ptr<Expr>, int depth);
private:
	std::stack<Ptr<CruxObject>> m_results;
	Ptr<Env> m_env;
	Ptr<Env> m_globals;
	std::unordered_map<Ptr<Expr>, int> m_locals;

	Ptr<CruxObject> lookUpVariable(Ptr<Token> name, Ptr<Expr> expr);

	void eval(Ptr<Expr> expr);
	bool isTruthy(Ptr<CruxObject> obj);
	bool isEqual(Ptr<CruxObject> a, Ptr<CruxObject> b);

	void typeCheck(Ptr<Token>& token, Ptr<CruxObject>& obj, TokenType target) {
		if (obj->type != target) {
			// TODO: Improve
			throw CruxRuntimeError("Type mismatch", token->line, token->lexeme);
		}
	}

	bool bTypeCheck(Ptr<Token>&, Ptr<CruxObject>& obj, TokenType target) {
		return obj->type == target;
	}

	void assert(bool condition, std::string message) {
		if (!condition) {
			throw CruxRuntimeError(message);
		}
	}
};