#ifndef BBP_SYSTEM_UEFI_H
#define BBP_SYSTEM_UEFI_H

// This header defines stuff on how the bootloader interacts with the firmware.
// This is what platforms need to implement indepentantly.
// This is because the bootloader interfaces with the UEFI to actually boot into BBP.
// This includes fetching drivers, loading boot records, reading configurations, time stuff, etc...

#include "stddef.h"
#include "DeviceDriver.h"
#include "BootRecord.h"
#include "Time.h"

namespace BBP
{
	namespace system
	{

		// Used by the bootloader to tell the firmware to do basic stuff (such as disable/enable user input).
		using UEFIDirective = std::word (*)();

		// Used by the bootloader to tell the firmware to do parametric stuff (such as 'set fan speed to ...').
		using UEFICommand = std::word (*)(std::word, std::byte **);

		// These type definitions define functions that the bootloader calls when loading fields.
		using UEFILoadDriver = void (*)(DeviceDriver &);
		using UEFILoadDaemon = void (*)();

		// This type definition is used by the bootloader to retrieve a field of data (Such as driver list, daemon list, battery count, etc...)
		template <typename T>
		using UEFIGetManifest = std::size_t (*)(std::PAGE<T> &);

		// This type definition is used to retrieve a 
		using UEFIConfigField = void (*)();

		// This structure is used to store functions needed to perform POST checks.
		// This includes: General post check, checking for boot records, etc...
		struct UEFI_POST
		{
			// Perform basic post first
			UEFIDirective basicPost;

			// Get a list of bootable boot records
			UEFIGetManifest<BootRecord*> retrieveBootrecords;

			// Configuration for loading into BIOS
			std::string_element biosModeKey;
			std::time_t biosModeDelay;

		};

		// This structure is used to store function pointers to load drivers into the kernel before the kernel activates
		struct UEFI_Drivers
		{
			// Load keyboard, screen, file system, and system driver
			UEFILoadDriver loadKeyboard;
			UEFILoadDriver loadScreen;
			UEFILoadDriver loadFileSystem;
			UEFILoadDriver loadSystem;

			// Get list of other drivers
			UEFIGetManifest<UEFILoadDriver> getOtherDrivers;

			// Load remaining drivers
			UEFIDirective loadOtherDrivers;
		};

		// Forward declare daemon stuff
		struct DaemonRecord;

		// This structure is used to store function pointers to load daemons
		struct UEFI_Daemons
		{
			// A list of DaemonRecords
			std::PAGE<system::DaemonRecord> records;

			// The amount of specially designated DAEMONS
			std::size_t specialDaemons;

			// A list of indicies for special daemons
			std::PAGE<std::index_t> specialIndicies;

			// Which daemon is init?
			std::index_t initDaemon;
		};

		// This structure is used to perform actions pertaining to the state of the system (such as suspend, power off, power on, etc...)
		struct UEFI_State
		{
			UEFIDirective powerOff;
			UEFIDirective suspend;
			UEFIDirective wakeUp;
		};

		// This structure is used to hold function pointers to remaining shell actions. 
		struct UEFI_Shell
		{
			// Enables and disables the shell
			UEFIDirective enableShell;
			UEFIDirective disableShell;

			// Performs the necessary action
			UEFICommand shellCommand;

		};

		// This struct is used for the kernel. It tells the kernel how to load in various components
		struct UEFI_System
		{
			// The volume label
			std::string_element volumeLabel;

			// The volume path
			std::conststring volumePath;

			// Root password
			std::conststring rootPassword;

			// Name of the device
			std::conststring deviceName;
		};

		// This structure is used to store the entire UEFI configuration, which can either be a constant or a dynamically configured object.
		// Either way, a single function retrieves that UEFI action.
		struct UEFI
		{
			// Holds a structure to each configuration
			UEFI_POST post;
			UEFI_Drivers drivers;
			UEFI_Daemons daemons;
			UEFI_State state;
			UEFI_Shell shell;
			UEFI_System system;
		};

		// This function is platform dependant. This provides flexibility in the sense that the platform may choose to read this from a config file
		// (Providing much greater flexibility), or just return a constant thing. Either way, the bootloader calls this function exactly once.
		UEFI &retrieveUEFI();

		const int invalidConfig = -1;
		const int failedPOST = -2;
		const int noBootRecords = -3;
		const int noKeyboard = -4;
		const int keyboardInitFailed = -5;
		const int screenInitFailed = -6;
		const int fileSystemInitFailed = -7;
		const int systemInitFailed = -8;

		// Platform dependant stuff
		extern HardwareAction keyboardActions[3];
		extern HardwareAction screenActions[3];
		extern HardwareAction fileSystemActions[3];
		extern HardwareAction systemActions[3];

		extern std::size_t keyboardCMDCount;
		extern std::size_t screenCMDCount;
		extern std::size_t fileSystemCMDCount;
		extern std::size_t systemCMDCount;

		extern HardwareCmd keyboardCMD[];
		extern HardwareCmd screenCMD[];
		extern HardwareCmd fileSystemCMD[];
		extern HardwareCmd systemCMD[];

	}
}



#endif