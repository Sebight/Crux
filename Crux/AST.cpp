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

AssignExpr::AssignExpr(Ptr<Token> name, Ptr<Expr> value, int mode)
	: name(std::move(name)), value(std::move(value)), mode(mode)
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

VarStmt::VarStmt(Ptr<Token> name, Ptr<Expr> initializer)
	: name(std::move(name)), initializer(std::move(initializer))
{
}


void VarStmt::accept(Visitor& visitor)
{
	visitor.visitVarStmt(static_pointer_cast<VarStmt>(shared_from_this()));
}

BlockStmt::BlockStmt(std::vector<Ptr<Stmt>> statements)
	: statements(std::move(statements))
{
}

void BlockStmt::accept(Visitor& visitor)
{
	visitor.visitBlockStmt(static_pointer_cast<BlockStmt>(shared_from_this()));
}

IfStmt::IfStmt(Ptr<Expr> condition, Ptr<Stmt> thenBranch, Ptr<Stmt> elseBranch)
	: condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch))
{
}

void IfStmt::accept(Visitor& visitor)
{
	visitor.visitIfStmt(static_pointer_cast<IfStmt>(shared_from_this()));
}

WhileStmt::WhileStmt(Ptr<Expr> cond, Ptr<Stmt> body) : cond(std::move(cond)), body(std::move(body))
{
}

void WhileStmt::accept(Visitor& visitor)
{
	visitor.visitWhileStmt(static_pointer_cast<WhileStmt>(shared_from_this()));
}

CallExpr::CallExpr(Ptr<Expr> callee, Ptr<Token> paren, std::vector<Ptr<Expr>> arguments) : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments))
{
}

void CallExpr::accept(Visitor& visitor)
{
	visitor.visitCall(static_pointer_cast<CallExpr>(shared_from_this()));
}

FunctionStmt::FunctionStmt(Ptr<Token> name, std::vector<Ptr<Token>> params, std::vector<Ptr<Stmt>> body) : name(std::move(name)), params(std::move(params)), body(std::move(body))
{
}

void FunctionStmt::accept(Visitor& visitor)
{
	visitor.visitFunctionStmt(static_pointer_cast<FunctionStmt>(shared_from_this()));
}

ReturnStmt::ReturnStmt(Ptr<Token> keyword, Ptr<Expr> value) : keyword(std::move(keyword)), value(std::move(value))
{
}

void ReturnStmt::accept(Visitor& visitor)
{
	visitor.visitReturnStmt(static_pointer_cast<ReturnStmt>(shared_from_this()));
}