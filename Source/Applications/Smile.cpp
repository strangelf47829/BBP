#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Resources.h"
#include "../include/ELF.h"
#include "../include/FileSys.h"
#include "../include/Lex.h"
#include "../include/CPPApplications.h"
#include "../include/Executable.h"
#include "../include/System.h"
#include "../include/Graphics.h"
#include "../include/OS.h"
#include "../include/System.h"
#include "../include/Locale.h"
#include "../include/ELSA.h"
#include "../include/Threading.h"
#include "../include/Hyperv.h"
#include "../include/StateMachine.h"
#include "../include/Time.h"

BBP::userspace::HyperVisor hypervisor;
BBP::userspace::HyperVisor hypervisor2;
BBP::userspace::StateMachine state;


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

	BBP::std::word count = BBP::std::printf("%s", string);

	return count;
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

int BBP::smile_main(int argc, char **argv)
{


	/*
	
	std::window *w = &BBP::std::kernelDisplay;

	for (std::index_t idx = 0; idx < points - 1; idx++)
		std::R2D::Line(w, xpoints[idx], ypoints[idx], xpoints[idx + 1], ypoints[idx + 1]);
	std::R2D::Line(w, xpoints[0], ypoints[0], xpoints[points - 1], ypoints[points - 1]);

	std::R2D::Line(w, 770, 405, 770, 420);
	std::R2D::Line(w, 780, 405, 780, 420);

	std::R2D::Line(w, 740, 470, 810, 470);
	std::R2D::Line(w, 740, 470, 735, 465);
	std::R2D::Line(w, 810, 470, 815, 465);

	

	std::pos_t xo = w->cursorPosX;
	std::pos_t yo = w->cursorPosY;
	std::pos_t co = w->coloumn;
	std::pos_t lo = w->line;

	w->fontSize = 5;
	w->cursorPosX = 500;
	w->cursorPosY = 200;
	w->coloumn = 0;
	w->line = 0;
	const char *hello = "\xe1\xe1!\n";

	std::R2D::print(w, hello);

	w->fontSize = 0;
	w->cursorPosX = 0;
	w->cursorPosY = yo;
	w->coloumn = co;
	w->line = lo;
	
	*/

	// EmbeddabLe Simple Assembler
	// Roger Bemelmans
	std::PATH testFile("/boot/ELSA.esa");
	std::PATH dstFile("/boot/ELSA.o");

	// Compile source
	esa::processor.translate(testFile, dstFile);

	// Now try and deconstruct it
	std::FILE compiledFile(dstFile);
	std::FILE compiledFile2("/boot/ELSA2.o");

	// Get processor
	std::ELF::ELFBuilder compiled(compiledFile.b().page, system::kernelSS()->activeContext->activemem);
	std::ELF::ELFBuilder compiled2(compiledFile2.b().page, system::kernelSS()->activeContext->activemem);


	// Set the correspondig syscall
	hypervisor.systemcalls.data[0] = (BBP::userspace::HyperVisor::syscall_t)printcall;
	hypervisor.systemcalls.data[1] = (BBP::userspace::HyperVisor::syscall_t)printcallU;
	hypervisor.systemcalls.data[2] = (BBP::userspace::HyperVisor::syscall_t)printcallX;

	// Get time before execution in milliseconds
	std::time_t before = std::millis();

	// Get zero-th thread
	userspace::Thread *t = hypervisor.spawnThread(compiled, system::kernelSS()->activeContext->activemem);
	userspace::Thread *t2 = hypervisor.spawnThread(compiled2, system::kernelSS()->activeContext->activemem);

	// Unload binary since we have a copy
	compiled.close();
	compiledFile.close();
	compiled2.close();
	compiledFile2.close();

	hypervisor.currentPIDCount = 0;

	// Set active hypervisor
	state.setActiveHypervisor(&hypervisor);

	for (int i = 0; i < 1500; i++)
	{

		std::printf("\e[0;32m");
		state.setActiveThread(t);
		hypervisor.advanceThread(state, 10);

		std::printf("\e[0;31m");
		state.setActiveThread(t2);
		hypervisor.advanceThread(state, 10);
	}
	std::printf("\e[0;37m\n");

	// Close
	hypervisor.destroyThread(0);
	hypervisor.destroyThread(1);

	// Get time after execution in milliseconds
	std::time_t after = std::millis();

	// Now print
	std::printf("Time difference: %ums\n", after - before);

	return 0;
}
