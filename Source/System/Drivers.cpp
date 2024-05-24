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
	return writeData(data, std::strlen(data));
}

BBP::std::word BBP::system::DeviceDriver::writeData(std::string data, std::size_t amount)
{
	// Define the amount of bytes sent
	std::size_t bytesSent = 0;

	// Calculate the size of a block
	std::size_t blockSize = std::seqlen(hardwareDriver.getOutput());

	// While this amount is less than the amount of requested bytes...
	while (bytesSent < amount)
	{
		// Calculate the amount of bytes that need to be sent
		std::size_t bytesToSend = amount - bytesSent;

		// If this amount is larger than a block size, limit
		if (bytesToSend > blockSize)
			bytesToSend = blockSize;

		// Write into software buffer
		for (std::index_t idx = 0; idx < bytesToSend; idx++)
			softwareDriver << data[idx + bytesSent];

		// Then send amount of data out
		std::size_t blockBytesSent = hardwareDriver.send(bytesToSend);

		// Then Seek back the amount of data sent
		softwareDriver.seekOutputBack(blockBytesSent + 1);

		// Add up bytes sent
		bytesSent += blockBytesSent;

		// Then check if we sent the expected amount of bytes. If we did not, break out
		if (blockBytesSent != bytesToSend)
			break;
	}

	// Then return amount of bytes sent
	return bytesSent;
}

// Read data
BBP::std::word BBP::system::DeviceDriver::receiveData(std::word amount)
{
	// Define the amount of bytes received
	std::size_t bytesReceived = 0;

	// Calculate the size of a block
	std::size_t blockSize = std::seqlen(hardwareDriver.getInput());

	// While this amount is less than the amount of requested bytes...
	while (bytesReceived < amount)
	{
		// Calculate the amount of bytes that need to be received
		std::size_t bytesToReceive = amount - bytesReceived;

		// If this amount is larger than a block size, limit
		if (bytesToReceive > blockSize)
			bytesToReceive = blockSize;

		// Then receive amount of data
		std::size_t blockBytesReceived = hardwareDriver.receive(bytesToReceive);

		// Then write into software buffer
		for (std::index_t idx = 0; idx < bytesToReceive; idx++)
			softwareDriver < hardwareDriver.getInput()[idx];

		// Add up bytes received
		bytesReceived += blockBytesReceived;

		// Then check if we receive the expected amount of bytes. If we did not, break out
		if (blockBytesReceived != bytesToReceive)
			break;
	}

	// Clear out hardware driver
	for (std::index_t idx = 0; idx < blockSize; idx++)
		hardwareDriver.getInput()[idx] = 0;

	// Then return amount of bytes sent
	return bytesReceived;
}