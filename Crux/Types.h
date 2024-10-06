#pragma once
#include <memory>
#include <string>


template<typename T>
using Ptr = std::shared_ptr<T>;

class CruxObject;

class CruxRuntimeError : public std::exception {
public:
	CruxRuntimeError(std::string message, int line = -1, std::string token = "") : m_message(message) {
		if (line != -1 && !token.empty()) {
			m_message = "Line " + std::to_string(line) + ": " + message + " at token: " + token;
		}
		else {
			m_message = message;
		}
	}
	const char* what() const noexcept override {
		return m_message.c_str();
	}

private:
	std::string m_message;
};

class CruxResolverError : public std::exception {
public:
	CruxResolverError(std::string message, int line = -1, std::string token = "") : m_message(message) {
		if (line != -1 && !token.empty()) {
			m_message = "Line " + std::to_string(line) + ": " + message + " at token: " + token;
		}
		else {
			m_message = message;
		}
	}
	const char* what() const noexcept override {
		return m_message.c_str();
	}

private:
	std::string m_message;
};

// TODO: Look into optimizing stack trace overhead, etc.
class CruxReturn : public std::exception {
public:
	Ptr<CruxObject> value;

	CruxReturn(Ptr<CruxObject> value) : value(value) { }
};

class ParseError : public std::exception {
public:
	ParseError(std::string message, int line = -1) : m_line(line), m_message(message) {
		message = "";

		if (m_line != -1) {
			message = "Parse Error at line " + std::to_string(m_line) + ": " + m_message;
		}
		else {
			message = "Parse Error: " + m_message;
		}

		m_message = message;
	}
	const char* what() const noexcept override {
		return m_message.c_str();
	}

private:
	int m_line;
	std::string m_message;
};