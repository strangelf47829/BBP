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
#include "../include/Syscalls.h"
#include "../include/Async.h"

/*
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

	// Get Inspect a path
	std::PATH inspection = "/mnt/v/home";

	// Inspect a path
	std::Inspect(inspection);

	// String
	std::static_string<255> str;

	// While we can step the iterator
	while (std::canStepInspector())
	{
		// Get type
		std::FileSysInfo::FileSysEntryType type = std::getInspectorFileType();

		// Get name
		std::getInspectorPath(str);

		// Get filetype name
		std::conststring typeStr = "Unkown";

		switch (type)
		{
		case std::FileSysInfo::FileSysEntryType::File:
			typeStr = "File";
			break;
		case std::FileSysInfo::FileSysEntryType::Directory:
			typeStr = "Directory";
			break;
		case std::FileSysInfo::FileSysEntryType::Unkown:
			typeStr = "Unkown";
			break;
		}
		// print stuff
		std::printf(" - %s is of type %s.\n", str.data, typeStr);

		// Step at the end
		std::stepInspectionIterator();
	}

}*/


BBP::std::errno_t BBP::system::cp_builtin(std::size_t argc, std::c_string *argv)
{
	std::async_task<int (*)(int), int>::lambda_t k = [](std::async_stack_t<int> &stack, std::async_stack_t<int> &arg) -> int { /*Double*/ stack.template get<0>() = 2 * arg.template get<0>(); return 0; };
	std::async_task<int (*)(int), int>::lambda_t l = [](std::async_stack_t<int> &stack, std::async_stack_t<int> &arg) -> int { /*Return double again*/ return stack.template get<0>() * 2; };

	std::async_task<int (*)(int), int>::lambda_t list[2] = { k, l };

	// Collect everything
	std::async_task<int (*)(int), int> task1(2, list);
	std::async_task<int (*)(int), int> task2(2, list);

	std::word arg1 = 13;
	std::word arg2 = 12;

	// Call asynchronously
	task1.Async(arg1);
	task2.Async(arg2);
	task1.Step();
	task2.Step();
	int result1 = task1.Await();
	int result2 = task2.Await();

	std::printf("Inputs were (%u,%u) and outputs were (%u,%u)\n", arg1,arg2,result1,result2);


	void *result_dub = systemcalls::malloc(3);
	systemcalls::free(result_dub);

	/*
	std::PATH dirPath = "/";
	std::ResourceManager alloc;

	std::DirectoryInfo info;
	info.populate(dirPath, &alloc);

	std::size_t entityCount = info.entity_count();

	for (std::index_t idx = 0; idx < entityCount; idx++)
	{
		std::string entityName; 
		info.loadEntity(idx, entityName);

		std::printf(" - %s\n", entityName.data);
	}

	info.release();*/

	

	return 0;
}