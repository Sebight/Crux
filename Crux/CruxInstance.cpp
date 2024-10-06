#include "CruxObject.h"
#include "CruxCallable.h"
#include <map>
#include "CruxClass.h"
#include "CruxInstance.h"
#include "CruxFunction.h"

Ptr<CruxObject> CruxInstance::get(Ptr<Token> name)
{
	if (m_fields.find(name->lexeme) != m_fields.end()) {
		return m_fields[name->lexeme];
	}

	Ptr<CruxFunction> method = m_klass->findMethod(name->lexeme);
	if (method != nullptr) {
		return method->bind(static_pointer_cast<CruxInstance>(shared_from_this()));
	}

	throw CruxRuntimeError("Undefined property.", name->line, name->lexeme);
}

void CruxInstance::set(Ptr<Token> name, Ptr<CruxObject> value)
{
	m_fields[name->lexeme] = value;
}

std::string CruxInstance::toString()
{
	return m_klass->getName() + " instance";
}
