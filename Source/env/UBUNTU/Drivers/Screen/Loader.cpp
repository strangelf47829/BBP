#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

void Environment::Drivers::screen::loadScreenDriver(BBP::system::DeviceDriver &driver)
{
	// Set stack to kernel stuff whatever
	stdoutStack = BBP::std::Stack<BBP::std::string_element>(&driver.hardwareDriver.getOutput(), driver.hardwareDriver.getOutput().dataSize);

	// When written to, use the print screen thingy
	stdoutStack.writeTo = print_stack_to_string;

	// This driver is builtin.
	driver.hardwareDriver.hwid = 0; // Reset

	// Set appropriate flags
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::Builtin;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::Async;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::External;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::SimplexReceiver;
	driver.hardwareDriver.hwid |= BBP::system::HardwareClassification::System;

	// Also do the same for STDOUT
	BBP::system::kernelSS()->getKernelSystemContext()->STDOUT.writeTo = print_stack_to_string;

	// Then set input page
	driver.softwareDriver.setOutputPage(&stdoutStack);
}