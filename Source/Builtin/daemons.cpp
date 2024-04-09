#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"

BBP::std::errno_t BBP::system::daemon_builtin(std::size_t argc, std::c_string *argv)
{
	// If argument count is 1, print daemon help util
	if (argc < 2)
	{
		std::printf("Daemon: Builtin command for manipulating daemons.\n\n\tThis is where the help would go.\n");
		return 0;
	}

	std::printf("Daemon: handling command '%s'\n", argv[1]);

	return 0;
}

