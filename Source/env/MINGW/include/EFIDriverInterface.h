#ifndef HOST_EFI_DRIVERINTERFACE_H
#define HOST_EFI_DRIVERINTERFACE_H

#include "../../../include/EFI.h"

namespace Host
{
	// Get timestamp
	void getStartupUnixTimestamp(BBP::std::word &, BBP::std::word &);
}

#endif