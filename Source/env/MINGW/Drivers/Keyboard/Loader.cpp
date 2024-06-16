#include "../../../include/KeyboardDriver.h"
#include "../../../../include/drvcmd.h"

// Reduces amount of characters needed to type
namespace KeyB = Host::Drivers::Keyboard;

// Amount of screen commands
constexpr BBP::std::size_t keyboardCMDsize = 5;

// List of screen commands and actions
BBP::system::HardwareCmd keyboardCMD[keyboardCMDsize] = { KeyB::connectKeyboard, KeyB::disconnectKeyboard, KeyB::keyCount, KeyB::captureKey, KeyB::pollKey };
BBP::system::HardwareAction keyboardActions[3] = { KeyB::sendDataToKeyboard, KeyB::receiveDataFromKeyboard, KeyB::receiveKeyboardMetadata };

void Host::Drivers::Keyboard::loadKeyboardDriver(BBP::system::DeviceDriver &driver)
{
	// Set commands
	driver.hardwareDriver.setHandleData(keyboardActions, keyboardCMDsize, keyboardCMD);
}