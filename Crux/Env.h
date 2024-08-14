#pragma once

#include <unordered_map>
#include <string>
#include "CruxObject.h"
#include "Types.h"

class Env {
public:
	Env() = default;
	~Env() = default;

	void define(std::string name, Ptr<CruxObject> value) {
		m_values[name] = std::move(value);
	}

	void assign(Ptr<Token> name, Ptr<CruxObject> value) {
		auto it = m_values.find(name->lexeme);
		if (it != m_values.end()) {
			it->second = value;
			return;
		}

		throw CruxRuntimeError("Undefined variable: " + name->lexeme + ".", name->line, name->lexeme);
	}

	Ptr<CruxObject> get(Ptr<Token> name) {
		auto it = m_values.find(name->lexeme);
		if (it != m_values.end()) {
			return it->second;
		}

		throw CruxRuntimeError("Undefined variable: " + name->lexeme + ".", name->line, name->lexeme);
	}
private:
	std::unordered_map<std::string, Ptr<CruxObject>> m_values;
};