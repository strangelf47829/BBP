#include "../include/Kernel.h"
#include "../include/drvcmd.h"

void BBP::system::Kernel::startCapture()
{
	singleton.keyboardDriver.hardwareDriver.Connect();
}

void BBP::system::Kernel::stopCapture()
{
	singleton.keyboardDriver.hardwareDriver.Disconnect();
}

BBP::std::size_t BBP::system::Kernel::getKeyCount()
{
	// Declare
	std::size_t count = 0;

	// Send command
	bool couldGetCount = singleton.keyboardDriver.hardwareDriver.executeCommand(std::getKeyboardKeyCount, 1, &count);

	// If could not get count, return 0
	if (couldGetCount == false)
		return 0;

	// Return count
	return count;
}

// Get key
BBP::std::string_element BBP::system::Kernel::getKeyboardKey()
{
	// Declare key
	std::word key = 0;

	// Send command
	singleton.keyboardDriver.hardwareDriver.executeCommand(std::getKeyboardKeyInput, 1, &key);

	// Return key
	return key;
}

// Wait until key is available, then retrieve
BBP::std::string_element BBP::system::Kernel::getKey()
{
	// While no key is available, wait
	while (singleton.getKeyCount() == 0);

	// Then return key
	return singleton.getKeyboardKey();
}

// Retrieve key anyway
BBP::std::string_element BBP::system::Kernel::getKeyAsync()
{
	// If no key is available return nothing
	if (singleton.getKeyCount() == 0)
		return 0;

	// Otherwise return key
	return singleton.getKeyboardKey();
}