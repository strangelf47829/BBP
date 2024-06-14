#include "../include/BIOS.h"
#include "../include/EFIShell.h"
#include "../../../include/Kernel.h"
#include "../../../include/SyscallList.h"
#include "../include/EFIDriverInterface.h"

void Host::BIOS::boot(BBP::system::EFI &efi, BBP::FirmwareInterface &Hardware)
{
bootStart:

	// Print splashscreen
	splash(efi);

	// Print entering BIOS setup
	print("\n\n\nPress <\e[0;35m");
	printCharacter(efi.post.biosModeKey);
	print("\e[0;37m> to enter interactive EFI shell.\n");

	// Get time now
	BBP::std::time_t now = milliseconds();

	// While delay,
	while (milliseconds() - now < efi.post.biosModeDelay)
	{
		// Get key
		BBP::std::string_element keyInp = 0;

		// If no key gotten, continue
		if (getKeyboardInput(keyInp) == false)
			continue;

		// If not required key, do nothing
		if (keyInp != efi.post.biosModeKey)
			continue;

		// Something gotten! enter boot interruption. If return false, do not continue booting
		if (Host::enterShell(*this) == false)
			return;

		// Reboot!
		goto bootStart;
	}

	print("normal boot...\n");

	BBP::system::Kernel::enterKernelSpace(efi, Hardware);
}