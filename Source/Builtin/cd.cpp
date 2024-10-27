#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"
#include "../include/FileSysInfo.h"
#include "../include/errnoStrings.h"

// CD: change directory.
// If none is specified, go to home.

BBP::std::conststring defaultPath = "/home/";

BBP::std::errno_t BBP::system::cd_builtin(std::size_t argc, std::c_string *argv)
{
	// Default directory
	std::conststring targetDir = "/home/";

	// If argv has a second parameter, set 'targetdir' to that parameter
	if (argc > 1)
		targetDir = argv[1];	

	// Create new path
	std::PATH newPath;

	// Then set that path relative to working directory
	newPath.DeriveFromShellDirectory(targetDir);

	// Only scarcely populate, since 'cd' is not interested in exact file information, etc... 
	std::DirectoryInfo info;
	std::errno_t success = info.scarce_populate(newPath);

	// If there was an error, print error and abort
	if (success != ENONE)
	{
		// Allocate static string for error message
		std::static_string<32> errnoMsg;

		// Then get associated string
		std::strerror(success, errnoMsg);

		// Then print error
		std::printf("cd: %s: %s", targetDir, errnoMsg.data);

		// Then return errno
		return success;
	}

	// Create directory
	std::PATH dirPath = newPath.makeDirectory();

	// Success, copy over data
	system::Shell::getWorkingDirectory().copyFrom(newPath.makeDirectory());

	// Success
	return ENONE;
}
