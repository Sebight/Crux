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

	AssignExpr(Ptr<Token> name, Ptr<Expr> value);
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