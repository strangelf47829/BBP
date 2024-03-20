#include "../../include/BootRecord.h"
#include "../OSRegistry.h"
#include "../BIOSServices.h"

// Configure the OS in such way that the BIOS can boot into it.
BBP::system::BootRecord Environment::BIOS::BBPRecord(OS::BIOS::BBPOSIX::canBoot, OS::BIOS::BBPOSIX::EntryPoint, "BBPOSIX");

// Can always boot into BBP
bool OS::BIOS::BBPOSIX::canBoot(BBP::system::UEFI *)
{
	return true;
}