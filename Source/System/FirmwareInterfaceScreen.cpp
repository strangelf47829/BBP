#include "../include/FirmwareInterface.h"
#include "../include/drvcmd.h"

void BBP::FirmwareInterface::clearScreen()
{
	HardwareScreen.hardwareDriver.executeCommand(std::screenClearScreen, 0, nullptr);
}

void BBP::FirmwareInterface::printString(std::string &str)
{
	HardwareScreen.writeData(str);
}