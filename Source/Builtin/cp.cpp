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
#include "../include/FileSysInfo.h"
#include "../include/stddrv.h"

void check(BBP::std::string str)
{
	// Create path
	BBP::std::PATH path = str;

	// Check 
	bool exists = BBP::std::isFileOnDisk(path);

	// Print diagnostic
	if (exists == false)
	{
		BBP::std::printf("File %s does not exist.\n", str.data);
		return;
	}

	// Get file size
	BBP::std::size_t fileSize = BBP::std::getFilesizeFromDisk(path);

	// Get file type
	bool isFile = BBP::std::isPathOfTypeFile(path);
	bool isDir = BBP::std::isPathOfTypeDirectory(path);

	// Print
	BBP::std::printf("File %s is of type", str.data);
	
	if (isFile)
		BBP::std::printf(" file ");
	else if (isDir)
		BBP::std::printf(" directory ");
	else
		BBP::std::printf(" unkown ");

	BBP::std::printf("and occupies %u bytes.\n", fileSize);
}

BBP::std::errno_t BBP::system::cp_builtin(std::size_t argc, std::c_string *argv)
{
	check("/mnt/v/home/a.out");
	check("/mnt/v/home/gofuckyourself");
	check("/mnt/v/home/");
	check("/mnt/v/home/test.txt");

	std::STATIC_PAGE<std::string_element, 100> pa;
	std::Stack<std::string_element> str(&pa);

	std::PATH p("/mnt/v/home/test.txt");
	std::readFileFromDisk(str, p);

	std::printf("File contains:\n%s\n\n", pa.data);

	std::PATH p2("/mnt/v/home/test2.txt");
	std::readFileFromDisk(str, p2);

	std::printf("File contains:\n%s\n\n", pa.data);
}