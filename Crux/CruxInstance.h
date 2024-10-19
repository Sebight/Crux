#pragma once
#include <map>

class CruxClass;

class CruxInstance : public CruxObject {
public:
	CruxInstance(Ptr<CruxClass> klass) : m_klass(klass) {}

	virtual Ptr<CruxObject> get(Ptr<Token> name);
	void set(Ptr<Token> name, Ptr<CruxObject> value);

	std::string toString();
private:
	Ptr<CruxClass> m_klass;
	std::map<std::string, Ptr<CruxObject>> m_fields;
};