#include "../include/Time.h"
#include <chrono>

::std::time_t startup_time;

void BBP::std::initClock()
{
	startup_time = now();
}

BBP::std::time_t BBP::std::now()
{
	return ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count();
}

BBP::std::time_t BBP::std::millis()
{
	return now() - startup_time;
}

void BBP::std::delay(std::time_t _millis)
{
	time_t _now = BBP::std::millis();
	while (_now + _millis > BBP::std::millis());
}