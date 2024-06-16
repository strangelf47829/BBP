#include "../include/EFIShell.h"

// String to store user input
BBP::std::static_string<32> cmd;
BBP::std::size_t commandLength;

bool Host::enterShell(Host::BIOS &bios)
{
	// Clear screen
	bios.clearScreen();

	// Print something
	bios.print("Entering shell...\n");

	// Read one line
	while (BBP::std::strcmp(&cmd, "exit") == false)
		readLine(bios);

	return false;
}

void Host::readLine(Host::BIOS &bios)
{
	// Declare bool
	bool readInput = true;

	// Set commandLength to 0
	commandLength = 0;

	// Clear input
	for (BBP::std::index_t idx = 0; idx < cmd.dataSize; idx++)
		cmd[idx] = 0;

	// Print ']'
	bios.printCharacter(']');

	// While reading input:
	while (readInput)
	{
		// Declare character
		BBP::std::string_element inputChar = 0;

		// If no input, continue
		if (bios.getKeyboardInput(inputChar) == false)
			continue;

		// Switch inputChar
		switch (inputChar)
		{

		case 0x0a: // 'Enter'
			// Stop reading input
			readInput = false;
			bios.printCharacter('\n');
			break;

		case '\b':
		case 0x7F: // Delete
			// If no data is stored, do nothing
			if (commandLength == 0)
				break;

			// Otherwise set null character and move back one. Also move back and print space
			cmd[commandLength--] = 0;
			bios.print("\b \b");
			break;

		default:
			// Check if buffer is full
			if (commandLength + 1 >= cmd.dataSize)
				break;

			// Otherwise add command and null character
			cmd[commandLength++] = inputChar;
			cmd[commandLength] = 0;

			// Also print character
			bios.printCharacter(inputChar);
			break;

		}
	}
}