#ifndef OS_BIOSSERVICES_H
#define OS_BIOSSERVICES_H

#include "../include/EFI.h"

namespace OS
{
	namespace BBPOSIX
	{
		// Checks if the OS can be executed
		bool canBoot(BBP::system::EFI *);

		// Actual OS Entry point
		bool EntryPoint(BBP::system::EFI *, BBP::std::TaskPool *&);

		// OS Panic
		void OSPanic(BBP::std::errno_t, BBP::std::c_string);
	}

	namespace WINDOWSE20
	{
		// Checks if the OS can be executed
		bool canBoot(BBP::system::EFI *);

		// Actual OS Entry point
		bool EntryPoint(BBP::system::EFI *, BBP::std::TaskPool *&);
	}
}
#endif