#include "../include/FirmwareInterface.h"
#include "../include/drvcmd.h"

void BBP::FirmwareInterface::startCapture()
{
	HardwareKeyboard.hardwareDriver.Connect();
}

void BBP::FirmwareInterface::stopCapture()
{
	HardwareKeyboard.hardwareDriver.Disconnect();
}

BBP::std::size_t BBP::FirmwareInterface::getKeyCount()
{
	// Declare
	std::size_t count = 0;

	// Send command
	bool couldGetCount = HardwareKeyboard.hardwareDriver.executeCommand(std::getKeyboardKeyCount, 1, &count);

	// If could not get count, return 0
	if (couldGetCount == false)
		return 0;

	// Return count
	return count;
}

// Get key
BBP::std::string_element BBP::FirmwareInterface::getKeyboardKey()
{
	// Declare key
	std::word key = 0;

	// Send command
	HardwareKeyboard.hardwareDriver.executeCommand(std::getKeyboardKeyInput, 1, &key);

	// Return key
	return key;
}

// Wait until key is available, then retrieve
BBP::std::string_element BBP::FirmwareInterface::getKey()
{
	// While no key is available, wait
	while (this->getKeyCount() == 0);

	// Then return key
	return this->getKeyboardKey();
}

// Retrieve key anyway
BBP::std::string_element BBP::FirmwareInterface::getKeyAsync()
{
	// If no key is available return nothing
	if (this->getKeyCount() == 0)
		return 0;

	// Otherwise return key
	return this->getKeyboardKey();
}