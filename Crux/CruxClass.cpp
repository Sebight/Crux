#include "CruxCallable.h"
#include "CruxCallable.h"
#include <map>
#include "CruxClass.h"
#include "CruxInstance.h"

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
	return instance;
}