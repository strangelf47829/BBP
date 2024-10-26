#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"

BBP::std::errno_t BBP::system::logout_builtin(std::size_t argc, std::c_string *argv)
{
	// Unwind the kernel
	BBP::std::kernel_unwind();

	// Shouldn't be possible to access this place, throw error
	throw 0;
}

