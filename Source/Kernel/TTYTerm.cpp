#include "../include/Kernel.h"
#include "../include/drvcmd.h"

void BBP::system::Kernel::clearScreen()
{
	// Core implemented
	singleton.Core().firmware->clearScreen();
}

void BBP::system::Kernel::printString(std::string &str)
{
	// Core implemented
	singleton.Core().firmware->printString(str);
}

void BBP::system::Kernel::biosSplashCommand(EFI &configuration)
{
	// Do this pretty much
	singleton.Core().firmware->HardwareScreen.hardwareDriver.executeCommand(std::screenBIOSSplashScreen, 1, (std::word *)&configuration);
}