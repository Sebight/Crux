#pragma once

#include "CruxObject.h"

class Interpreter;
class Env;

class CruxCallable : public CruxObject {
public:
	CruxCallable() = default;
	virtual int arity() = 0;
	virtual Ptr<CruxObject> call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments) = 0;
};
