#include "CruxFunction.h"
#include "CruxFunction.h"
#include "Interpreter.h"

Ptr<CruxObject> CruxFunction::call(Ptr<Interpreter> interpreter, std::vector<Ptr<CruxObject>> arguments) {
	Ptr<Env> funcEnv = std::make_shared<Env>(m_closure);
	for (int i = 0; i < m_declaration->params.size(); i++) {
		funcEnv->define(m_declaration->params[i]->lexeme, arguments[i]);
	}

	try {
		interpreter->executeBlock(m_declaration->body, funcEnv);
	}
	catch (CruxReturn& e) {
		return std::move(e.value);
	}

	return std::make_shared<CruxObject>();
}

int CruxFunction::arity()
{
	return m_declaration->params.size();
}
