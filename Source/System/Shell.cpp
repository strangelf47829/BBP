#include "../include/DaemonRegistry.h"
#include "../include/stdio.h"
#include "../include/Shell.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/BuiltinShell.h"

BBP::std::errno_t BBP::system::initd::shell(std::size_t argc, std::c_string *argv)
{
	return ENONE;
}

// Is builtin?
bool BBP::system::initd::isBuiltin()
{
	// If no arguments, do nothing
	if (argumentCount == 0)
		return false;

	// Then hash first argument
	std::hash_t argumentOneHash = std::strhsh(argumentVectors[0]);

	// Compare over list of known hashes
	for (std::index_t idx = 0; idx < builtinCommands.dataSize; idx++)
	{
		// Get command
		BuiltinCommand *command = builtinCommands[idx];

		// If command is invalid, skip
		if (command == nullptr)
			continue;

		// If command hash is not the same, exit
		if (command->hash != argumentOneHash)
			continue;

		// If strings are equal, return true
		if (std::strcmp(command->name.data, argumentVectors[0]))
			return true;
	}

	// Default is no.
	return false;
}

// initializre the shell
BBP::std::errno_t BBP::system::initd::shellInit(std::size_t argc, std::c_string *argv)
{
	// Expecting at least one argument (EFI primary volume). If no arguments, return error
	if (argc == 0 || argv == nullptr)
		return EINVAL;

	// Now get c_string from argument list
	std::c_string pathStr = argv[0];

	// If this is nullptr, return error
	if (pathStr == nullptr)
		return ENODATA;

	// Now set
	BBP::system::Shell::getPrimaryVolume().volumePath = pathStr;

	// Return no error
	return ENONE;

}

// Execute builtin action
BBP::std::errno_t BBP::system::initd::shellBuiltin(std::size_t argc, std::c_string *argv)
{
	// If no arguments, do nothing
	if (argumentCount == 0)
		return -1;

	// Then hash first argument
	std::hash_t argumentOneHash = std::strhsh(argumentVectors[0]);

	// Compare over list of known hashes
	for (std::index_t idx = 0; idx < builtinCommands.dataSize; idx++)
	{
		// Get command
		BuiltinCommand *command = builtinCommands[idx];

		// If command is invalid, skip
		if (command == nullptr)
			continue;

		// If command hash is not the same, exit
		if (command->hash != argumentOneHash)
			continue;

		// If strings are equal, return function outcome
		if (std::strcmp(command->name.data, argumentVectors[0]))
			return command->entryPoint(argc, argv);
	}

	// Error.
	return ENODATA;
}

