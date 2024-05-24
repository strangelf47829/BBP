#include "../include/Kernel.h"
#include "../include/drvcmd.h"

void BBP::system::Kernel::initClock()
{
	// Send command to system driver
	singleton.systemDriver.hardwareDriver.executeCommand(std::initClock, 0, nullptr);
}

BBP::std::time_t BBP::system::Kernel::millis()
{
	// Declare time
	std::time_t time = 0;

	// Send syscall
	singleton.systemDriver.hardwareDriver.executeCommand(std::getMillis, 1, &time);

	// Return
	return time;
}

BBP::std::time_t BBP::system::Kernel::micros()
{
	// Declare time
	std::time_t time = 0;

	// Send syscall
	singleton.systemDriver.hardwareDriver.executeCommand(std::getMicros, 1, &time);

	// Return
	return time;
}