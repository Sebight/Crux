#pragma once

#include "Visitor.h"
#include "AST.h"

class ASTPrinter : public Visitor {
public:
	void visitBinary(Ptr<BinaryExpr>) override;
	void visitLiteral(Ptr<LiteralExpr>) override;
	void visitUnary(Ptr<UnaryExpr>) override;
	void visitGrouping(Ptr<GroupingExpr>) override;
	void visitVariable(Ptr<VariableExpr>) override;
	void visitAssign(Ptr<AssignExpr>) override;
	void visitLogical(Ptr<LogicalExpr>) override;

	void print(Ptr<Expr> expr) {
		expr->accept(*this);
	}

	void parenthesize(const std::string& name, std::initializer_list<Ptr<Expr>> exprs);

};