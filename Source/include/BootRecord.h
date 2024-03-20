#ifndef BBP_KERNEL_BOOTRECORD_H
#define BBP_KERNEL_BOOTRECORD_H

#include "Strings.h"



namespace BBP
{
	namespace system
	{
		// A boot record is a structure that tells the bootloader how (and if) it should load in a particular manner.
		// This does not mean how to necessarily load an OS, but more of a 'what c++ function is in this case the exit point of the bootloader?'
		// This is useful for when the firmware may wish to boot into a BIOS mode, or flash mode, or just normal mode, etc...

		struct UEFI;

		struct BootRecord
		{
			// Name of the record name (For example 'BIOS', 'Flash', 'Normal', 'Network', 'Shell', etc...)
			std::static_string<16> recordName;

			// This function checks if this boot record is available
			bool (*isBootRecordAvailable)();

			// This function attempts to load into the boot record
			using bootRecordEntryPoint = bool (*)(UEFI *);
			bootRecordEntryPoint entryPoint;

			BootRecord() = delete;
			BootRecord(bool (*check)(), bootRecordEntryPoint entry, std::conststring name);
			

		};
	}
}

#endif
