#include "../../../../include/DeviceDriver.h"
#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"

// Load the keyboard
void Environment::Drivers::keyboard::loadKeyboardDriver(BBP::system::DeviceDriver &driver)
{
	// Set commands
	driver.hardwareDriver.setHandleData(keyboardManifest.actions, keyboardManifest.commandCount, keyboardManifest.commands);
}