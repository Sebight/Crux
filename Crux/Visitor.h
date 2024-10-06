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
class CallExpr;
class GetExpr;
class SetExpr;
class ThisExpr;

class ExprStmt;
class PrintStmt;
class VarStmt;
class BlockStmt;
class IfStmt;
class WhileStmt;
class FunctionStmt;
class ReturnStmt;
class ClassStmt;


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
	virtual void visitCall(Ptr<CallExpr>) = 0;
	virtual void visitGet(Ptr<GetExpr>) = 0;
	virtual void visitSet(Ptr<SetExpr>) = 0;
	virtual void visitThis(Ptr<ThisExpr>) = 0;

	virtual void visitExprStmt(Ptr<ExprStmt>) = 0;
	virtual void visitPrintStmt(Ptr<PrintStmt>) = 0;
	virtual void visitVarStmt(Ptr<VarStmt>) = 0;
	virtual void visitBlockStmt(Ptr<BlockStmt>) = 0;
	virtual void visitIfStmt(Ptr<IfStmt>) = 0;
	virtual void visitWhileStmt(Ptr<WhileStmt>) = 0;
	virtual void visitFunctionStmt(Ptr<FunctionStmt>) = 0;
	virtual void visitReturnStmt(Ptr<ReturnStmt>) = 0;
	virtual void visitClassStmt(Ptr<ClassStmt>) = 0;
};