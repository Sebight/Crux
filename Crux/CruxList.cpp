#include "CruxCallable.h"
#include "CruxInstance.h"
#include "CruxList.h"

int CruxListConstructor::arity()
{
	// TODO: Items in list init
	return 0;
}

Ptr<CruxObject> CruxListConstructor::call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments)
{
	return std::make_shared<CruxList>();
}

// List impl

CruxList::CruxList() : CruxInstance::CruxInstance(nullptr)
{
}

std::string CruxList::toString()
{
	std::string str = "";
	for (int i = 0; i < m_items.size(); i++) {
		if (i == 0) str += "[";
		str += " ";
		str += m_items[i]->toString();
		str += " ";
		
		if (i != m_items.size() - 1) {
			str += ",";
		}
		else {
			str += "]";
		}
	}
	return str;
}

Ptr<CruxObject> CruxList::get(Ptr<Token> name)
{
	if (name->lexeme == "size") {
		return std::make_shared<CruxObject>(static_cast<double>(m_items.size()));
	}
	else if (name->lexeme == "get") {
		return std::make_shared<CruxListAccessMethod>(this, CruxListAccessMethodType::Get);
	}
	else if (name->lexeme == "push") {
		return std::make_shared<CruxListAccessMethod>(this, CruxListAccessMethodType::Add);
	}
	else {
		return nullptr;
	}
}

Ptr<CruxObject> CruxList::at(int index)
{
	return m_items[index];
}

void CruxList::push(Ptr<CruxObject> item)
{
	m_items.push_back(item);
}

CruxListAccessMethod::CruxListAccessMethod(CruxList* list, CruxListAccessMethodType type)
{
	m_list = list;
	m_accessType = type;
}

int CruxListAccessMethod::arity()
{
	// Add with 1 arg
	// Get with 1 arg
	return 1;
}

Ptr<CruxObject> CruxListAccessMethod::call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments)
{
	if (m_accessType == CruxListAccessMethodType::Get) {
		int index = static_cast<int>(arguments[0]->num);
		return m_list->at(index);
	}
	else {
		m_list->push(arguments[0]);
		return std::make_shared<CruxObject>();
	}
}
