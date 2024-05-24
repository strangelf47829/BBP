#include "../include/Kernel.h"
#include "../include/drvcmd.h"

void BBP::system::Kernel::clearScreen()
{
	singleton.screenDriver.hardwareDriver.executeCommand(std::screenClearScreen, 0, nullptr);
}

void BBP::system::Kernel::printString(std::string &str)
{
	singleton.screenDriver.writeData(str);
}

void BBP::system::Kernel::biosSplashCommand(UEFI &configuration)
{
	// Do this pretty much
	singleton.screenDriver.hardwareDriver.executeCommand(std::screenBIOSSplashScreen, 1, (std::word *)&configuration);
}