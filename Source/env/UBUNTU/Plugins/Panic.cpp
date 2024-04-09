#include "../../../OS/BIOSServices.h"
#include "../../../include/Kernel.h"
#include "../../../include/DriverCommands.h"
#include <cstdio>

void OS::BIOS::BBPOSIX::OSPanic(BBP::std::errno_t errorCode, BBP::std::c_string message)
{
	// Clear screen
	BBP::system::getKernelInstance().getScreenDriver().hardwareDriver.executeCommand(screenClearScreen, 0, nullptr);

	// Print panic
	for (BBP::std::byte k = 0; k < 255; k++)
		if (message)
			printf("Uh-oh! Kernel has panicked! code %d: %s\n", errorCode, message);
		else
			printf("Uh-oh! Kernel has panicked!\n");
}