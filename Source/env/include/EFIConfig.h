#ifndef ENV_EFI_CONFIGURATION_H
#define ENV_EFI_CONFIGURATION_H

#include "Bootloader.h"

// Configuration for EFI

namespace Host
{
	// Configure an EFI record given a BIOS
	void configure(BBP::BIOS *, BBP::system::EFI &);

	// Get the active configuration
	void setActiveInterface(BBP::system::EFI &);


}

#endif