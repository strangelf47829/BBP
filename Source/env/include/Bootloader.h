#ifndef ENV_BOOTLOADER_H
#define ENV_BOOTLOADER_H

#include "../../include/FirmwareInterface.h"

// Get references to commonly used functions

namespace Host
{
	// Global BIOS getter
	BBP::BIOS *getBIOS();

}

#endif