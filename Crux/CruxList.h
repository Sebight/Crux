#pragma once

class Interpreter;

class CruxListConstructor : public CruxCallable {
public:
	CruxListConstructor() = default;

	int arity() override;

	Ptr<CruxObject> call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments) override;
};

class CruxList : public CruxInstance {
public:
	CruxList();

	std::string toString();

	Ptr<CruxObject> get(Ptr<Token> name) override;

	Ptr<CruxObject> at(int index);
	void push(Ptr<CruxObject> item);

private:
	std::vector<Ptr<CruxObject>> m_items;
};

enum class CruxListAccessMethodType {
	Get,
	Add
};

class CruxListAccessMethod : public CruxCallable {
public:
	CruxListAccessMethod(CruxList* list, CruxListAccessMethodType type);

	int arity() override;
	Ptr<CruxObject> call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments) override;
private:
	CruxList* m_list;
	CruxListAccessMethodType m_accessType;
};