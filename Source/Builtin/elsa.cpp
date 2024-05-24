#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"

// ELSA builtin command
/*

Elsa blah blah blah

Arguments:
-g		: emit debug symbols
-c		: skip linking
-W,<n>	: Linker argument
-o		: Set output file
-I		: Add include path


*/

BBP::std::errno_t BBP::system::elsa_builtin(std::size_t argc, std::c_string *argv)
{
	std::string outputFile = std::String("a.out");

	// Create static page for strings
	std::STATIC_PAGE<std::string, 32> inputFiles;

	// Input file count
	std::size_t inputFileCount = 0;

	// Flags
	bool enableDebug = false;
	bool enableLinking = true;

	// Foreach arg,
	for (std::index_t idx = 1; idx < argc; idx++)
	{
		//std::printf("arg %u: %s\n", idx, argv[idx]);

		// If empty argument, do nothing
		if (argv[idx] == nullptr)
			continue;

		if (argv[idx][0] == 0)
			continue;

		// If argument does not start with '-', add it to input files.
		if (argv[idx][0] != '-')
		{
			inputFiles[inputFileCount++] = std::string(std::strlen(argv[idx]), argv[idx]);
			continue;
		}

		// Get character of argument
		std::string_element argCharacter = argv[idx][1];

		// Do thing based on following:
		switch (argCharacter)
		{
			// Empty argument
		case 0:
			std::printf("Skipping empty argument '-'.\n");
			break;
		case 'g':
			enableDebug = true;
			break;
		case 'c':
			enableLinking = false;
			break;
		case 'r':
			esa::processor = esa::esaProcessor();
			break;
		case 'o':
			// Check if argument exists after this
			if (argc <= idx + 1)
			{
				std::printf("Filename must follow -o argument.\n");
				break;
			}

			// Otherwise set name
			outputFile = std::string(std::strlen(argv[idx + 1]), argv[idx + 1]);
			idx++;
			break;

		default:
			std::printf("Unkown argument '-%c'.\n", argCharacter);
			break;

		}


	}

	// If no input files are supplied, throw error
	if (inputFileCount == 0)
	{
		std::printf("Elsa: fatal error: No input files.\n");
		return 1;
	}

	//std::printf("Compiling stuff...\n");

	std::PATH output(outputFile);
	std::PATH input(inputFiles[0]);

	// Get home directory
	std::PATH currentPath;

	// Copy data from old path into current path
	currentPath.copyFrom(system::Shell::getWorkingDirectory());

	// Make paths absolute
	std::PATH::makeRelative(currentPath, input);
	std::PATH::makeRelative(currentPath, output);

	// Print stuff
	//std::printf("Compiling %s -> %s\n", input.relName(), output.relName());

	// Process one file
	esa::processor.translate(input, output);

}