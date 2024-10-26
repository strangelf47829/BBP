#include "../../include/BootRecord.h"
#include "../OSRegistry.h"
#include "../BIOSServices.h"

// Configure the OS in such way that the BIOS can boot into it.
BBP::system::BootRecord OS::BBPRecord(OS::BBPOSIX::canBoot, OS::BBPOSIX::EntryPoint, "BBPOSIX");

// Can always boot into BBP
bool OS::BBPOSIX::canBoot(BBP::system::EFI *)
{
	return true;
}