/*
constexpr BBP::std::string_element NL = 0x0a;	// new line
constexpr BBP::std::string_element CR = 0x0d;	// carriage return

constexpr BBP::std::string_element CL = 0x0c;	// clear line

constexpr BBP::std::string_element DEL = 0x7f;	// Delete character

// empty stuff
void emptyIRQFunctor(BBP::std::size_t argc, BBP::std::interrupt_arg_t *argv)
{
	BBP::std::printf("IRQ called.\n");
}

BBP::system::appInfo shellInformation;

BBP::system::appInfo BBP::system::getShellInformation()
{
	return shellInformation;
}

BBP::std::size_t BBP::system::initd::getLine(BBP::std::static_string<shellLineLength> &str)
{
	// Flags for capturing input
	bool captureInput = true;
	std::size_t lineLength = 0;

	// Initialize shell to 0
	for (std::index_t idx = 0; idx < str.dataSize; idx++)
		str.static_data[idx] = 0;

	while (captureInput)
	{
		// Get a single character
		std::string_element character = std::getChar();

		// Determine special character
		switch (character)
		{
		case NL: // If character is new line,
		case CR: // Or carriage return,
			goto endloop;

		case CL: // Clear line
			getKernelInstance().getScreenDriver().hardwareDriver.executeCommand(screenClearScreen, 0, nullptr);

			// Reprint
			printShellDirectory();
			std::printf("%s", str.static_data);

			goto continueLoop;

		case DEL: // Delete a character
			if (lineLength)
			{
				std::printf("%c %c", 8, 8);
				lineLength--;
				str.static_data[lineLength] = 0;
			}
			goto continueLoop;

		default:
			goto keepGoing;
		}

	endloop:
		captureInput = false;
	continueLoop:
		continue;
	keepGoing:

		// Check if character is a valid character
		if (!(character >= 0x20 && character <= 0x7E))
			continue;

		// If can stil write
		if (lineLength >= shellLineLength)
			continue;

		// Update term
		str.static_data[lineLength++] = character;
		std::printf("%c", character);

	}

	// Print newline at end of argument
	std::printf("\n");

	return lineLength;
}

BBP::std::size_t BBP::system::initd::getShellLine()
{
	// Get line
	printShellDirectory();

	BBP::std::c_string a = "makeirq";
	BBP::std::c_string b = "irq";

	// Print shell stuff
	shell(1, &a);
	shell(1, &b);

	return 0;

	// Get the line length
	std::size_t lineLength = getLine(shellLine);

	// If line has no elements, return
	if (lineLength == 0)
		return 0;

	std::c_string argv[1] = { shellLine.static_data };
	shell(1, argv);

	return maxDaemonNameLength + 1;
}


BBP::std::errno_t BBP::system::initd::shell(std::size_t argc, std::c_string *argv)
{
	// If no arguments are supplied, just enter shell mode.
	if (argc == 0)
	{
		// Print shell information on greeting
		printShellInformation();

		// Get a line for a shell
		while (getShellLine() != maxDaemonNameLength + 1);

		return 0;
	}

	// Otherwise, execute shell commands. argv[0] is always a command, followed by arguments.
	std::printf("Executing command: %s\n", argv[0]);

	// Hash command
	std::hash_t cmdHash = std::strhsh(shellLine.static_data);

	// Check for simple builtin commands
	switch (cmdHash)
	{
	case std::static_hash("logout"):
		return maxDaemonNameLength + 1;
	case std::static_hash("?"):
		printShellInformation();
		return 0;
	case std::static_hash("emma"):
		std::printf("Hewwo!\n");
		return 0;
	case std::static_hash("irq"):
		getKernelInstance().IRQHandler.updateHandler();
		return 0;
	case std::static_hash("makeirq"):

		// create IRQ thingy
		BBP::std::IRQ irq;
		BBP::std::createInterrupt(irq, emptyIRQFunctor);
		irq.removeFromQueue = false;

		getKernelInstance().IRQHandler.handle(irq);

		return 0;
	case std::static_hash("run"):
		try {
			BBP::std::execute(smile_main, 0, nullptr);
		}
		catch (...)
		{

		}
		return 0;
	default:
		std::printf("Unkown command '%s' (0x%08x)\n", shellLine.static_data, cmdHash);
		return 0;
	}

	return 0;
}

void BBP::system::printShellDirectory()
{
	// Get active user
	procFrame *ownFrame = getKernelInstance().SubSystems().activeContext->activeFrame;

	// Also get some paths
	std::PATH fullpath = ownFrame->processContext.activeDirectory;
	fullpath = fullpath.makeAbsolutePath();

	// Print username
	std::printf("\e[0;92m%s@", ownFrame->identifier.owner->username.data);
	std::printf("%s\e[0;37m:", ownFrame->identifier.owner->connectionMethod.data);


	std::printf("\e[1;34m%s\e[0;37m$ ", fullpath.relName());



}
void BBP::system::printShellInformation()
{
	// Get license information
	appInfo info = getShellInformation();

	// Print build information
	std::printf("BBP shell, version %u.%u.%u(%u)", info.build.majorVersion, info.build.minorVersion, info.build.patchVersion, info.build.buildVersion);

	if (info.build.extraInfo)
		std::printf(" %s\n", info.build.extraInfo);
	else
		std::printf("\n");

	// Print copyright information
	std::printf("Copyright (C) %u %s\n", info.copyright.copyrightYear, info.copyright.copyrightHolder);

	// Print license information
	std::printf("License %s: %s", info.license.licenseName, info.license.licenseInfo);

	if (info.license.licenseURL)
		std::printf(" <%s>\n\n", info.license.licenseURL);
	else
		std::printf("\n\n");
}
*/
