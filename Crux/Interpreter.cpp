#include "Interpreter.h"

void Interpreter::visitLiteral(Ptr<LiteralExpr> expr)
{
	switch (expr->value->type)
	{
	case TokenType::NUMBER:
		m_results.push(std::make_shared<CruxObject>(std::stod(getLiteralString(expr->value->literal.value()))));
		break;

	case TokenType::STRING:
		m_results.push(std::make_shared<CruxObject>(getLiteralString(expr->value->literal.value())));
		break;

	case TokenType::NIL:
		m_results.push(std::make_shared<CruxObject>());
		break;

	case TokenType::TRUE:
		m_results.push(std::make_shared<CruxObject>(true));
		break;

	case TokenType::FALSE:
		m_results.push(std::make_shared<CruxObject>(false));
		break;

	default:
		printf("Unexpected literal type\n");
		break;
	}
}

void Interpreter::visitGrouping(Ptr<GroupingExpr> group)
{
	eval(group->expr);
}

void Interpreter::visitUnary(Ptr<UnaryExpr> unary) {
	eval(unary->right);

	switch (unary->op->type) {
	case TokenType::MINUS: {
		Ptr<CruxObject> n = m_results.top();
		m_results.pop();

		typeCheck(unary->op, n, TokenType::NUMBER);

		n->num = -n->num;
		m_results.push(n);

		break;
	}
	case TokenType::BANG: {
		Ptr<CruxObject> b = m_results.top();
		m_results.pop();

		m_results.push(std::make_shared<CruxObject>(!isTruthy(b)));

		break;
	}

	default:
		printf("Unexpected unary operator\n");
		break;
	}
}

void Interpreter::visitBinary(Ptr<BinaryExpr> bin) {
	eval(bin->left);
	eval(bin->right);

	Ptr<CruxObject> r = m_results.top();
	m_results.pop();

	Ptr<CruxObject> l = m_results.top();
	m_results.pop();

	switch (bin->op->type) {
	case TokenType::MINUS: {
		typeCheck(bin->op, l, TokenType::NUMBER);
		typeCheck(bin->op, r, TokenType::NUMBER);
		m_results.push(std::make_shared<CruxObject>(l->num - r->num));

		break;
	}
	case TokenType::SLASH: {
		typeCheck(bin->op, l, TokenType::NUMBER);
		typeCheck(bin->op, r, TokenType::NUMBER);


		assert(r->num != 0, "Division by zero");

		m_results.push(std::make_shared<CruxObject>(l->num / r->num));

		break;
	}

	case TokenType::STAR: {
		typeCheck(bin->op, l, TokenType::NUMBER);
		typeCheck(bin->op, r, TokenType::NUMBER);
		m_results.push(std::make_shared<CruxObject>(l->num * r->num));

		break;
	}

	case TokenType::PLUS: {
		if (l->type == TokenType::NUMBER && r->type == TokenType::NUMBER) {
			m_results.push(std::make_shared<CruxObject>(l->num + r->num));
		}
		else if (l->type == TokenType::STRING && r->type == TokenType::STRING) {
			std::string res = l->str + r->str;
			m_results.push(std::make_shared<CruxObject>(res));
		}
		else {
			throw CruxRuntimeError("Operands must be two numbers or two strings", bin->op->line, bin->op->lexeme);
		}

		break;
	}

	case TokenType::GREATER: {
		typeCheck(bin->op, l, TokenType::NUMBER);
		typeCheck(bin->op, r, TokenType::NUMBER);

		m_results.push(std::make_shared<CruxObject>(l->num > r->num));

		break;
	}

	case TokenType::GREATER_EQUAL: {
		typeCheck(bin->op, l, TokenType::NUMBER);
		typeCheck(bin->op, r, TokenType::NUMBER);
		m_results.push(std::make_shared<CruxObject>(l->num >= r->num));

		break;
	}

	case TokenType::LESS: {
		typeCheck(bin->op, l, TokenType::NUMBER);
		typeCheck(bin->op, r, TokenType::NUMBER);
		m_results.push(std::make_shared<CruxObject>(l->num < r->num));

		break;
	}

	case TokenType::LESS_EQUAL: {
		typeCheck(bin->op, l, TokenType::NUMBER);
		typeCheck(bin->op, r, TokenType::NUMBER);
		m_results.push(std::make_shared<CruxObject>(l->num <= r->num));

		break;
	}

	case TokenType::BANG_EQUAL: {
		m_results.push(std::make_shared<CruxObject>(!isEqual(l, r)));

		break;
	}

	case TokenType::EQUAL_EQUAL: {
		m_results.push(std::make_shared<CruxObject>(isEqual(l, r)));

		break;
	}
	}
}

void Interpreter::interpret(Ptr<Expr> expr)
{
	try {
		eval(expr);
		// We get the result at the top of the stack
		Ptr<CruxObject> result = m_results.top();
		printf("%s\n", result->toString().c_str());
	}
	catch (const std::exception& e) {
		printf("Error: %s\n", e.what());
	}
}

void Interpreter::eval(Ptr<Expr> expr)
{
	expr->accept(*this);
}

bool Interpreter::isTruthy(Ptr<CruxObject> obj)
{
	if (obj->type == TokenType::NIL)
		return false;

	if (obj->type == TokenType::FALSE)
		return false;

	return true;
}

bool Interpreter::isEqual(Ptr<CruxObject> a, Ptr<CruxObject> b)
{
	if (a->type == TokenType::NIL && b->type == TokenType::NIL)
		return true;

	if (a->type == TokenType::NIL)
		return false;

	if (a->type != b->type)
		return false;

	if (a->type == TokenType::STRING && b->type == TokenType::STRING)
		return a->str == b->str;

	if (a->type == TokenType::NUMBER && b->type == TokenType::NUMBER)
		return a->num == b->num;


	return a->boolean == b->boolean;
}
