#ifndef BBP_ENV_H
#define BBP_ENV_H

#include "../../../include/stddef.h"
#include "../../../include/Strings.h"
#include "../../../include/UEFI.h"
#include "../../../include/Kernel.h"
#include "../../../include/DriverCommands.h"

namespace Environment
{

	namespace Drivers
	{

		struct Manifest
		{
			// Get actions
			BBP::system::HardwareAction actions[3];

			// The commands
			BBP::std::size_t commandCount;
			BBP::system::HardwareCmd *commands;

			// Loader
			BBP::system::UEFILoadDriver loader;
		};

		extern Manifest keyboardManifest;
		extern Manifest screenManifest;
		extern Manifest fileManifest;
		extern Manifest systemManifest;

	}

	namespace UEFI
	{
		
		BBP::system::UEFI &configureUEFI();

		BBP::std::size_t loadBootrecords(BBP::std::PAGE<BBP::system::BootRecord*> &records);

	}

}

#endif
