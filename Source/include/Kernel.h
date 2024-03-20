#ifndef BBP_SYSTEM_KERNEL_H
#define BBP_SYSTEM_KERNEL_H

#include "KernelSubSystems.h"

namespace BBP
{

	namespace system
	{

		// This is the entry point for the bootloader. Usually called from 'main'.
		int bootloaderMain();
		

		KernelSubSystems *kernelSS();

		// This class holds all the information needed to run an actual system.
		// This includes drivers, applications, Security functions, etc...
		class Kernel
		{
			// Subsystems
			KernelSubSystems subsystems;

			// A running counter for amount of processes
			pid_t pidCount;
			std::index_t emptyFrame;

			// The minimum amount of software drivers needed to run an instance of this system.
			// This means: Screen driver, Keyboard driver, and file driver.
			DeviceDriver keyboardDriver;
			DeviceDriver screenDriver;
			DeviceDriver fileDriver;

			// Since there may be more drivers present, this page holds extra room for drivers
			std::PAGE<DeviceDriver> drivers;

			// Users
			bool rootConfigured;
			UserInformation *root;
			UserInformation *activeUser;
			
			// Maximum other users
			static constexpr std::size_t userSize = 4;
			std::STATIC_PAGE<UserInformation, userSize> systemUsers;

			// Daemons

		public:

			// Constructor
			Kernel();

			// Subsystems
			KernelSubSystems &SubSystems();

			// Functions to log into other users
			bool login(std::string username, std::string password);
			UserInformation &getActiveUser();
			
			void configureRoot(UEFI *);

			// Context tools
			void setKernelContext();					// Sets the context to kernel context
			void setHypervisorContext(std::index_t);	// Sets the context to the specified hypervisor's contexts

			// Functions to manipulate system contexts
			SystemContext *getCurrentSystemContext();
			void setSystemVolume(std::VOLUME *);
			void setSystemVolume(std::string_element label, std::conststring path);
			void setSystemProgmem();
			void setSystemMemory(std::ResourceManager *);

			// process frame manipulation
			// ..

			// Proc frame utilities
			std::index_t findFirstEmptyFrame();
			void AllocateFrame(std::index_t);
			void DeallocateFrame(std::index_t);

			pid_t getNextDaemonPID();
			pid_t getNextBinPID();
			pid_t getNextShellPID();

			// Spawning threads
			procFrame *allocateDaemonFrame(DaemonRecord *, std::string);
			procFrame *allocateDaemonFrame(procFrame *, DaemonRecord *, std::string);

			// Thread execution controls
			bool stepFrame(procFrame *);
			bool executeFrame(procFrame *);
			std::errno_t getFrameResult(procFrame *);

			// Called by the bootloader to configure the drivers
			DeviceDriver &getKeyboardDriver(); 
			bool activateKeyboardDriver();			// Returns true on failure, return false on success.
			DeviceDriver &getScreenDriver();
			bool activateScreenDriver();			// Returns true on failure, return false on success.
			DeviceDriver &getFileDriver();
			bool activateFileDriver();				// Returns true on failure, return false on success.

			// External stuff
			void allocateExternalDrivers(std::size_t);
			DeviceDriver &getNextAvailableDriver();

			// Load a driver into memory
			std::errno_t loadDriver(UEFILoadDriver loadDriver, DeviceDriver &intoDriver, std::size_t argc, std::word *argv);
			std::errno_t loadDriver(UEFILoadDriver loadDriver, DeviceDriver &);

			// The actual calling point for the kernel
			std::errno_t Entry(UEFI &, BootRecord::bootRecordEntryPoint);
			BootRecord::bootRecordEntryPoint getBBPEntryPoint();

			// This is the standard BBP 'OS' delivered with the BBP Library.
			// A stub is needed to fetch this.
			bool BBPEntry(UEFI *);

		};


		// Get the kernel singleton
		Kernel &getKernelInstance();
		bool BBPEntryStub(UEFI *);


	}

}

#endif