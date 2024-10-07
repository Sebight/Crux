#include "CruxFunction.h"
#include "CruxInstance.h"
#include "Interpreter.h"

Ptr<CruxObject> CruxFunction::call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments) {
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

Ptr<CruxFunction> CruxFunction::bind(Ptr<CruxInstance> instance)
{
	Ptr<Env> env = std::make_shared<Env>(m_closure);
	env->define("this", instance);
	return std::make_shared<CruxFunction>(m_declaration, env, m_isConstructor);
}
