#include "../include/Signal.h"
#include "../include/stdio.h"
#include "../include/Resources.h"
#include "../include/FileSys.h"
#include "../include/Executable.h"
#include "../include/CPPApplications.h"
#include "../include/Time.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../include/Kernel.h"



int main()
{
	
	// init
		

	//BBP::std::STDERR.writeTo = BBP::std::print_to_terminal;
	BBP::system::kernelSS()->activeContext->STDOUT.writeTo = BBP::std::print_to_terminal;
	BBP::system::kernelSS()->activeContext->activemem = &BBP::system::kernelSS()->activeContext->progmem;

	for (BBP::std::index_t node = 0; node < BBP::std::max_open_files; node++)
		BBP::system::kernelSS()->activeContext->fileTable.data[node] = BBP::std::FileNode();

	BBP::std::VOLUME prim('v', "/mnt/v/");
	BBP::system::kernelSS()->activeContext->primaryVolume = &prim;

	BBP::std::PATH p("/home/");
	BBP::system::kernelSS()->activeContext->workingDirectory = &p;

	BBP::std::FILE f(BBP::system::kernelSS()->activeContext->STDOUT, "/proc/stdout");
	BBP::std::FILE f2(BBP::system::kernelSS()->activeContext->STDIN, "/proc/stdin");

	BBP::std::Driver::initializeGraphics(&BBP::system::kernelSS()->activeContext->display, 1000, 800);

	//BBP::std::PATH path("/bin/font");
	//BBP::std::R2D::GetFontFromFile(&BBP::std::kernelDisplay, path);

	//BBP::std::kernelDisplay.fontSize = 2;
	//BBP::std::kernelDisplay.fontSpacing = 6;
	

	// Initialize system clock
	BBP::std::initClock();

	int magicCount = 4;

	BBP::std::PAGE<BBP::std::string_element> bootPage(magicCount + sizeof(BBP::std::executable_main) / sizeof(BBP::std::string_element),
		(BBP::std::string_element *)BBP::system::kernelSS()->activeContext->activemem->malloc(magicCount * sizeof(BBP::std::string_element) + sizeof(BBP::std::executable_main)));
	BBP::std::Stack<BBP::std::string_element> bootStack(&bootPage, bootPage.dataSize);
	((BBP::std::executable_main *)bootPage.raw)[0] = BBP::boot_main;
	
	bootPage.raw[BBP_EXEC_MAGIC0IDX] = BBP_EXEC_MAGIC0;
	bootPage.raw[BBP_EXEC_MAGIC1IDX] = BBP_EXEC_MAGIC1;
	bootPage.raw[BBP_EXEC_MAGIC2IDX] = BBP_EXEC_MAGIC2;
	bootPage.raw[BBP_EXEC_MAGIC3IDX] = BBP_EXEC_MAGIC3;

	BBP::std::PAGE<BBP::std::string_element> logoutPage(magicCount + sizeof(BBP::std::executable_main) / sizeof(BBP::std::string_element),
		(BBP::std::string_element *)BBP::system::kernelSS()->activeContext->activemem->malloc(magicCount * sizeof(BBP::std::string_element) + sizeof(BBP::std::executable_main)));
	BBP::std::Stack<BBP::std::string_element> logoutStack(&logoutPage, logoutPage.dataSize);
	((BBP::std::executable_main *)logoutPage.raw)[0] = BBP::logout_main;

	logoutPage.raw[BBP_EXEC_MAGIC0IDX] = BBP_EXEC_MAGIC0;
	logoutPage.raw[BBP_EXEC_MAGIC1IDX] = BBP_EXEC_MAGIC1;
	logoutPage.raw[BBP_EXEC_MAGIC2IDX] = BBP_EXEC_MAGIC2;
	logoutPage.raw[BBP_EXEC_MAGIC3IDX] = BBP_EXEC_MAGIC3;

	BBP::std::PAGE<BBP::std::string_element> neofetchPage(magicCount + sizeof(BBP::std::executable_main) / sizeof(BBP::std::string_element),
		(BBP::std::string_element *)BBP::system::kernelSS()->activeContext->activemem->malloc(magicCount * sizeof(BBP::std::string_element) + sizeof(BBP::std::executable_main)));
	BBP::std::Stack<BBP::std::string_element> neofetchStack(&neofetchPage, neofetchPage.dataSize);
	((BBP::std::executable_main *)neofetchPage.raw)[0] = BBP::neofetch_main;

	neofetchPage.raw[BBP_EXEC_MAGIC0IDX] = BBP_EXEC_MAGIC0;
	neofetchPage.raw[BBP_EXEC_MAGIC1IDX] = BBP_EXEC_MAGIC1;
	neofetchPage.raw[BBP_EXEC_MAGIC2IDX] = BBP_EXEC_MAGIC2;
	neofetchPage.raw[BBP_EXEC_MAGIC3IDX] = BBP_EXEC_MAGIC3;

	BBP::std::PAGE<BBP::std::string_element> badapplePage(magicCount + sizeof(BBP::std::executable_main) / sizeof(BBP::std::string_element),
		(BBP::std::string_element *)BBP::system::kernelSS()->activeContext->activemem->malloc(magicCount * sizeof(BBP::std::string_element) + sizeof(BBP::std::executable_main)));
	BBP::std::Stack<BBP::std::string_element> badAppleStack(&badapplePage, badapplePage.dataSize);
	((BBP::std::executable_main *)badapplePage.raw)[0] = BBP::smile_main;

	badapplePage.raw[BBP_EXEC_MAGIC0IDX] = BBP_EXEC_MAGIC0;
	badapplePage.raw[BBP_EXEC_MAGIC1IDX] = BBP_EXEC_MAGIC1;
	badapplePage.raw[BBP_EXEC_MAGIC2IDX] = BBP_EXEC_MAGIC2;
	badapplePage.raw[BBP_EXEC_MAGIC3IDX] = BBP_EXEC_MAGIC3;

	BBP::std::FILE bootExec(bootStack, "/proc/exec/boot");
	BBP::std::FILE logoutExec(logoutStack, "/proc/exec/logout");
	BBP::std::FILE neofetchExec(neofetchStack, "/proc/exec/neofetch");
	BBP::std::FILE badappleExec(badAppleStack, "/proc/exec/elsa");

	// Execute something
	BBP::std::captureInput();

	try {
		BBP::std::execute(BBP::std::executables[0].entry, 0, nullptr);
	}
	catch (...)
	{

	}

	BBP::std::stopCapturingInput();

	// Print error message if errno is not 0
	if (BBP::system::kernelSS()->activeContext->__errno && BBP::system::kernelSS()->activeContext->STDERR.atElement)
		BBP::system::kernelSS()->activeContext->STDOUT <<= BBP::system::kernelSS()->activeContext->STDERR.page->data;

	// Clean up
	bootExec.close();
	f.close();
	f2.close();

	BBP::system::kernelSS()->activeContext->progmem.free(bootPage.data);
	BBP::system::kernelSS()->activeContext->progmem.free(logoutPage.data);
	BBP::system::kernelSS()->activeContext->progmem.free(neofetchPage.data);
	BBP::system::kernelSS()->activeContext->progmem.free(badapplePage.data);
	//BBP::std::progmem.free(BBP::std::fileTable.data);

	BBP::system::kernelSS()->activeContext->progmem.deleteAll();

	BBP::std::Driver::destructGraphics();
	BBP::std::vmem.dealloc();

	return BBP::system::kernelSS()->activeContext->__errno;
	
}



