#pragma once
#include "CruxCallable.h"
#include "AST.h"

class CruxFunction : public CruxCallable {
public:
	CruxFunction(Ptr<FunctionStmt> declaration, Ptr<Env> closure) : m_declaration(std::move(declaration)), m_closure(closure) {}
	Ptr<CruxObject> call(Ptr<Interpreter> interpreter, std::vector<Ptr<CruxObject>> arguments) override;
	int arity() override;
	std::string toString() override {
		return "fun<" + m_declaration->name->lexeme + ">";
	}
private:
	Ptr<FunctionStmt> m_declaration;
	Ptr<Env> m_closure;
};