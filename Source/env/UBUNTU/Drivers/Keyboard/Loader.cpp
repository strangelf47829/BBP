#include "../../../../include/DeviceDriver.h"
#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"

// Load the keyboard
void Environment::Drivers::keyboard::loadKeyboardDriver(BBP::system::DeviceDriver &driver)
{
	// Set stack to hardware input stuff
	keyboardStack = BBP::std::Stack<BBP::std::string_element>(&BBP::system::getKernelInstance().getKeyboardDriver().hardwareDriver.getInput(), BBP::system::deviceBufferSize);

	// Set commands
	driver.hardwareDriver.setHandleData(keyboardManifest.actions, keyboardManifest.commandCount, keyboardManifest.commands);

	// Set keyboard stack to keyboard stack
	BBP::system::getKernelInstance().getKeyboardDriver().softwareDriver.setInputPage(&keyboardStack);
}