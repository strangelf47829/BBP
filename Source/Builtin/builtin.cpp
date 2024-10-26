#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"

// The builtin command: executes a builtin function.
// TODO: implement shell

BBP::std::errno_t BBP::system::builtin_builtin(std::size_t argc, std::c_string *argv)
{
	return 0;
}
