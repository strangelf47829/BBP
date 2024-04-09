#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"

BBP::std::word printcall(BBP::userspace::StateMachine *state, BBP::userspace::HyperVisor *, BBP::userspace::Thread *thr, BBP::userspace::Instruction &Args)
{
	// Get a page
	BBP::userspace::xvalue val = BBP::userspace::xvalue(*state, Args.args[1]);

	// Get the physical address
	BBP::std::address_t physicalAddress = val.getOwnPhysicalAddress();

	// Create string
	BBP::std::c_string string = nullptr;

	// Check if string is in TLS or in executable
	if (physicalAddress >= thr->executable.BinaryData.dataSize)
		string = thr->executable.BinaryData.nextPage->data + physicalAddress - thr->executable.BinaryData.dataSize;
	else
		string = thr->executable.BinaryData.data + physicalAddress;

	// Move string to stdout
	BBP::system::getKernelInstance().getScreenDriver().softwareDriver << string;

	return 0;
}
BBP::std::word printcallU(BBP::userspace::StateMachine *state, BBP::userspace::HyperVisor *, BBP::userspace::Thread *thr, BBP::userspace::Instruction &Args)
{
	// Get a page
	BBP::userspace::xvalue val = BBP::userspace::xvalue(*state, Args.args[1]);

	// Print
	BBP::std::word word = val.resolve(*state);
	BBP::std::word count = BBP::std::printf("%d\n", word);

	return count;
}
BBP::std::word printcallX(BBP::userspace::StateMachine *state, BBP::userspace::HyperVisor *, BBP::userspace::Thread *thr, BBP::userspace::Instruction &Args)
{
	// Get a page
	BBP::userspace::xvalue val = BBP::userspace::xvalue(*state, Args.args[1]);

	// Print
	BBP::std::word word = val.resolve(*state);
	BBP::std::word count = BBP::std::printf("0x%x\n", word);

	return count;
}

BBP::std::errno_t BBP::system::rae_builtin(std::size_t argc, std::c_string *argv)
{
	// This is the file to run
	std::string runFile = std::String("a.out");


	// Foreach arg,
	for (std::index_t idx = 1; idx < argc; idx++)
	{
		//If empty argument, do nothing
		if (argv[idx] == nullptr)
				continue;

		if (argv[idx][0] == 0)
			continue;

		// If argument does not start with '-', set file to run
		if (argv[idx][0] != '-')
		{
			runFile = std::string(std::strlen(argv[idx]), argv[idx]);
			continue;
		}

		// Get character of argument
		std::string_element argCharacter = argv[idx][1];

		// Do thing based on following:
		switch (argCharacter)
		{
			// Empty argument
		case 0:
			std::printf("Skipping empty argument '-'.\n");
			break;

		default:
			std::printf("Unkown argument '-%c'.\n", argCharacter);
			break;

		}
	}

	// Get path for file to run
	std::PATH runFilePath(runFile);

	// Get own frame
	procFrame *ownFrame = getKernelInstance().SubSystems().activeContext->activeFrame;

	// Get home directory
	std::PATH currentPath;

	// Copy data from old path into current path
	currentPath.copyFrom(*(ownFrame->systemContext->workingDirectory));

	// Make paths absolute
	std::PATH::makeRelative(currentPath, runFilePath);

	// Now try running
	try
	{
		// Open file
		std::FILE fileToRun(runFilePath);

		// Convert to binary format
		std::ELF::ELFBuilder compiled(fileToRun.b().page, system::kernelSS()->activeContext->activemem);

		// Get hypervisor
		userspace::HyperVisor *hyperv = &getKernelInstance().SubSystems().hypervisors[0];

		// Set syscalls
		hyperv->systemcalls.data[0] = (BBP::userspace::HyperVisor::syscall_t)printcall;
		hyperv->systemcalls.data[1] = (BBP::userspace::HyperVisor::syscall_t)printcallU;
		hyperv->systemcalls.data[2] = (BBP::userspace::HyperVisor::syscall_t)printcallX;

		// Spawn thread
		userspace::Thread *t = hyperv->spawnThread(compiled, system::kernelSS()->activeContext->activemem);

		// Get state
		userspace::StateMachine *state = &getKernelInstance().SubSystems().state;

		// Set state active hypervisor
		state->setActiveHypervisor(hyperv);

		// Now we can unload the binary file
		compiled.close();
		fileToRun.close();

		// Get time before execution
		std::time_t before = std::micros();

		// Amount of instructions ran
		std::word instructions1 = 0;

		// Run until execution is done.
		while (t->isThreadCold() == false)
		{
			// Set activethread
			state->setActiveThread(t);

			// Then advance thread
			hyperv->advanceThread(*state);

			instructions1++;
		}

		// Get time after execution
		std::time_t after = std::micros();

		// Time is in microseconds
		std::time_t delta = after - before;

		// Frequency is measured in
		std::word kiloHz = (instructions1 * 1000) / delta;

		// Now print deltatime
		std::printf("Time difference: %uus. Instructions: (%u). Speed: %uKHz.\n", delta, instructions1, kiloHz);

		// Close thread after execution
		hyperv->destroyThread(0);
	}
	catch (std::except const &e)
	{
		std::printf("Exec error.\n");
	}
	catch (std::signal_t const &e)
	{
		std::printf("Signal received: %u\n", e);
	}
	
	return 0;
}