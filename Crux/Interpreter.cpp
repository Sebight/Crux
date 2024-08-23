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

void Interpreter::visitVariable(Ptr<VariableExpr> expr)
{
	Ptr<CruxObject> value = m_env.get(expr->name);
	if (value == nullptr) {
		throw CruxRuntimeError("Undefined variable '" + expr->name->lexeme + "'", expr->name->line, expr->name->lexeme);
	}

	m_results.push(value);
}

void Interpreter::visitAssign(Ptr<AssignExpr> expr)
{
	eval(expr->value);
	Ptr<CruxObject> value = m_results.top();
	m_results.pop();

	m_env.assign(expr->name, value);
}

void Interpreter::visitLogical(Ptr<LogicalExpr> expr)
{
	eval(expr->left);
	Ptr<CruxObject> left = m_results.top();

	if (expr->op->type == TokenType::OR) {
		if (isTruthy(left)) {
			m_results.push(left);
			return;
		}
	}
	else {
		if (!isTruthy(left)) {
			m_results.push(left);
		}
	}

	eval(expr->right);
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

void Interpreter::interpret(std::vector<Ptr<Stmt>> statements)
{
	try {
		for (auto& statement : statements) {
			execute(statement);
		}
	}
	catch (const std::exception& e) {
		// TODO: Proper handling for runtime errors
		printf("Error: %s\n", e.what());
	}
}

void Interpreter::execute(Ptr<Stmt> stmt)
{
	stmt->accept(*this);
}

void Interpreter::executeBlock(std::vector<Ptr<Stmt>> statements, Env& env)
{
	Env& prev = m_env;
	try {
		m_env = env;
		for (Ptr<Stmt> stmt : statements) {
			execute(stmt);
		}
	}
	catch (const std::exception& e) {
		throw e;
	}
	m_env = prev;
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

void Interpreter::visitExprStmt(Ptr<ExprStmt> stmt)
{
	eval(stmt->expr);
}

void Interpreter::visitPrintStmt(Ptr<PrintStmt> stmt)
{
	eval(stmt->expr);
	Ptr<CruxObject> result = m_results.top();
	m_results.pop();
	printf("%s\n", result->toString().c_str());
}

void Interpreter::visitVarStmt(Ptr<VarStmt> stmt)
{
	Ptr<CruxObject> value = std::make_shared<CruxObject>();

	if (stmt->initializer != nullptr) {
		eval(stmt->initializer);
		value = m_results.top();
		m_results.pop();
	}

	m_env.define(stmt->name->lexeme, value);
}

void Interpreter::visitBlockStmt(Ptr<BlockStmt> stmt)
{
	Env newEnv = Env(std::make_shared<Env>(m_env));
	executeBlock(stmt->statements, newEnv);
}

void Interpreter::visitIfStmt(Ptr<IfStmt> stmt)
{
	eval(stmt->condition);
	Ptr<CruxObject> cond = m_results.top();
	m_results.pop();

	if (isTruthy(cond)) {
		execute(stmt->thenBranch);
	}
	else if (stmt->elseBranch != nullptr) {
		execute(stmt->elseBranch);
	}
}

void Interpreter::visitWhileStmt(Ptr<WhileStmt> stmt)
{
	eval(stmt->cond);
	Ptr<CruxObject> cond = m_results.top();
	m_results.pop();

	while (isTruthy(cond)) {
		execute(stmt->body);

		eval(stmt->cond);
		cond = m_results.top();
		m_results.pop();
	}
}
