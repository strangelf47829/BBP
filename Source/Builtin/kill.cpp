#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"

BBP::std::errno_t BBP::system::kill_builtin(std::size_t argc, std::c_string *argv)
{

	std::string outputFile = "/home/kill.out";
	std::string inputFile = "/home/testtest.esa";

	std::PATH output(outputFile);
	std::PATH input(inputFile);

	// Get home directory
	std::PATH currentPath;

	// Copy data from old path into current path
	currentPath.copyFrom(system::Shell::getWorkingDirectory());

	// Make paths absolute
	std::PATH::makeRelative(currentPath, input);
	std::PATH::makeRelative(currentPath, output);

	// Print stuff
	//std::printf("Compiling %s -> %s\n", input.relName(), output.relName());

	char c = '\033';

	// Process one file
	esa::processor.translate(input, output);

	return 0;
}