#include "../include/DaemonRegistry.h"
#include "../include/stdio.h"
#include "../include/Shell.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"

// just for show
#include "../include/Executable.h"
#include "../include/CPPApplications.h"

constexpr BBP::std::string_element NL = 0x0a;	// new line
constexpr BBP::std::string_element CR = 0x0d;	// carriage return

constexpr BBP::std::string_element CL = 0x0c;	// clear line

constexpr BBP::std::string_element DEL = 0x7f;	// Delete character


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

	// Get the line length
	std::size_t lineLength = getLine(shellLine);

	// If line has no elements, return
	if (lineLength == 0)
		return 0;

	// Look for first space. (Ignoring delimiters for now)
	// ...

	// Hash command
	std::hash_t cmdHash = std::strhsh(shellLine.static_data);

	std::printf("Hash: 0x%08x\n", cmdHash);

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
	case std::static_hash("run"):
		try {
			BBP::std::execute(smile_main, 0, nullptr);
		}
		catch (...)
		{

		}
		

		return 0;
	}

	//0x39a4887f

	std::c_string argv[1] = { shellLine.static_data };
	shell(1, argv);

	return lineLength;
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