#include "../BIOSServices.h"
#include "../../include/UEFI.h"
#include "../../include/DriverCommands.h"
#include "../../include/stdio.h"

bool OS::BIOS::WINDOWSE20::EntryPoint(BBP::system::UEFI *uefi)
{
	BBP::std::printf("This is where Windowse20 would boot.\n");
	return true;
}