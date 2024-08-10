#pragma once

#include <memory>

template <typename T>
using Ptr = std::shared_ptr<T>;

// Forward declare all classes
class BinaryExpr;
class LiteralExpr;
class UnaryExpr;
class GroupingExpr;
class VariableExpr;
class AssignExpr;
class LogicalExpr;


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
};