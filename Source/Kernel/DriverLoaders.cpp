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
	// Create path out of configuration
	std::PATH path = configuration.system.volumePath;

	// Check if that path is a folder or not
	bool isPathADirectory = path.isDirectory();

	// Giving a non-directory path is not supported, if not a directory show invalid configuration
	if (isPathADirectory == false)
		return invalidConfig;

	// Drivers themselves were configured at boot, now configure the kernel to work with these drivers
	singleton.Core().systemVolume.volumePath = path;

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
