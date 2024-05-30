#ifndef BBP_SYSTEM_EFI_H
#define BBP_SYSTEM_EFI_H

// This header defines stuff on how the bootloader interacts with the firmware.
// This is what platforms need to implement indepentantly.
// This is because the bootloader interfaces with the EFI to actually boot into BBP.
// This includes fetching drivers, loading boot records, reading configurations, time stuff, etc...

#include "stddef.h"
#include "DeviceDriver.h"
#include "BootRecord.h"
#include "Time.h"
#include "License.h"

namespace BBP
{
	namespace system
	{

		// Used by the bootloader to tell the firmware to do basic stuff (such as disable/enable user input).
		using EFIDirective = std::word (*)();

		// Used by the bootloader to tell the firmware to do parametric stuff (such as 'set fan speed to ...').
		using EFICommand = std::word (*)(std::word, std::byte **);

		// These type definitions define functions that the bootloader calls when loading fields.
		using EFILoadDriver = void (*)(DeviceDriver &);
		using EFILoadDaemon = void (*)();

		// This type definition is used by the bootloader to retrieve a field of data (Such as driver list, daemon list, battery count, etc...)
		template <typename T>
		using EFIGetManifest = std::size_t (*)(std::PAGE<T> &);

		// This type definition is used to retrieve a 
		using EFIConfigField = void (*)();

		// This structure is used to store functions needed to perform POST checks.
		// This includes: General post check, checking for boot records, etc...
		struct EFI_POST
		{
			// Perform basic post first
			EFIDirective basicPost;

			// Get a list of bootable boot records
			EFIGetManifest<BootRecord*> retrieveBootrecords;

			// Configuration for loading into BIOS
			std::string_element biosModeKey;
			std::time_t biosModeDelay;

		};

		// This structure is used to determine system capabilities.
		// This structure is also used to determine display modes by the screen driver.
		struct EFI_SysReport
		{
			// Name of underlying host
			std::conststring HostName;

			// Get total amount of useful heap
			std::size_t usefulHeap;

			// Get total system memory
			std::size_t totalMemory;

			// Get processor speed (in MHZ)
			std::word processorSpeed;

			// Get processor name
			std::static_string<32> processorName;

			// Get processor architecture
			std::static_string<16> processorArch;

			// UNIX Epoch at startup
			std::word epochUpper;
			std::word epochLower;

			// Display modes
			bool supportsTTY;
			bool supportsGUI;

			// What are the TTY Capabilities?
			std::halfword TTYHorizontalPage;
			std::halfword TTYVerticalPage;

			// What are the GUI capabilities?
			std::size_t VMEMSize;
			std::halfword xResolution;
			std::halfword yResolution;
			std::byte colourDepth;
		};
		

		// This structure is used to store function pointers to load drivers into the kernel before the kernel activates
		struct EFI_Drivers
		{
			// Load keyboard, screen, file system, and system driver
			EFILoadDriver loadKeyboard;
			EFILoadDriver loadScreen;
			EFILoadDriver loadFileSystem;
			EFILoadDriver loadSystem;

			// Get list of other drivers
			EFIGetManifest<EFILoadDriver> getOtherDrivers;

			// Load remaining drivers
			EFIDirective loadOtherDrivers;
		};

		// Forward declare daemon stuff
		struct DaemonRecord;

		// This structure is used to store function pointers to load daemons
		struct EFI_Daemons
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
		struct EFI_State
		{
			EFIDirective powerOff;
			EFIDirective suspend;
			EFIDirective wakeUp;
		};

		// This structure is used to hold function pointers to remaining shell actions. 
		struct EFI_Shell
		{
			// Enables and disables the shell
			EFIDirective enableShell;
			EFIDirective disableShell;

			// Performs the necessary action
			EFICommand shellCommand;

		};

		// This struct is used for the kernel. It tells the kernel how to load in various components
		struct EFI_System
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

		// This struct is used for defining application licenses and common names, etc..
		struct EFI_LicenseInfo
		{
			// BIOS Version
			appInfo BIOSInfo;
			
			// Kernel Version
			appInfo KernelInfo;

			// Name of kernel
			std::conststring KernelName;

		};

		// This structure is used to store the entire EFI configuration, which can either be a constant or a dynamically configured object.
		// Either way, a single function retrieves that EFI action.
		struct EFI
		{
			// Holds a structure to each configuration
			EFI_POST post;
			EFI_SysReport systemReport;
			EFI_Drivers drivers;
			EFI_Daemons daemons;
			EFI_State state;
			EFI_Shell shell;
			EFI_System system;
			EFI_LicenseInfo licenses;
		};

		const int invalidConfig = -1;
		const int failedPOST = -2;
		const int noBootRecords = -3;
		const int noKeyboard = -4;
		const int keyboardInitFailed = -5;
		const int screenInitFailed = -6;
		const int fileSystemInitFailed = -7;
		const int systemInitFailed = -8;
		const int outOfDaemonRecords = -9;
		const int OSError = -10;

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