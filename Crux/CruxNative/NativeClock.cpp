#include "NativeClock.h"

Ptr<CruxObject> NativeClock::call(Ptr<Interpreter> interpreter, std::vector<Ptr<CruxObject>> arguments)
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return std::make_shared<CruxObject>((double)millis);
}