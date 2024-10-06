#include "CruxCallable.h"
#include <map>
#include "CruxClass.h"
#include "CruxInstance.h"
#include "CruxFunction.h"

int CruxClass::arity()
{
	Ptr<CruxFunction> constructor = findMethod(m_name);
	return constructor == nullptr ? 0 : constructor->arity();
}

Ptr<CruxFunction> CruxClass::findMethod(std::string name)
{
	if (m_methods.find(name) != m_methods.end()) {
		return m_methods[name];
	}

	return nullptr;
}

Ptr<CruxObject> CruxClass::call(Ptr<Interpreter> interpreter, std::vector<Ptr<CruxObject>> arguments) {
	Ptr<CruxClass> thisPtr = std::dynamic_pointer_cast<CruxClass>(shared_from_this());
	Ptr<CruxInstance> instance = std::make_shared<CruxInstance>(thisPtr);

	Ptr<CruxFunction> constructor = findMethod(m_name);
	if (constructor != nullptr) {
		constructor->bind(instance)->call(interpreter, arguments);
	}

	return instance;
}