#include "../../../../include/Time.h"
#include "../../include/SystemDriver.h"
#include <chrono>

// Times
::std::time_t startup_time_micros;
::std::time_t startup_time_millis;

// Get current milliseconds from system
BBP::std::time_t now_millis()
{
	return ::std::chrono::duration_cast<::std::chrono::milliseconds>(::std::chrono::steady_clock::now().time_since_epoch()).count();
}

// Get current microseconds from system
BBP::std::time_t now_micros()
{
	return ::std::chrono::duration_cast<::std::chrono::microseconds>(::std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

// Initialize the clock
bool Environment::Drivers::System::initClock(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Initialize clocks
	startup_time_micros = now_micros();
	startup_time_millis = now_millis();

	// Success
	return true;
}

// Get milliseconds
bool Environment::Drivers::System::getClockMillis(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Get current time
	BBP::std::time_t milliseconds = now_millis() - startup_time_millis;

	// If argc is 0 or argv is nullptr,
	if (argc == 0 || argv == nullptr)
		return false; // error

	// Otherwise set argv[0] to time
	argv[0] = milliseconds;

	// Return success
	return true;
}

bool Environment::Drivers::System::getClockMicros(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Get current time
	BBP::std::time_t microseconds = now_micros() - startup_time_micros;

	// If argc is 0 or argv is nullptr,
	if (argc == 0 || argv == nullptr)
		return false; // error

	// Otherwise set argv[0] to time
	argv[0] = microseconds;

	// Return success
	return true;
}