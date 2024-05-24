#include "../include/Kernel.h"

BBP::std::errno_t BBP::system::Kernel::loadKeyboardDriver(UEFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadKeyboard == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadKeyboard(keyboardDriver);

	// Start capturing input
	startCapture();

	// Return no errors
	return 0;
}

BBP::std::errno_t BBP::system::Kernel::loadScreenDriver(UEFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadScreen == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadScreen(screenDriver);

	// Return no errors
	return 0;
}

BBP::std::errno_t BBP::system::Kernel::loadFileDriver(UEFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadFileSystem == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadFileSystem(fileDriver);

	// Return no errors
	return 0;
}

BBP::std::errno_t BBP::system::Kernel::loadSystemDriver(UEFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadSystem == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadSystem(systemDriver);
	
	// Return no errors
	return 0;
}