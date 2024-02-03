#ifndef BBP_STDLIB_SIGNAL_H
#define BBP_STDLIB_SIGNAL_H

#include "stddef.h"

// Invalid signal


#define __SIGNAL__(sig) BBP::std::__errno = BBP::std::sig + BBP::std::SIGADJ; throw BBP::std::SIGNAL(BBP::std::sig)

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

		constexpr signal_t SIGINV = 1;
		constexpr signal_t SIGTERM = 2;
		constexpr signal_t SIGSEGV = 3;
		constexpr signal_t SIGINT = 4;
		constexpr signal_t SIGILL = 5;
		constexpr signal_t SIGABRT = 6;
		constexpr signal_t SIGFPE = 7;
		constexpr signal_t SIGSHTDN = 8; // Shut down the system
		constexpr signal_t SIGTHREXP = 9; // Signal thread exception. 
		constexpr signal_t SIGADJ = 150;
	}
}


#endif