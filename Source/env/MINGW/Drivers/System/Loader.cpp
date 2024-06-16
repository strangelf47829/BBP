#include "../../../include/SystemDriver.h"

// Reduces amount of characters needed to type
namespace System = Host::Drivers::System;

// Amount of system command
constexpr BBP::std::size_t systemCMDsize = 8;

// List of system commands and actions
BBP::system::HardwareCmd systemCMD[systemCMDsize] = { System::connectSystem, System::disconnectSystem, System::handleSystemCall, System::initClock, System::getClockMillis, System::getClockMicros, System::getUnixTimestamp, System::getStartupTimestamp };
BBP::system::HardwareAction systemActions[3] = { System::sendDataToSystem, System::receiveDataFromSystem, System::receiveSystemMetadata };

void Host::Drivers::System::loadSystemDriver(BBP::system::DeviceDriver &driver)
{
	// This driver is builtin.
	driver.hardwareDriver.hwid = 0; // Reset

	// Set appropriate flags
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::Builtin;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::Async;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::External;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::SimplexReceiver;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::System;

	// Set commands
	driver.hardwareDriver.setHandleData(systemActions, systemCMDsize, systemCMD);
}