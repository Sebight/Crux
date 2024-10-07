#pragma once

class CruxFunction;

class CruxClass : public CruxCallable {
public:
	CruxClass(std::string name, Ptr<CruxClass> superclass, std::map<std::string, Ptr<CruxFunction>> methods) : m_name(name), m_methods(methods), m_superclass(superclass) {}

	std::string toString();

	int arity() override;

	std::string getName() {
		return m_name;
	}

	Ptr<CruxFunction> findMethod(std::string name);

	Ptr<CruxObject> call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments) override;
private:
	std::string m_name;
	std::map<std::string, Ptr<CruxFunction>> m_methods;
	Ptr<CruxClass> m_superclass;
};