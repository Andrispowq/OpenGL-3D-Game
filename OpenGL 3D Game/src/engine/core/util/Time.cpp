#include "Time.h"

namespace Time
{
	double Time::getTime()
	{
		using std::chrono::seconds;
		using std::chrono::duration_cast;

		return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - start).count() / pow(10, 9);
	}
};