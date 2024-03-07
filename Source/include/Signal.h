#ifndef BBP_STDLIB_SIGNAL_H
#define BBP_STDLIB_SIGNAL_H

#include "stddef.h"

namespace BBP
{
	namespace std
	{
		typedef int signal_t;

		struct SIGNAL
		{
			signal_t _sig;

			SIGNAL(signal_t sig)
				: _sig(sig)
			{
			}
		};

		void raise(signal_t);

		constexpr signal_t SIGINV = 1;
		constexpr signal_t SIGTERM = 2;
		constexpr signal_t SIGSEGV = 3;
		constexpr signal_t SIGINT = 4;
		constexpr signal_t SIGILL = 5;
		constexpr signal_t SIGABRT = 6;
		constexpr signal_t SIGFPE = 7;
		constexpr signal_t SIGSHTDN = 8; // Shut down the system
		constexpr signal_t SIGTHREXP = 9; // Signal thread exception. 
		constexpr signal_t SIGEND = 10; // Signals the end of a process by reaching the end
		constexpr signal_t SIGADJ = 150;
	}
}


#endif