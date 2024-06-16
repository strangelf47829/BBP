#ifndef HOST_EFI_SHELL_H
#define HOST_EFI_SHELL_H

#include "BIOS.h"

namespace Host
{

	// Enter interactive EFI Shell
	bool enterShell(BIOS &);

	// Load line from reader
	void readLine(BIOS &);

}

#endif