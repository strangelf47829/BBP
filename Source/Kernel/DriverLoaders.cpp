#include "../include/Kernel.h"

BBP::std::errno_t BBP::system::Kernel::loadKeyboardDriver(EFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadKeyboard == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadKeyboard(singleton.Core().firmware->HardwareKeyboard);

	// Start capturing input
	singleton.Core().firmware->startCapture();

	// Return no errors
	return 0;
}

BBP::std::errno_t BBP::system::Kernel::loadScreenDriver(EFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadScreen == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadScreen(singleton.Core().firmware->HardwareScreen);

	// Return no errors
	return 0;
}

BBP::std::errno_t BBP::system::Kernel::loadFileDriver(EFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadFileSystem == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadFileSystem(singleton.Core().firmware->HardwareFile);

	// Return no errors
	return 0;
}

BBP::std::errno_t BBP::system::Kernel::loadSystemDriver(EFI &configuration)
{
	// Check if function exists
	if (configuration.drivers.loadSystem == nullptr)
		return invalidConfig;

	// Execute driver init function
	configuration.drivers.loadSystem(singleton.Core().firmware->HardwareSystem);
	
	// Return no errors
	return 0;
}