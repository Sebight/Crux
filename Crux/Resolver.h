#pragma once
#include "Visitor.h"
#include "Interpreter.h"

enum FunctionType {
	None,
	Function
};

class Resolver : public Visitor {
public:
	Resolver(Interpreter* interpreter) : m_interpreter(interpreter), m_currentFunction(FunctionType::None) {}

	void visitBinary(Ptr<BinaryExpr> expr) override;
	void visitLiteral(Ptr<LiteralExpr> expr) override {};
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

	void resolve(std::vector<Ptr<Stmt>> statements);

private:
	Interpreter* m_interpreter;

	std::vector<std::unordered_map<std::string, bool>> m_scopes;
	FunctionType m_currentFunction;

	void resolve(Ptr<Stmt> stmt);
	void resolve(Ptr<Expr> expr);
	void resolveLocal(Ptr<Expr> expr, Ptr<Token> name);
	void resolveFunction(Ptr<FunctionStmt> stmt, FunctionType type);

	void declare(Ptr<Token> name);
	void define(Ptr<Token> name);

	void beginScope();
	void endScope();
};