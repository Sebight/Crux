#pragma once
#include <memory>
#include "Types.h"

// Forward declare all classes
class BinaryExpr;
class LiteralExpr;
class UnaryExpr;
class GroupingExpr;
class VariableExpr;
class AssignExpr;
class LogicalExpr;

class ExprStmt;
class PrintStmt;


class Visitor {
public:
	virtual ~Visitor() = default;

	virtual void visitBinary(Ptr<BinaryExpr>) = 0;
	virtual void visitLiteral(Ptr<LiteralExpr>) = 0;
	virtual void visitUnary(Ptr<UnaryExpr>) = 0;
	virtual void visitGrouping(Ptr<GroupingExpr>) = 0;
	virtual void visitVariable(Ptr<VariableExpr>) = 0;
	virtual void visitAssign(Ptr<AssignExpr>) = 0;
	virtual void visitLogical(Ptr<LogicalExpr>) = 0;

	virtual void visitExprStmt(Ptr<ExprStmt>) = 0;
	virtual void visitPrintStmt(Ptr<PrintStmt>) = 0;
};