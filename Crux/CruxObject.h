#pragma once

#include <string>
#include "Token.h"
#include <memory>
#include "Types.h"
#include <vector>

class CruxObject : std::enable_shared_from_this<CruxObject> {
public:
	std::string str;
	double num;
	bool boolean;
	TokenType type;

	virtual ~CruxObject() = default;

	CruxObject();
	CruxObject(std::string str);
	CruxObject(double num);
	CruxObject(bool boolean);

	virtual std::string toString();
};

