#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"
#include "../include/FileSysInfo.h"


BBP::std::errno_t BBP::system::ls_builtin(std::size_t argc, std::c_string *argv)
{
	// Get kernel
	Kernel *k = &getKernelInstance();

	// Get subsystem
	KernelSubSystems *kernelSS = &k->SubSystems();

	// Get active procframe
	procFrame *ownFrame = kernelSS->activeContext->activeFrame;

	// Get string to cd into.
	std::c_string defaultPathTo = ownFrame->systemContext->workingDirectory->relName();
	std::string pathTo(std::strlen(defaultPathTo), defaultPathTo);

	// If argument exists
	if (argc >= 2)
	{
		// If argument is '.', set path to './'
		if (std::strcmp(argv[1], "."))
			pathTo = std::String("./");

		// If argument is '.,', set path to '../'
		else if (std::strcmp(argv[1], ".."))
			pathTo = std::String("../");

		// Otherwise, set path to argv[1]
		else
			pathTo = std::String(argv[1]);
	}


	// Get home directory
	std::PATH currentPath;
	std::PATH changeTo(pathTo);

	// Copy data from old path into current path
	currentPath.copyFrom(*(ownFrame->systemContext->workingDirectory));

	// Combine them
	currentPath = changeTo.makeAbsolutePath(&currentPath);

	// Now get path
	std::c_string pathName = currentPath.relName();

	// Check if changeto is now defined as root or not. If it is, move forward.
	if (changeTo.isDefinedFromRoot())
		pathName += std::strlen(ownFrame->systemContext->contextVolume.volumePath.relName()) - 1;

	// Create new path
	std::PATH newPath(pathName);

	// Only scarcely populate, since 'cd' is not interested in 
	std::DirectoryInfo info;
	bool success = info.populate(newPath, ownFrame->systemContext->activemem);

	// Working string
	std::string entry;

	// The max amount of characters per line
	std::word charsPerLine = 40;
	std::word currentCharCount = 0;

	// Print stuff
	for (std::index_t idx = 0; idx < info.entity_count(); idx++)
	{
		// Load file
		bool success = info.loadEntity(idx, entry);

		// If not successful, exit
		if (success == false)
			continue;

		// Get size of string
		std::size_t nameLength = entry.dataSize;

		// If currentCharCount + nameLength is greater than charsPerLine, new line and reset
		if (nameLength + currentCharCount > charsPerLine)
		{
			std::printf("\n");
			currentCharCount = 0;
		}

		// Print (highlighted TODO: Use something other than strings)
		if (idx < info.file_count())
		{
			// green highligt
			std::printf("\e[0;92m%s\e[0;38m", entry.data);
		}
		else
		{
			// Green background, blue foreground
			std::printf("\e[0;102;34m%s\e[0;38;48m", entry.data);
		}

		// print spaces and increase count
		std::printf("    ");
		currentCharCount += 4 + nameLength;
	}

	// print newline
	std::printf("\n");

	// Release resources
	info.release();

	return 0;
}