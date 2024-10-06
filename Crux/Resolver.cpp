#include "Resolver.h"

void Resolver::visitExprStmt(Ptr<ExprStmt> stmt)
{
	resolve(stmt->expr);
}

void Resolver::visitPrintStmt(Ptr<PrintStmt> stmt)
{
	resolve(stmt->expr);
}

void Resolver::visitVarStmt(Ptr<VarStmt> stmt)
{
	declare(stmt->name);
	if (stmt->initializer != nullptr)
	{
		resolve(stmt->initializer);
	}

	define(stmt->name);
}

void Resolver::visitBlockStmt(Ptr<BlockStmt> stmt)
{
	beginScope();
	resolve(stmt->statements);
	endScope();
}

void Resolver::visitIfStmt(Ptr<IfStmt> stmt)
{
	resolve(stmt->condition);
	resolve(stmt->thenBranch);

	if (stmt->elseBranch != nullptr)
	{
		resolve(stmt->elseBranch);
	}
}

void Resolver::visitWhileStmt(Ptr<WhileStmt> stmt)
{
	resolve(stmt->cond);
	resolve(stmt->body);
}

void Resolver::visitFunctionStmt(Ptr<FunctionStmt> stmt)
{
	declare(stmt->name);
	define(stmt->name);

	resolveFunction(stmt, FunctionType::Function);
}

void Resolver::visitReturnStmt(Ptr<ReturnStmt> stmt)
{
	if (m_currentFunction == FunctionType::None)
	{
		throw CruxResolverError("Cannot return from top-level code.");
	}

	// TODO: Could support early returns in constructors, but feels unnecessary
	if (m_currentFunction == FunctionType::Constructor) {
		throw CruxResolverError("Cannot return a value from a constructor.");
	}

	if (stmt->value != nullptr)
	{
		resolve(stmt->value);
	}
}

void Resolver::visitClassStmt(Ptr<ClassStmt> stmt)
{
	ClassType enclosingClass = m_currentClass;
	m_currentClass = ClassType::Class;
	declare(stmt->name);

	beginScope();
	m_scopes.back()["this"] = true;

	for (Ptr<FunctionStmt>& method : stmt->methods) {
		FunctionType decl = FunctionType::Method;

		if (method->name->lexeme == stmt->name->lexeme) {
			decl = FunctionType::Constructor;
		}

		resolveFunction(method, decl);
	}
	endScope();

	m_currentClass = enclosingClass;

	define(stmt->name);
}

void Resolver::resolve(std::vector<Ptr<Stmt>> statements)
{
	try {
		for (auto& stmt : statements)
		{
			resolve(stmt);
		}
	}
	catch (CruxResolverError& e) {
		printf("Resolve Error: %s\n", e.what());
	}
}

void Resolver::resolve(Ptr<Stmt> stmt)
{
	stmt->accept(*this);
}

void Resolver::resolve(Ptr<Expr> expr)
{
	expr->accept(*this);
}

void Resolver::resolveLocal(Ptr<Expr> expr, Ptr<Token> name)
{
	for (int i = m_scopes.size() - 1; i >= 0; i--) {
		if (m_scopes[i].find(name->lexeme) != m_scopes[i].end()) {
			m_interpreter->resolve(expr, m_scopes.size() - 1 - i);
			return;
		}
	}
}

void Resolver::resolveFunction(Ptr<FunctionStmt> stmt, FunctionType type)
{
	FunctionType enclosingFunction = m_currentFunction;
	m_currentFunction = type;
	beginScope();
	for (auto& param : stmt->params) {
		declare(param);
		define(param);
	}

	resolve(stmt->body);
	endScope();
	m_currentFunction = enclosingFunction;
}

void Resolver::declare(Ptr<Token> name)
{
	if (m_scopes.empty()) return;

	auto& scope = m_scopes.back();
	if (scope.find(name->lexeme) != scope.end())
	{
		throw CruxResolverError(name->lexeme + ": Variable with this name already declared in this scope.");
	}

	scope[name->lexeme] = false;
}

void Resolver::define(Ptr<Token> name)
{
	if (m_scopes.empty()) return;
	m_scopes.back()[name->lexeme] = true;
}

void Resolver::beginScope()
{
	m_scopes.push_back(std::unordered_map<std::string, bool>());
}

void Resolver::endScope()
{
	m_scopes.pop_back();
}

void Resolver::visitBinary(Ptr<BinaryExpr> expr)
{
	resolve(expr->left);
	resolve(expr->right);
}

void Resolver::visitUnary(Ptr<UnaryExpr> expr)
{
	resolve(expr->right);
}

void Resolver::visitGrouping(Ptr<GroupingExpr> expr)
{
	resolve(expr->expr);
}

void Resolver::visitVariable(Ptr<VariableExpr> expr)
{

	if (!m_scopes.empty() && m_scopes.back()[expr->name->lexeme] == false)
	{
		// TODO: Investigate why this gets triggered (the second condition passes)
		// throw CruxResolverError(expr->name->lexeme + ": Cannot read local variable in its own initializer.");
	}

	resolveLocal(expr, expr->name);
}

void Resolver::visitAssign(Ptr<AssignExpr> expr)
{
	resolve(expr->value);
	resolveLocal(expr, expr->name);
}

void Resolver::visitLogical(Ptr<LogicalExpr> expr)
{
	resolve(expr->left);
	resolve(expr->right);
}

void Resolver::visitCall(Ptr<CallExpr> expr)
{
	resolve(expr->callee);

	for (auto& arg : expr->arguments)
	{
		resolve(arg);
	}
}

void Resolver::visitGet(Ptr<GetExpr> expr)
{
	resolve(expr->object);
}

void Resolver::visitSet(Ptr<SetExpr> expr)
{
	resolve(expr->value);
	resolve(expr->object);
}

void Resolver::visitThis(Ptr<ThisExpr> expr)
{
	if (m_currentClass == ClassType::None) {
		throw CruxResolverError("Can't use <this> outside of a class.", expr->keyword->line, expr->keyword->lexeme);
	}

	resolveLocal(expr, expr->keyword);
}
