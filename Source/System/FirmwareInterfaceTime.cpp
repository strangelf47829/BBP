#include "../include/FirmwareInterface.h"
#include "../include/drvcmd.h"

void BBP::FirmwareInterface::initClock()
{
	// Send command to system driver
	HardwareSystem.hardwareDriver.executeCommand(std::initClock, 0, nullptr);
}

BBP::std::time_t BBP::FirmwareInterface::millis()
{
	// Declare time
	std::time_t time = 0;

	// Send syscall
	HardwareSystem.hardwareDriver.executeCommand(std::getMillis, 1, &time);

	// Return
	return time;
}

BBP::std::time_t BBP::FirmwareInterface::micros()
{
	// Declare time
	std::time_t time = 0;

	// Send syscall
	HardwareSystem.hardwareDriver.executeCommand(std::getMicros, 1, &time);

	// Return
	return time;
}