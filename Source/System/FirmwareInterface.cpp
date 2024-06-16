#include "../include/FirmwareInterface.h"
#include "../include/cstdio.h"

BBP::FirmwareInterface::FirmwareInterface(BBP::BIOS *b, BBP::system::EFI *cfg)
	: bios(b),
	biosMode(true),
	configuration(cfg),
	HardwareKeyboard(nullptr, nullptr, "Keyboard", nullptr, 0, nullptr),
	HardwareScreen(nullptr, nullptr, "Screen", nullptr, 0, nullptr),
	HardwareFile(nullptr, nullptr, "FileSystem", nullptr, 0, nullptr),
	HardwareSystem(nullptr, nullptr, "System", nullptr, 0, nullptr)
{

	// Check for valid bios
	if (bios == nullptr)
		return; // Invalid

	// Check for configuration
	if (configuration == nullptr)
		return; // Invalid

	// Attempt to load system driver first
	if (configuration->drivers.loadSystem)
		configuration->drivers.loadSystem(HardwareSystem);

	// Then load file system
	if (configuration->drivers.loadFileSystem)
		configuration->drivers.loadFileSystem(HardwareFile);

	// Then load keyboard driver
	if (configuration->drivers.loadKeyboard)
		configuration->drivers.loadKeyboard(HardwareKeyboard);

	// Finally load screen driver
	if (configuration->drivers.loadScreen)
		configuration->drivers.loadScreen(HardwareScreen);

	// If other drivers exist, load those
	if (configuration->drivers.loadOtherDrivers)
		configuration->drivers.loadOtherDrivers();

}

void BBP::FirmwareInterface::SwitchToDrivers()
{
	// Set appropriate flag
	biosMode = false;

	// Initialize timers
	initClock();

	// Start capture
	startCapture();

	// Initialize terminal
	initDisplay();
}