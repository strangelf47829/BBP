#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"

// Execute shell command
BBP::std::errno_t BBP::system::initd::shell(std::string &line)
{
	// Copy data from line into shellLine
	std::strcpy(&shellLine, line.data);

	// Run shell
	return shell();
}

// Execute shell command (through shellLine)
BBP::std::errno_t BBP::system::initd::shell()
{
	// Get line length
	std::size_t lineLength = std::strlen(shellLine);

	// Delimit input
	delimitInput(lineLength);

	// Check if builtin
	bool isCmdBuiltin = isBuiltin();

	if (isCmdBuiltin)
		shellBuiltin(argumentCount, argumentVectors.static_data);
	else
		std::printf("%s: command not found.\n", argumentVectors[0]);
}

// Spawn application
BBP::std::errno_t BBP::system::initd::shellApplication(std::size_t argc, std::c_string *argv)
{

	// Print information
	printShellInformation();

	// Try to do a shell immediately
	//std::string shellCMD0 = std::String("cd /boot/esa");
	//shell(shellCMD0);

	std::string shellCMD1 = std::String("cp");
	shell(shellCMD1);

	//std::string shellCMD2 = std::String("rae");
	//shell(shellCMD2);

	std::string shellCMD3 = std::String("logout");
	shell(shellCMD3);

	// Get a shell Line
	while (1)
		shellApplicationGetLine();

}

void BBP::system::initd::delimitInput(std::size_t lineLength)
{
	// Create stack of arguments
	std::Stack<std::c_string> argStack(&argumentVectors, argumentVectors.dataSize);

	// If last character is space
	bool isLastCharacterDelimiter = false;

	// Last known string thing
	std::c_string lastKnownCharacter = shellLine.static_data;

	// reset argument count
	argumentCount = 0;

	// Split lines
	for (std::index_t idx = 0; idx < lineLength; idx++)
	{
		// Get current character
		std::string_element character = shellLine[idx];

		// If idx is space and isLastCharacterDelimiter, do nothing
		if (character == ' ' && isLastCharacterDelimiter)
			continue;

		// If character is space, but last character is not delimiting character, push lastKnownStuff
		if (character == ' ' && !isLastCharacterDelimiter)
		{
			// Push value
			argStack << lastKnownCharacter;
			isLastCharacterDelimiter = true;

			// Update value
			argumentCount++;

			// Write 0 to shellLine stuff
			shellLine[idx] = 0;

			// Update
			isLastCharacterDelimiter = true;
			continue;
		}

		// If character is not space, but is last character delimiter, update lastKnown stuff
		if (character != ' ' && isLastCharacterDelimiter)
		{
			lastKnownCharacter = &shellLine[idx];

			// Update
			isLastCharacterDelimiter = false;
		}

		// Update
		isLastCharacterDelimiter = (character == ' ');
	}

	// If last character is not a delimiter, push argument
	if (isLastCharacterDelimiter == false)
	{
		argStack << lastKnownCharacter;

		argumentCount++;
	}
}

// Get a line
void BBP::system::initd::shellApplicationGetLine()
{
	// Print directory
	printShellDirectory();

	// Get line
	std::size_t lineLength = loadUserInput();

	// If line length is 0, do nothing
	if (lineLength == 0)
		return;

	// Now do shell
	shell(shellLine);
}

// Load a line from the user
BBP::std::size_t BBP::system::initd::loadUserInput()
{
	// Keep track of line length
	std::size_t lineLength = 0;

	// Should keep reading user input
	bool readInput = true;

	// Clear buffer
	for (std::index_t idx = 0; idx < shellLine.dataSize; idx++)
		shellLine.static_data[idx] = 0;

	// Read input
	while (readInput)
	{
		// Get a character
		std::string_element character = std::getChar();

		// If character is not within C0 or C1, do normal thing
		bool isInC0 = (character >= BBP::std::Terminal::TerminalApplication::C0_Low && character <= BBP::std::Terminal::TerminalApplication::C0_High);
		bool isInC1 = (character >= BBP::std::Terminal::TerminalApplication::C1_Low && character <= BBP::std::Terminal::TerminalApplication::C1_High);
		bool isDelete = (character == 0x7F);

		// If in neither, add character normally
		if ((isInC0 || isInC1 || isDelete) == false && lineLength < shellLine.dataSize - 1)
		{
			// Write character
			shellLine[lineLength++] = character;

			// Null terminate
			shellLine[lineLength] = 0;

			// Write character
			//getKernelInstance().getScreenDriver().writeData(strChar);
			std::printf("%c", character);

			// Continue
			continue;
		}

		// If within C1, do nothing
		if (isInC1)
			continue;

		// Get C0 action and do action
		BBP::std::Terminal::TerminalApplication::C0 c0Action = (BBP::std::Terminal::TerminalApplication::C0)character;

		// If is delete, and can delete
		if (isDelete && lineLength)
		{
			// Move null into current space, then decrement lineLength
			shellLine[--lineLength] = 0;

			// Output a space
			std::printf("%c %c", 8, 8);

			// Continue loop
			continue;
		}

		// Switch case
		switch (c0Action)
		{
			// Newline
		case BBP::std::Terminal::TerminalApplication::C0::LF:
			// Stop reading stuff
			readInput = false;
			std::printf("\n");
			break;

			// Clear screen
		case BBP::std::Terminal::TerminalApplication::C0::FF:

			// Clear screen command
			getKernelInstance().getScreenDriver().hardwareDriver.executeCommand(screenClearScreen, 0, nullptr);

			// Reprint directory
			printShellDirectory();

			// Only print if something is present.
			if (lineLength)
				std::printf("%s", shellLine.static_data);

			break;
		}

	}

	// Null terminate
	shellLine[lineLength] = 0;

	// Return that
	return lineLength;
}