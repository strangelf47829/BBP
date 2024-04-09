#ifndef BBP_STD_STDCTRL_H
#define BBP_STD_STDCTRL_H

// This header defines standard actions used by applications to do various things
namespace BBP
{
	namespace std
	{

		// The bootloader listens for this kind of signal
		struct BootSignal
		{

		};

		// The kernel listens for this kind of signal
		struct KernelSignal
		{
			// Indicates if the intention is to unwind the stack until the kernel
			bool intendedUnwind;



		};

		// A process will listen for this kind of signal
		struct ProcSignal
		{

		};

		// Abort or kill an application
		void abort();
		void kill();

		// Send a signal to an application
		void signal();

		// Cause a kernel panic
		void panic();

		// unwind until the kernel
		void kernel_unwind();

	}
}

#endif
