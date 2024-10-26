#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"
#include "../include/errnoStrings.h"

BBP::std::errno_t BBP::system::cat_builtin(std::size_t argc, std::c_string *argv)
{
	// Requires at least one argument
	if (argc < 2 || argv == nullptr)
	{
		std::printf("cat: no filename supplied.\n");
		return EINVAL;
	}

	// Create path
	std::PATH pathToFile;

	// Then make it relative to working directory
	pathToFile.DeriveFromShellDirectory(argv[1]);

	// Get exception Level
	std::byte exceptionLevel = std::getExceptionLogLevel();

	// Try to open file
	try
	{
		// Then set loglevel to 0
		std::setExceptionLogLevel(0);

		// Open file
		std::FILE file(pathToFile);

		// Get file size
		std::size_t fileSize = file.b().max_elements;

		// Then print out the whole file
		for (std::size_t idx = 0; idx < fileSize; idx++)
		{
			char c = file.b()[idx];
			std::printf("%c", c);
		}

		file.close();
	}
	// Catch exception
	catch (std::except e)
	{
		// Create string for exception
		std::static_string<50> errormsg;

		// Then write error message
		std::strerror(e.code, errormsg);

		// Then print message
		std::printf("cat: %s: %s\n", argv[1], errormsg.data);
	}

	// Then set exception log level back to original state
	std::setExceptionLogLevel(exceptionLevel);

	return ENONE;
}
