#include "../../include/SystemDriver.h"
#include "../../include/Environment.h"

void Environment::Drivers::System::loadSystemDriver(BBP::system::DeviceDriver &driver)
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
	driver.hardwareDriver.setHandleData(systemManifest.actions, systemManifest.commandCount, systemManifest.commands);
}