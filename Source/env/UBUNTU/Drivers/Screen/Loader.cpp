#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

void Environment::Drivers::screen::loadScreenDriver(BBP::system::DeviceDriver &driver)
{
	// Set stack to kernel stuff whatever
	stdoutStack = BBP::std::Stack<BBP::std::string_element>(&driver.hardwareDriver.getOutput(), driver.hardwareDriver.getOutput().dataSize);

	// This driver is builtin.
	driver.hardwareDriver.hwid = 0; // Reset

	// Set appropriate flags
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::Builtin;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::Async;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::External;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::SimplexReceiver;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::System;

	// Set commands
	driver.hardwareDriver.setHandleData(screenManifest.actions, screenManifest.commandCount, screenManifest.commands);

	// Also do the same for STDOUT, and STDERR
	BBP::system::kernelSS()->getKernelSystemContext()->STDOUT.writeTo = print_stack_to_string;
	BBP::system::kernelSS()->getKernelSystemContext()->STDERR.writeTo = print_stack_to_string;

	// Then set input page
	driver.softwareDriver.setOutputPage(&stdoutStack);
}