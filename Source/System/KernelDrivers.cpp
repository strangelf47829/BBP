// This file initializes software defined drivers for use before booting.
// The minimum amount of drivers needed: Keyboard input, screen output, and file IO.

#include "../include/DeviceDriver.h"
#include "../include/Kernel.h"

// Device driver constructor
BBP::system::DeviceDriver::DeviceDriver(std::Stack<std::string_element> *input, std::Stack<std::string_element> *output, std::conststring name, HardwareAction actions[3], std::size_t cmdc, const HardwareCmd *cmdv)
	: softwareDriver(input, output, name),
	hardwareDriver(actions, cmdc, cmdv)
{

}

BBP::std::word BBP::system::DeviceDriver::writeData(std::c_string data)
{
	// Then tell hardware handle to write that shiiit
	hardwareDriver.send(std::strlen(data));
}


// Load a driver
BBP::std::errno_t BBP::system::Kernel::loadDriver(UEFILoadDriver loadDriver, DeviceDriver &intoDriver, BBP::std::size_t argc, BBP::std::word *argv)
{
	// Check if function exists
	if (loadDriver == nullptr)
		return invalidConfig;

	// Execute driver init function
	loadDriver(intoDriver);

	// Execute connection command
	intoDriver.hardwareDriver.executeCommand(intoDriver.hardwareDriver.connectCmd, argc, argv);
}

// Load a driver
BBP::std::errno_t BBP::system::Kernel::loadDriver(UEFILoadDriver loadDriver, DeviceDriver &intoDriver)
{
	// Check if function exists
	if (loadDriver == nullptr)
		return invalidConfig;

	// Execute driver init function
	loadDriver(intoDriver);

	return 0;
}
