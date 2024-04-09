#include "../../../include/Time.h"
#include <chrono>

::std::time_t startup_time_micros;
::std::time_t startup_time_millis;

void BBP::std::initClock()
{
	startup_time_micros = now_micros();
	startup_time_millis = now_millis();
}

BBP::std::time_t BBP::std::now_millis()
{
	return ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count();
}

BBP::std::time_t BBP::std::now_micros()
{
	return ::std::chrono::duration_cast<::std::chrono::microseconds>(::std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

BBP::std::time_t BBP::std::millis()
{
	return now_millis() - startup_time_millis;
}

BBP::std::time_t BBP::std::micros()
{
	return now_micros() - startup_time_micros;
}

void BBP::std::delay(std::time_t _millis)
{
	time_t _now = BBP::std::millis();
	while (_now + _millis > BBP::std::millis());
}