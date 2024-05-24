#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"
#include "../include/FileSysInfo.h"

// CD: change directory.
// If none is specified, go to home.

BBP::std::conststring defaultPath = "/home/";

BBP::std::errno_t BBP::system::cd_builtin(std::size_t argc, std::c_string *argv)
{
	// Get string to cd into.
	std::string pathTo = std::String(defaultPath);

	// If argument exists
	if (argc >= 2)
	{
		// If argument is '.', set path to './'
		if (std::strcmp(argv[1], (std::c_string)"."))
			pathTo = std::String("./");

		// If argument is '.,', set path to '../'
		else if (std::strcmp(argv[1], (std::c_string)".."))
			pathTo = std::String("../");

		// Otherwise, set path to argv[1]
		else
			pathTo = std::String(argv[1]);
	}


	// Get home directory
	std::PATH currentPath;
	std::PATH changeTo(pathTo);

	// Copy data from old path into current path
	currentPath.copyFrom(system::Shell::getWorkingDirectory());

	// Combine them
	currentPath = changeTo.makeAbsolutePath(&currentPath);

	// Now get path
	std::c_string pathName = currentPath.relName();

	// Check if changeto is now defined as root or not. If it is, move forward.
	if (changeTo.isDefinedFromRoot())
		pathName += std::strlen(system::Shell::getPrimaryVolume().volumePath.relName()) - 1;

	// Create new path
	std::PATH newPath(pathName);

	// Only scarcely populate, since 'cd' is not interested in 
	std::DirectoryInfo info;
	bool success = info.scarce_populate(newPath);

	// Copy over stuff from that path, if success
	if (success)
		system::Shell::getWorkingDirectory().copyFrom(newPath.makeDirectory());
	else
	{
		// Handle thing is not directory.
		if (info.is_directory() == false)
		{
			std::printf("cd: %s: Not a directory\n", changeTo.relName());
			return 1;
		}

		// Handle thing does not exist.
		if (info.path_exists() == false)
		{
			std::printf("cd: %s: No such file or directory\n", changeTo.relName());
			return 2;
		}

		// General error
		std::printf("cd: %s: Filesystem error\n", changeTo.relName());
		return 3;
	}

	// Success
	return 0;
}