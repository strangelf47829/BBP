#ifndef BBP_SYSTEM_KERNELCORE_H
#define BBP_SYSTEM_KERNELCORE_H

#ifndef BBP_SYSTEM_KERNEL_H
#ifndef IDE
#error Do not include "KernelCore.h", include "Kernel.h" instead
#endif
#endif


#include "FirmwareInterface.h"
#include "UserInformation.h"

// The kernel subsystem class handles with the state of the kernel, rather than performing tasks.
// This includes for example, keeping track of open files, etc...
// Technically, this would be the "Kernel" and the actual Kernel.h would be the shell, however I've decided to split
// the kernel into two parts like so so the actual Shell.h can access both the kernel and the Kernel SS.
// It also means that in the future, a hypervisor could be implemented by just instancing more kernel Core.

namespace BBP
{
	namespace system
	{

		// Forward declare Kernel
		class Kernel;

		class KernelCore
		{
		public:

			// Primary kernel volume
			std::VOLUME systemVolume;

			// Firmware
			FirmwareInterface *firmware;

			// External drivers
			std::PAGE<DeviceDriver *> externalDrivers;

			// Configuration used to load this kernel
			EFI *configuration;

			// Information about logged in user
			UserInformation loggedInUser;

			// I/O stuff
			std::static_string<509> STDERRPage;
			std::static_string<509> STDOUTPage;
			std::static_string<509> STDINPage;

			// File table stuff
			std::noderef_t activeNodeRef;
			std::STATIC_PAGE<std::FileNode, std::max_open_files> fileTable;

			// Memory access
			std::ResourceManager allocator;

			// Constructors
			KernelCore();

		};
		


	}
}

#endif