#pragma once

#include "Token.h"
#include "Visitor.h"
#include <memory>
#include <vector>

class ASTNode {
public:
	ASTNode() = default;
	virtual ~ASTNode() = default;

	virtual void accept(Visitor& visitor) = 0;
};

class Expr : public ASTNode, public std::enable_shared_from_this<Expr> {
public:
	Expr() = default;
	~Expr() override = default;
};

class BinaryExpr : public Expr {
public:
	Ptr<Expr> left;
	Ptr<Token> op;
	Ptr<Expr> right;

	BinaryExpr(Ptr<Expr> left, Ptr<Token> op, Ptr<Expr> right);
	void accept(Visitor& visitor) override;
};

class LiteralExpr : public Expr {
public:
	Ptr<Token> value;
	explicit LiteralExpr(Ptr<Token> value);
	void accept(Visitor& visitor) override;
};

class UnaryExpr : public Expr {
public:
	Ptr<Token> op;
	Ptr<Expr> right;

	UnaryExpr(Ptr<Token> op, Ptr<Expr> right);
	void accept(Visitor& visitor) override;
};

class GroupingExpr : public Expr {
public:
	Ptr<Expr> expr;

	GroupingExpr(Ptr<Expr> expr);
	void accept(Visitor& visitor) override;
};

class VariableExpr : public Expr {
public:
	Ptr<Token> name;

	VariableExpr(Ptr<Token> name);
	void accept(Visitor& visitor) override;
};

class AssignExpr : public Expr {
public:
	Ptr<Token> name;
	Ptr<Expr> value;
	// 1 - =
	// 2 - +=
	// 3 - -=
	int mode;

	AssignExpr(Ptr<Token> name, Ptr<Expr> value, int mode);
	void accept(Visitor& visitor) override;
};

class LogicalExpr : public Expr {
public:
	Ptr<Expr> left;
	Ptr<Token> op;
	Ptr<Expr> right;

	LogicalExpr(Ptr<Expr> left, Ptr<Token> op, Ptr<Expr> right);
	void accept(Visitor& visitor) override;
};
class CallExpr : public Expr {
public:
	Ptr<Expr> callee;
	Ptr<Token> paren;
	std::vector<Ptr<Expr>> arguments;

	CallExpr(Ptr<Expr> callee, Ptr<Token> paren, std::vector<Ptr<Expr>> arguments);
	void accept(Visitor& visitor) override;
};

class GetExpr : public Expr {
public:
	Ptr<Expr> object;
	Ptr<Token> name;

	GetExpr(Ptr<Expr> object, Ptr<Token> name);
	void accept(Visitor& visitor) override;
};

class SetExpr : public Expr {
public:
	enum SetOperation {
		ASSIGN = 1,
		PLUS_ASSIGN = 2,
		MINUS_ASSIGN = 3
	};
	Ptr<Expr> object;
	Ptr<Token> name;
	Ptr<Expr> value;
	SetOperation operation;

	SetExpr(Ptr<Expr> object, Ptr<Token> name, Ptr<Expr> value, int operation);
	void accept(Visitor& visitor) override;
};

class ThisExpr : public Expr {
public:
	Ptr<Token> keyword;

	ThisExpr(Ptr<Token> keyword);
	void accept(Visitor& visitor) override;
};

class SuperExpr : public Expr {
public:
	Ptr<Token> keyword;
	Ptr<Token> method;

	SuperExpr(Ptr<Token> keyword, Ptr<Token> method);
	void accept(Visitor& visitor) override;
};

class Stmt : public ASTNode, public std::enable_shared_from_this<Stmt> {
public:
	Stmt() = default;
	~Stmt() override = default;
};

class ExprStmt : public Stmt {
public:
	Ptr<Expr> expr;

	ExprStmt(Ptr<Expr> expr);
	void accept(Visitor& visitor) override;
};

class PrintStmt : public Stmt {
public:
	Ptr<Expr> expr;

	PrintStmt(Ptr<Expr> expr);
	void accept(Visitor& visitor) override;
};

class VarStmt : public Stmt {
public:
	Ptr<Token> name;
	Ptr<Expr> initializer;

	VarStmt(Ptr<Token> name, Ptr<Expr> initializer);
	void accept(Visitor& visitor) override;
};

class BlockStmt : public Stmt {
public:
	std::vector<Ptr<Stmt>> statements;

	BlockStmt(std::vector<Ptr<Stmt>> statements);
	void accept(Visitor& visitor) override;
};

class IfStmt : public Stmt {
public:
	Ptr<Expr> condition;
	Ptr<Stmt> thenBranch;
	Ptr<Stmt> elseBranch;

	IfStmt(Ptr<Expr> condition, Ptr<Stmt> thenBranch, Ptr<Stmt> elseBranch);
	void accept(Visitor& visitor) override;
};

class WhileStmt : public Stmt {
public:
	Ptr<Expr> cond;
	Ptr<Stmt> body;

	WhileStmt(Ptr<Expr> cond, Ptr<Stmt> body);
	void accept(Visitor& visitor) override;
};

class FunctionStmt : public Stmt {
public:
	Ptr<Token> name;
	std::vector<Ptr<Token>> params;
	std::vector<Ptr<Stmt>> body;

	FunctionStmt(Ptr<Token> name, std::vector<Ptr<Token>> params, std::vector<Ptr<Stmt>> body);
	void accept(Visitor& visitor) override;
};

class ReturnStmt : public Stmt {
public:
	Ptr<Token> keyword;
	Ptr<Expr> value;

	ReturnStmt(Ptr<Token> keyword, Ptr<Expr> value);
	void accept(Visitor& visitor) override;
};

class ClassStmt : public Stmt {
public:
	Ptr<Token> name;
	std::vector<Ptr<FunctionStmt>> methods;
	Ptr<VariableExpr> superclass;

	ClassStmt(Ptr<Token> name, std::vector<Ptr<FunctionStmt>> methods, Ptr<VariableExpr> superclass);
	void accept(Visitor& visitor) override;
};