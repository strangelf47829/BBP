// This file initializes software defined drivers for use before booting.
// The minimum amount of drivers needed: Keyboard input, screen output, and file IO.

#include "../include/DeviceDriver.h"
#include "../include/Kernel.h"

// Device driver constructor
BBP::system::DeviceDriver::DeviceDriver(std::Stack<std::string_element> *input, std::Stack<std::string_element> *output, std::conststring name, HardwareAction actions[3], std::size_t cmdc, const HardwareCmd *cmdv)
	: stdinp(nullptr, 0),
	stdout(nullptr, 0),
	softwareDriver(input, output, name),
	hardwareDriver(actions, cmdc, cmdv)
{
	// Reset stdinput
	stdinp = std::Stack<std::string_element>(&hardwareDriver.getInput(), deviceBufferSize);
	stdout = std::Stack<std::string_element>(&hardwareDriver.getOutput(), deviceBufferSize);

	// Check if input is nullptr. If it is, set stdinp
	if (input == nullptr)
		softwareDriver.setInputPage(&stdinp);

	// Check if output is nullptr. If it is, set stdout
	if (output == nullptr)
		softwareDriver.setOutputPage(&stdout);
}

void BBP::system::DeviceDriver::Associate()
{
	softwareDriver.setInputPage(&stdinp);
	softwareDriver.setOutputPage(&stdout);
}

BBP::std::word BBP::system::DeviceDriver::writeData(std::c_string data)
{
	// Write into software buffer
	softwareDriver << data;

	// Then send amount of data out
	std::size_t bytesSent = hardwareDriver.send(std::strlen(data));

	// Then Seek back the amount of data sent
	softwareDriver.seekOutputBack(bytesSent + 1);

	// Then return amount of bytes sent
	return bytesSent;
}

BBP::std::word BBP::system::DeviceDriver::writeData(std::string data)
{
	// Write into software buffer
	softwareDriver << data.data;

	// Then send amount of data out
	std::size_t bytesSent = hardwareDriver.send(std::strlen(data));

	// Then Seek back the amount of data sent
	softwareDriver.seekOutputBack(bytesSent + 1);

	// Then return amount of bytes sent
	return bytesSent;
}

// Read data
BBP::std::word BBP::system::DeviceDriver::receiveData(std::word amount)
{
	// Receive hardware
	std::size_t received = hardwareDriver.receive(amount);

	// Then write that into software
	for (std::index_t idx = 0; idx < received; idx++)
	{
		if (idx + 1 < received)
			softwareDriver < hardwareDriver.getInput()[idx];
		else
			softwareDriver.triggerInput(hardwareDriver.getInput()[idx]);
	}

	// Then seek
	softwareDriver.seekInputBack(received + 1);

	// Then return amount of bytes received
	return received;
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
