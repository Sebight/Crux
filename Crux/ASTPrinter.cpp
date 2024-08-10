#include "ASTPrinter.h"

void ASTPrinter::parenthesize(const std::string& name, std::initializer_list<Ptr<Expr>> exprs) {
	printf("(%s", name.c_str());
	for (auto& expr : exprs) {
		printf(" ");
		expr->accept(*this);
	}
	printf(")");
}

void ASTPrinter::visitBinary(Ptr<BinaryExpr> expr) {
	parenthesize(expr->op->lexeme, { expr->left, expr->right });
}

void ASTPrinter::visitLiteral(Ptr<LiteralExpr> expr) {
	printf("%s", expr->value->lexeme.c_str());
}

void ASTPrinter::visitUnary(Ptr<UnaryExpr> expr) {
	parenthesize(expr->op->lexeme, { expr->right });
}

void ASTPrinter::visitGrouping(Ptr<GroupingExpr> expr) {
	parenthesize("group", { expr->expr });
}

void ASTPrinter::visitVariable(Ptr<VariableExpr> expr) {

}

void ASTPrinter::visitAssign(Ptr<AssignExpr> expr) {

}

void ASTPrinter::visitLogical(Ptr<LogicalExpr> expr) {

}