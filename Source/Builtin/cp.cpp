#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"
#include "../include/ELSA/Symbol.h"
#include "../include/Hashmap.h"
#include "../include/Concepts.h"
#include "../include/Dictionary.hxx"
#include "../include/ELSA/Numerical.h"
#include "../include/SequentialMemory.h"
#include "../include/ELSA/SectionDB.h"
#include "../include/ELSA/Keywords.h"
#include "../include/ELSA/Processor.h"
#include "../include/ELSA/Lexer.h"
#include "../include/ELSA/DebugUtils.h"
#include "../include/stddrv.h"

void check(BBP::std::string str)
{
	// Create path
	BBP::std::PATH path = str;

	// Check 
	bool exists = BBP::std::isFileOnDisk(path);

	// Print diagnostic
	if (exists)
		BBP::std::printf("File %s does exist.\n", str.data);
	else
		BBP::std::printf("File %s does not exist.\n", str.data);
}

BBP::std::errno_t BBP::system::cp_builtin(std::size_t argc, std::c_string *argv)
{
	check("/mnt/v/home/a.out");
	check("/mnt/v/home/gofuckyourself");
}