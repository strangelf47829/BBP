#ifndef OS_BIOSSERVICES_H
#define OS_BIOSSERVICES_H

#include "../include/UEFI.h"

namespace OS
{
	namespace BIOS
	{
		namespace BBPOSIX
		{
			// Checks if the OS can be executed
			bool canBoot(BBP::system::UEFI *);

			// Actual OS Entry point
			bool EntryPoint(BBP::system::UEFI *);

			// OS Panic
			void OSPanic(BBP::std::errno_t, BBP::std::c_string);
		}

		namespace WINDOWSE20
		{
			// Checks if the OS can be executed
			bool canBoot(BBP::system::UEFI *);

			// Actual OS Entry point
			bool EntryPoint(BBP::system::UEFI *);
		}

	}
}

#endif