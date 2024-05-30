#ifndef BBP_BIOS_SYSINFO_H
#define BBP_BIOS_SYSINFO_H

#include "../../../include/Memory.h"
#include "../../../include/UEFI.h"

namespace Environment
{
	namespace BIOS
	{
		// Get total RAM size
		BBP::std::size_t totalSystemMemory();
		BBP::std::size_t totalFreeMemory();

		// Get CPU speed
		BBP::std::word getCPUData(BBP::system::UEFI *);

		// Get Display width and height
		BBP::std::halfword getDisplayCols();
		BBP::std::halfword getDisplayLines();


	}
}

#endif