/*
BBP::std::static_string STDERRPage;
BBP::std::Stack<BBP::std::string_element> BBP::std::STDERR(&STDERRPage, 4096);

BBP::std::static_string STDOUTPage;
BBP::std::Stack<BBP::std::string_element> BBP::std::STDOUT(&STDOUTPage, 4096);

BBP::std::static_string STDINPage;
BBP::std::Stack<BBP::std::string_element> BBP::std::STDIN(&STDINPage, 4096);
*/
/*
void exec()
{
	BBP::std::VOLUME prim('v', "/mnt/v/");
	BBP::std::primaryVolume = &prim;

	BBP::std::PATH p("/home/");
	BBP::std::workingDirectory = &p;

	BBP::std::FILE f(BBP::std::STDOUT, "/mnt/v/proc/stdout");
	BBP::std::FILE f2(BBP::std::STDIN, "/mnt/v/proc/stdin");

	try
	{
		BBP::std::__errno = BBP::main();
	}
	catch (BBP::std::except &e)
	{
		std::printf("\nUncaught exception: Program terminated with code %u.\n", BBP::std::__errno);
	}
	catch (int sig)
	{
		switch (sig)
		{
		case BBP::std::SIGTERM:
			BBP::std::STDERR << "[BBP] Received signal 'SIGTERM': Terminating program." <<= BBP::std::endl;
			break;
		case BBP::std::SIGSEGV:
			BBP::std::STDERR << "[BBP] Received signal 'SIGSEGV': Terminating program." <<= BBP::std::endl;
			break;
		case BBP::std::SIGINT:
			BBP::std::STDERR << "[BBP] Received unhandled interrupt: Terminating program." <<= BBP::std::endl;
			break;
		case BBP::std::SIGILL:
			BBP::std::STDERR << "[BBP] Illegal instruction: Terminating program." <<= BBP::std::endl;
			break;
		case BBP::std::SIGABRT:
			BBP::std::STDERR << "[BBP] Aborting program execution." <<= BBP::std::endl;
			break;
		case BBP::std::SIGFPE:
			BBP::std::STDERR << "[BBP] Unhandled math error: Terminating program." <<= BBP::std::endl;
			break;
		}
	}


	f.close();
	f2.close();

	BBP::std::progmem.free(BBP::std::fileTable.data);
}

int main()
{
	// init
	BBP::std::STDERR.writeTo = BBP::std::print_to_terminal;
	BBP::std::STDOUT.writeTo = BBP::std::print_to_terminal;
	BBP::std::activemem = &BBP::std::progmem;

	BBP::std::fileTable = BBP::std::PAGE<BBP::std::FileNode *>(BBP::std::max_open_files, (BBP::std::FileNode **)BBP::std::activemem->calloc(BBP::std::max_open_files, sizeof(BBP::std::FileNode *)));

	

	exec();

	// Destroy anything if needed.
	BBP::std::size_t deletedCount = BBP::std::activemem->deleteAll();
	BBP::std::size_t freedCount = BBP::std::activemem->freeAll();

	BBP::std::size_t totalCount = freedCount + deletedCount;
	
	if (totalCount)
	{
		BBP::std::STDERR << "\nNot all memory on the heap was released." <<= BBP::std::endl;
		
		BBP::std::printf("\tAmount of blocks free'd: %u\n", freedCount);
		BBP::std::printf("\tAmount of blocks delete'd: %u\n", deletedCount);

	}
	else
	{
		//BBP::std::STDERR << "All memory was released! Good job :)" <<= BBP::std::endl;
	}

	return BBP::std::__errno;
}

*/