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

// Maximum tabulations per line. Newline upon each new tabulation.
constexpr BBP::std::size_t maxTabulationsPerLine = 8;

BBP::std::errno_t BBP::system::ls_builtin(std::size_t argc, std::c_string *argv)
{
	// Set target string, default is this directory
	std::conststring targetDir = "./";

	// If argv has a second argument, set 'targetdir' to that argument
	if (argc > 1)
		targetDir = argv[1];

	// Then create a new path
	std::PATH pathToDir;

	// Then set that path relative to current working directory
	pathToDir.DeriveFromShellDirectory(targetDir);

	// Only scarcely populate, since 'cd' is not interested in 
	std::DirectoryInfo info;
	std::errno_t error = info.populate(pathToDir, &system::Shell::getActiveMemory());

	// If error was found, print error and abort
	if (error != ENONE)
	{
		// Allocate error string
		std::static_string<32> errorMsg;

		// Copy from
		std::strerror(error, errorMsg);

		// Then print
		std::printf("ls: %s: %s\n", targetDir, errorMsg.data);

		// Now return error
		return error;
	}

	// Working string
	std::string entry;

	// This array stores the length of the running line where an entity may start *after a tabulation*.
	// This is so that entities may be aligned with eachother, without having to go through the directory iterator twice
	std::STATIC_PAGE<std::size_t, maxTabulationsPerLine> tabulationLengths;

	// Now zero initialize it
	for (std::index_t idx = 0; idx < tabulationLengths.dataSize; idx++)
		tabulationLengths[idx] = 0;

	// Current running length of the line
	std::size_t runningLength = 0;

	// Now Print the strings
	for (std::index_t idx = 0; idx < info.entity_count(); idx++)
	{
		// Read string from iterator
		bool couldRead = info.loadEntity(idx, entry);

		// If could not read, then break out of loop
		if (couldRead == false)
			break;
		
		// Then get name length (NOT NULL TERMINATED!)
		std::size_t nameLength = entry.dataSize;


		// If is file, print as file
		if (idx < info.file_count())
		{
			// green highligt
			std::printf("\e[0;92m%s\e[0;38m", entry.data);
		}
		
		// Otherwise, print the name as a directory
		else
		{
			// Green background, blue foreground
			std::printf("\e[0;102;34m%s\e[0;38;48m", entry.data);
		}

		// Now add name length to running length
		runningLength += nameLength;

		// Now check for next tabulation
		for (std::index_t tidx = 0; tidx < tabulationLengths.dataSize; tidx++)
		{
			// Get value
			std::size_t value = tabulationLengths[tidx];

			// If the value is 0, the current position is marked and a tabulation is emitted.
			if (value == 0)
			{
				tabulationLengths[tidx] = runningLength;
				std::printf("\t");
				break;
			}

			// If there has been a tabulation at this length before, just emit a tabulation and break
			if (value == runningLength)
			{
				std::printf("\t");
				break;
			}

			// If the value is greater than running length, move up to that value with spaces, then emit a tab
			if (value > runningLength)
			{
				while(runningLength++ < value)
					std::printf(" ");

				// Then emit tab, and break
				std::printf("\t");
				break;
			}

			// If the value is lower than current running length, this means one of three things.
			// 1) this length has not been reached yet, therefore the next value is 0 (see first if clause);
			// 2) the next length is above or equal to the running length, meaning that the next value will be used;
			// 3) this is the last value there is, in which case a new line should be inserted and running length reset.

		       if (tidx == tabulationLengths.dataSize - 1)
		       {
			       // Reset runningLength and emit newline
			       runningLength = 0;
			       std::printf("\n");
			       break;
		       }	       
		}
	}

	// print newline
	std::printf("\n");

	// Release resources
	info.release();

	return ENONE;
}
