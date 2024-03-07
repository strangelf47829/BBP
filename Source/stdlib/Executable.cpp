#include "../include/Executable.h"
#include "../include/CPPApplications.h"
#include "../include/stdio.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../include/Kernel.h"

const BBP::std::Executable BBP::std::executables[] = {{shell_main, "/proc/shell"}};

BBP::std::STATIC_PAGE<BBP::std::executableFrame, 12> BBP::std::procFrames;
BBP::std::Stack<BBP::std::executableFrame> BBP::std::Processes(&procFrames);

BBP::std::PID_t BBP::std::activeExecutable = 0;

void BBP::std::execute(std::executable_main mainFunc, int argc, char **argv)
{
	// Find a new PID
	PID_t procPID = getNextAvailablePID();

	// Create new process stack
	executableFrame procFrame = std::read(&procFrames, procPID);

	// Set pid
	procFrame.PID = procPID;

	// Create process memory
	procFrame.pmem = BBP::system::kernelSS()->activeContext->progmem.add_object(new std::ResourceManager());

	// Swap that in for the old activemem
	BBP::std::ResourceManager *oldActiveMem = BBP::system::kernelSS()->activeContext->activemem;
	BBP::system::kernelSS()->activeContext->activemem = procFrame.pmem;

	// Store old active executable
	PID_t oldActiveExecutable = std::activeExecutable;

	// Set active executable to this one
	std::activeExecutable = procPID;

	// Run main function in try block
	try 
	{
		// Set 'Is executing' flag to true
		procFrame.isExecuting = true;

		// Acually execute the program
		BBP::system::kernelSS()->activeContext->__errno = mainFunc(argc, argv);

		// Reset the 'is executing' flag
		procFrame.isExecuting = false;

		// Reset terminal
		BBP::system::kernelSS()->activeContext->display.fontSize = 1;
		BBP::system::kernelSS()->activeContext->display.fontSpacing = 6;
		BBP::std::R2D::fill(&BBP::system::kernelSS()->activeContext->display, 0xFFFFFFFF);
	}
	catch (const std::SIGNAL &signal)
	{
		switch (signal._sig)
		{
		case BBP::std::SIGSHTDN:
			// System shutdown, rethrow signal, but before doing that, reset the state of this program
			
			// Garbage collection
			procFrame.pmem->deleteAll();
			procFrame.pmem->freeAll();

			// Delete garbage collector
			BBP::system::kernelSS()->activeContext->progmem._delete(procFrame.pmem);

			// Restore active mem and active proc
			BBP::system::kernelSS()->activeContext->activemem = oldActiveMem;
			BBP::std::activeExecutable = oldActiveExecutable;
			throw;

		default:
			BBP::std::printf("[bbp] Program terminated after receiving signal %u", signal._sig);

			if (signal._sig == std::SIGILL)
				BBP::std::printf(" (SIGILL).");

			if (signal._sig == std::SIGSEGV)
				BBP::std::printf(" (SIGSEGV).");

			if (signal._sig == std::SIGSEC)
				BBP::std::printf(" (Security fault).");

			BBP::std::printf("\r\n");
			break;
		}
	}
	catch (std::except exception)
	{
		BBP::std::printf("[bbp] Uncaught exception: halting program.\r\n");
		system::kernelSS()->activeContext->STDOUT <<= system::kernelSS()->activeContext->STDERR;
	}
	catch (...)
	{
		BBP::std::printf("[bbp] Program terminated after uncaught object.\r\n");
	}

	if (system::kernelSS()->activeContext->__errno && system::kernelSS()->activeContext->STDERR[0])
		BBP::std::printf("Program terminated with code %d: %s\n\r", system::kernelSS()->activeContext->__errno, &system::kernelSS()->activeContext->STDERR[0]);

	// Print stuff
	//std::printf("(%p) Allocated a total of %u bytes during runtime.\n", procFrame.pmem, procFrame.pmem->totalAllocations);

	// Garbage collection
	BBP::std::size_t deletedCount = procFrame.pmem->deleteAll();
	BBP::std::size_t freedCount = procFrame.pmem->freeAll();

	if (deletedCount || freedCount)
	{
		printf("\nNot all memory on the heap was released.\n");

		printf("\tAmount of blocks free'd: %u\n", freedCount);
		printf("\tAmount of objects delete'd: %u\n", deletedCount);
	}

	// Delete garbage collector
	BBP::system::kernelSS()->activeContext->progmem._delete(procFrame.pmem);

	// Restore active mem and active proc
	BBP::system::kernelSS()->activeContext->activemem = oldActiveMem;
	BBP::std::activeExecutable = oldActiveExecutable;
}

BBP::std::PID_t pidCounter = 0;
BBP::std::PID_t BBP::std::getNextAvailablePID()
{
	return pidCounter++;
}

void BBP::std::exit()
{
	printf("\n\r");
	std::raise(std::SIGABRT);
}