#include "../include/stdctrl.h"

// Create static signals (stack get unwound anyway doofus)
BBP::std::KernelSignal shutdownSignal;

void BBP::std::panic()
{
	throw KernelSignal();
}

void BBP::std::kernel_unwind()
{
	// Set flag
	shutdownSignal.intendedUnwind = true;

	// Throw signal
	throw shutdownSignal;
}