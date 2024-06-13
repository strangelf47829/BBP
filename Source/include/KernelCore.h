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

			// Primary kernel volume
			std::VOLUME systemVolume;

			// Firmware
			FirmwareInterface *firmware;

			// Configuration used to load this kernel
			EFI *configuration;

			// Information about logged in user
			UserInformation loggedInUser;

			// I/O stuff
			std::STATIC_PAGE<std::string_element, std::max_string_length> STDOUTstream;
			std::STATIC_PAGE<std::string_element, std::max_string_length> STDINstream;
			std::STATIC_PAGE<std::string_element, std::max_string_length> STDERRstream;

			// File table stuff
			std::STATIC_PAGE<std::FileNode, std::max_open_files> fileTable;

			// Memory access
			std::ResourceManager kernelMemory;

		public:

			// Constructors
			KernelCore();
			KernelCore(FirmwareInterface *, EFI *, Kernel *);

		};
		


	}
}

#endif