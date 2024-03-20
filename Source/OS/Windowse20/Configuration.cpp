#include "../../include/BootRecord.h"
#include "../OSRegistry.h"
#include "../BIOSServices.h"
#include "../../include/Kernel.h"

// Configure the OS in such way that the BIOS can boot into it.
BBP::system::BootRecord Environment::BIOS::Windowse20Record(OS::BIOS::WINDOWSE20::canBoot, OS::BIOS::WINDOWSE20::EntryPoint, "Windowse 20");

// Can only boot into Windowse20 if screen supports the GUI, and if there is a mouse driver present.
bool OS::BIOS::WINDOWSE20::canBoot(BBP::system::UEFI *uefi)
{
	// Check for GUI capabilities. Get screen driver
	BBP::system::DeviceDriver *driver = &BBP::system::getKernelInstance().getScreenDriver();

	// Get driver metadata
	BBP::std::printf("Screen Hardware identification: 0x%08x\n", driver->hardwareDriver.hwid);
	BBP::std::printf("Screen device identification: 0x%08x\n", driver->hardwareDriver.dvid);

	return false;
}