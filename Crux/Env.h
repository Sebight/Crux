#pragma once

#include <unordered_map>
#include <string>
#include "CruxObject.h"
#include "Types.h"

class Env : public std::enable_shared_from_this<Env> {
public:
	Env() = default;
	Env(Ptr<Env> enclosing) : m_enclosing(enclosing) { }
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

		if (m_enclosing != nullptr) {
			m_enclosing->assign(name, value);
			return;
		}

		throw CruxRuntimeError("Undefined variable: " + name->lexeme + ".", name->line, name->lexeme);
	}

	void assignAt(int distance, Ptr<Token> name, Ptr<CruxObject> value) {
		ancestor(distance)->m_values[name->lexeme] = value;
	}

	Ptr<CruxObject> get(Ptr<Token> name) {
		auto it = m_values.find(name->lexeme);
		if (it != m_values.end()) {
			return it->second;
		}

		if (m_enclosing != nullptr) {
			return m_enclosing->get(name);
		}

		throw CruxRuntimeError("Undefined variable: " + name->lexeme + ".", name->line, name->lexeme);
	}

	Ptr<CruxObject> getAt(int distance, std::string name) {
		Ptr<Env> env = ancestor(distance);
		return env->m_values[name];
	}

	Ptr<Env> ancestor(int distance) {
		Ptr<Env> env = shared_from_this();
		for (int i = 0; i < distance; i++) {
			env = env->m_enclosing;
		}

		return env;
	}

	void dump(bool rec = false) {
		std::string content;
		size_t index = 0;
		std::string level = "NESTED";
		if (m_enclosing == nullptr) {
			level = "GLOBAL";
		}
		content += "[Dumping env]\n";
		for (auto& [key, value] : m_values) {
			content += level + " - " + "[" + std::to_string(index) + "] " + key + " => " + value->toString() + "\n";
			index++;
		}

		printf("%s", content.c_str());
		if (rec && m_enclosing != nullptr) {
			m_enclosing->dump(true);
		}
	}

	Ptr<Env> enclosing() {
		return m_enclosing;
	}
private:
	std::unordered_map<std::string, Ptr<CruxObject>> m_values;
	Ptr<Env> m_enclosing;
};