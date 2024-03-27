#include "../include/Kernel.h"


BBP::std::PAGE<BBP::std::string_element> &BBP::system::HardwareHandle::getInput()
{
	return inputBuffer;
}

BBP::std::PAGE<BBP::std::string_element> &BBP::system::HardwareHandle::getOutput()
{
	return outputBuffer;
}

BBP::std::size_t BBP::system::HardwareHandle::send(std::size_t amount)
{
	// If senddata is nullptr, do nothing
	if (sendData == nullptr)
		return 0;

	return sendData(amount, outputBuffer);
}

BBP::std::size_t BBP::system::HardwareHandle::receive(std::size_t amount)
{
	// If receiveData is nullptr, do nothing
	if (receiveData == nullptr)
		return 0;

	return receiveData(amount, inputBuffer);
}

// Hardware handle constructor
BBP::system::HardwareHandle::HardwareHandle(HardwareAction actions[3], std::size_t cmdc, const HardwareCmd *cmdv)
{
	// Set own actions
	sendData = actions[0];
	receiveData = actions[1];
	readMetadata = actions[2];

	// If cmdv is nullptr, force cmdc to 0
	if (cmdv == nullptr)
		cmdc = 0;

	// If cmdc is over command count, clamp it.
	if (cmdc >= commands.dataSize)
		cmdc = commands.dataSize;

	// Iterate over cmdc
	for (std::index_t cmdIndex = 0; cmdIndex < cmdc; cmdIndex++)
		commands.data[cmdIndex] = cmdv[cmdIndex];
}

// Set own handles.
void BBP::system::HardwareHandle::setHandleData(HardwareAction actions[3], std::size_t cmdCount, const HardwareCmd *cmdVector)
{
	// Set own data
	if (actions)
	{
		sendData = actions[0];
		receiveData = actions[1];
		readMetadata = actions[2];
	}

	// Copy commands if it exists
	if (cmdCount && cmdVector)
	{
		for (std::index_t idx = 0; idx < cmdCount; idx++)
			commands[idx] = cmdVector[idx];
	}

}

// Execute a command. Returns false on failure, true on success.
bool BBP::system::HardwareHandle::executeCommand(std::index_t commandIndex, std::size_t argc, std::word *argv)
{
	// Check if command index is within bounds
	if (commandIndex >= commands.dataSize)
		return false;

	// Retrieve command
	HardwareCmd command = commands[commandIndex];

	// If command is nullptr, return false
	if (command == nullptr)
		return false;

	// If argv is nullptr, force argc to 0
	if (argv == nullptr)
		argc = 0;

	// Execute command
	return command(argc, argv);
}