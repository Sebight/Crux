#pragma once
#include "CruxCallable.h"
#include "AST.h"

class CruxInstance;

class CruxFunction : public CruxCallable {
public:
	CruxFunction(Ptr<FunctionStmt> declaration, Ptr<Env> closure, bool isConstructor) : m_declaration(std::move(declaration)), m_closure(closure), m_isConstructor(isConstructor) {}
	
	Ptr<CruxObject> call(Ptr<Interpreter> interpreter, std::vector<Ptr<CruxObject>> arguments) override;
	int arity() override;
	
	Ptr<CruxFunction> bind(Ptr<CruxInstance> instance);

	std::string toString() override {
		return "fun<" + m_declaration->name->lexeme + ">";
	}
private:
	Ptr<FunctionStmt> m_declaration;
	Ptr<Env> m_closure;
	bool m_isConstructor;
};