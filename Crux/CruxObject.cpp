#include "CruxObject.h"

CruxObject::CruxObject()
{
	this->type = TokenType::NIL;
}

CruxObject::CruxObject(double num) {
	this->num = num;
	this->type = TokenType::NUMBER;
}

CruxObject::CruxObject(std::string str) {
	this->str = str;
	this->type = TokenType::STRING;
}

CruxObject::CruxObject(bool boolean) {
	this->boolean = boolean;
	this->type = boolean ? TokenType::TRUE : TokenType::FALSE;
}

std::string CruxObject::toString()
{
	switch (this->type)
	{
	case TokenType::NIL:
		return "nil";
	case TokenType::NUMBER:
		return std::to_string(this->num);
	case TokenType::STRING:
		return this->str;
	case TokenType::TRUE:
		return "true";
	case TokenType::FALSE:
		return "false";
	default:
		return "unknown";
	}
}
