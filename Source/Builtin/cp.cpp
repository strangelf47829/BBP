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

	info.release();

	return 0;
}