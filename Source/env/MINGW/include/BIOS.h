#ifndef BBP_BIOS_H
#define BBP_BIOS_H

#include "../../../include/stddef.h"
#include "../../../include/EFI.h"

namespace Environment
{
	namespace BIOS
	{
		// Post stuff
		BBP::std::word POST();

		// Is possible to boot into BIOS?
		bool canBootIntoBIOS(BBP::system::EFI *);

		// Bootrecord
		extern BBP::system::BootRecord bootRecord;

		// BIOS Entry point
		bool EntryPoint(BBP::system::EFI *);
	}
}

#endif
