#include "AST.h"

void BinaryExpr::accept(Visitor& visitor)
{
	visitor.visitBinary(static_pointer_cast<BinaryExpr>(shared_from_this()));
}


BinaryExpr::BinaryExpr(Ptr<Expr> left, Ptr<Token> op, Ptr<Expr> right)
	: left(std::move(left)), op(std::move(op)), right(std::move(right))
{
}

void LiteralExpr::accept(Visitor& visitor)
{
	visitor.visitLiteral(static_pointer_cast<LiteralExpr>(shared_from_this()));
}

LiteralExpr::LiteralExpr(Ptr<Token> value)
	: value(std::move(value))
{
}

void UnaryExpr::accept(Visitor& visitor)
{
	visitor.visitUnary(static_pointer_cast<UnaryExpr>(shared_from_this()));
}

UnaryExpr::UnaryExpr(Ptr<Token> op, Ptr<Expr> right)
	: op(std::move(op)), right(std::move(right))
{
}

void GroupingExpr::accept(Visitor& visitor)
{
	visitor.visitGrouping(static_pointer_cast<GroupingExpr>(shared_from_this()));
}

GroupingExpr::GroupingExpr(Ptr<Expr> expr)
	: expr(std::move(expr))
{
}

void VariableExpr::accept(Visitor& visitor)
{
	visitor.visitVariable(static_pointer_cast<VariableExpr>(shared_from_this()));
}

VariableExpr::VariableExpr(Ptr<Token> name)
	: name(std::move(name))
{
}

void AssignExpr::accept(Visitor& visitor)
{
	visitor.visitAssign(static_pointer_cast<AssignExpr>(shared_from_this()));
}	

AssignExpr::AssignExpr(Ptr<Token> name, Ptr<Expr> value)
	: name(std::move(name)), value(std::move(value))
{
}

void LogicalExpr::accept(Visitor& visitor)
{
	visitor.visitLogical(static_pointer_cast<LogicalExpr>(shared_from_this()));
}

LogicalExpr::LogicalExpr(Ptr<Expr> left, Ptr<Token> op, Ptr<Expr> right)
	: left(std::move(left)), op(std::move(op)), right(std::move(right))
{
}

ExprStmt::ExprStmt(Ptr<Expr> expr)
	: expr(std::move(expr))
{
}

void ExprStmt::accept(Visitor& visitor)
{
	visitor.visitExprStmt(static_pointer_cast<ExprStmt>(shared_from_this()));
}

PrintStmt::PrintStmt(Ptr<Expr> expr)
	: expr(std::move(expr))
{
}

void PrintStmt::accept(Visitor& visitor)
{
	visitor.visitPrintStmt(static_pointer_cast<PrintStmt>(shared_from_this()));
}