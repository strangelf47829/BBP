#include "../include/stdctrl.h"
#include "../include/stdio.h"

// Create static signals (stack get unwound anyway doofus)
BBP::std::KernelSignal shutdownSignal;

void BBP::std::panic()
{
	std::printf("Double fault detected. PANIC!\n");
	throw KernelSignal();
}

void BBP::std::kernel_unwind()
{
	// Set flag
	shutdownSignal.intendedUnwind = true;

	// Throw signal
	throw shutdownSignal;
}