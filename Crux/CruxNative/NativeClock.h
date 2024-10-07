#pragma once
#include "../CruxCallable.h"
#include <chrono>

class NativeClock : public CruxCallable {
public: 
	int arity() override {
		return 0;
	}
	Ptr<CruxObject> call(Interpreter* interpreter, std::vector<Ptr<CruxObject>> arguments) override;
};