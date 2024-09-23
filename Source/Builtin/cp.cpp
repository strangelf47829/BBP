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
#include "../include/Syscalls.h"
#include "../include/Async.h"
#include "../include/Tasks.h"
#include "../include/TaskPool.h"
#include "../include/ELSA/TranslationUnit.h"
#include "../include/ELSA/ELF.h"

void hihi(BBP::elsa::TranslationUnit &, BBP::std::index_t, BBP::elsa::keyword_t &)
{
	BBP::std::printf("Hello!");
}

BBP::std::string str = "eee Hello! eee";

BBP::std::errno_t BBP::system::cp_builtin(std::size_t argc, std::c_string *argv)
{

	// Create a binary application
	elsa::BinaryApplication application;

	// Then save file
	application.emitFile("/home/new2.out");

	sizeof(elsa::TranslationUnit);

	/*
	// Static page
	std::STATIC_PAGE<elsa::Section *, 16> sections_static;

	// Create ELF file
	ELF::ELF elf = &sections_static;

	// Just create something
	elsa::Section textSection;

	// Data
	ELF::Section textSectionData;

	// Then register
	elf.registerSection(textSection, textSectionData, ".text");

	// Create data
	std::PAGE<std::byte> textData;
	textSection.Allocate(textData, 3);
	textData[0] = 'H';
	textData[1] = 'e';
	textData[2] = 'l';

	// Now find
	BBP::elsa::Section *sect = elf.get(".text");

	// Now reallocate
	sect->Allocate(textData, 4);
	textData[0] = 'l';
	textData[1] = 'o';
	textData[2] = '!';
	textData[3] = 0;

	// Then add stuff to the text
	textSectionData.type = 1;
	textSectionData.writeData(elf.header.ident.littleEndian);

	// Save
	elf.saveFile("/home/new.out");

	*/
	

	// Create a translation unit
	//elsa::TranslationUnit unit;

	// Then create keywords and actions
	//unit.createAction("::sayhello", hihi);
	//unit.createKeyword("Hello!", "::sayhello");

	// Then get type (17th and 18th bit, use ident endiannes)
	//std::printf("0x%02x + 0x%02x = 0x%04x", a, b, c);

}

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

/*
int cp_stage_1(BBP::std::async_stack_t<int> &stack, BBP::std::async_stack_t<int> &arg)
{
	// Return arg
	return stack.template get<0>() = (arg.template get<0>() * 9) / 5;
}

int cp_stage_2(BBP::std::async_stack_t<int> &stack, BBP::std::async_stack_t<int> &arg)
{
	return stack.template get<0>() = (stack.template get<0>() + 32);
}

int rep_stg1(BBP::std::async_stack_t<int> &stack, BBP::std::async_stack_t<BBP::std::TaskFlowInterface *> &arg)
{
	// If exceeding, join
	if (stack.template get<0>() > 555)
		arg.template get<0>()->Kill();

	BBP::std::printf(".");

	return stack.template get<0>()++;
}


BBP::std::errno_t BBP::system::cp_builtin(std::size_t argc, std::c_string *argv)
{
	// Pool of tasks
	std::STATIC_PAGE<std::TaskFlowInterface *, 32> poolPage;
	std::TaskPool pool = poolPage;

	std::TaskTemplate<int (*)(int), 2, int>::atomic_t list[2] = { cp_stage_1, cp_stage_2 };
	std::TaskTemplate<int (*)(int), 2, int> d(list);

	std::TaskTemplate<int (*)(std::TaskFlowInterface *), 1, int>::atomic_t replist[1] = { rep_stg1 };
	std::TaskTemplate<int (*)(std::TaskFlowInterface *), 1, int> rep(replist);

	// Create task
	std::Task<int (*)(int), int> task;
	d.ShallowCopy(task.getList());

	// Create repeating task
	std::RepeatingTask<int (*)(), int> reptask;
	reptask.SetFunctor(rep_stg1);

	// Argument
	int cel = 30;

	auto t1 = task(30);
	auto t2 = reptask();

	// Then run
	pool.Add(t1);
	pool.Add(t2);

	// Run until not able to
	while (pool.Step());

	// Get results.
	int result1 = task.Join();
	int result2 = reptask.Join();

	// Print
	std::printf("%d*C is %d*F, counter is: %d\n", cel, result1, result2);

	/*
	std::async_task<int (*)(int), int>::lambda_t k = [](std::async_stack_t<int> &stack, std::async_stack_t<int> &arg) -> int { /*Double stack.template get<0>() = 2 * arg.template get<0>(); return 0; };
	std::async_task<int (*)(int), int>::lambda_t l = [](std::async_stack_t<int> &stack, std::async_stack_t<int> &arg) -> int { /*Return double again return stack.template get<0>() * 2; };

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
	*/

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

	info.release();

	

	return 0;
}
*/