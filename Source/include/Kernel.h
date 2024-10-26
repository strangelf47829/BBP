#ifndef BBP_SYSTEM_KERNEL_H
#define BBP_SYSTEM_KERNEL_H

#include "KernelCore.h"
#include "Boot.h"
#include "Tasks.h"
#include "TaskPool.h"
#include "Hyperv.h"
#include "StateMachine.h"
#include "SystemContext.h"
#include "SysConfig.h"
#include "DeviceDriver.h"
#include "EFI.h"
#include "stdctrl.h"
#include "KernelCore.h"
#include "Interrupts.h"
#include "UserInformation.h"
#include "cstdio.h"
#include "FileSysInfo.h"
#include "SyscallArgs.h"
#include "drvcmd.h"

namespace BBP
{

	namespace system
	{

		// Forward declaration of Kernel Core
		class KernelCore;

		// This class holds all the information needed to run an actual system.
		// This includes drivers, applications, Security functions, etc...
		class Kernel
		{
			// Static kernel instance
			static Kernel singleton;

			// The EFI used to spawn this kernel
			EFI *sysconfig;

			// Kernel IRQ handler.
			std::KernelIRQHandler IRQ;

			// Taskpool for internal stuff (updating IRQ, handling IO, etc...)
			std::STATIC_PAGE<std::TaskFlowInterface *, 8> taskpoolData;
			std::TaskPool taskpool;

			// External OS taskpool
			std::TaskPool *extTaskPool;

			// Essential drivers loaders
			std::errno_t loadSystemDriver(EFI &);
			std::errno_t loadFileDriver(EFI &);
			std::errno_t loadScreenDriver(EFI &);
			std::errno_t loadKeyboardDriver(EFI &);

			// Tasks
			static bool kernelUpdateCycle(std::async_stack_t<> &stack, std::async_stack_t<std::TaskFlowInterface *> &arg);
			std::RepeatingTask<bool (*)()> kernelUpdateCycleTask;

			// Enter boot selection screen
			system::BootRecord::bootRecordEntryPoint enterBootSelection(std::PAGE<BootRecord *> &bootrecords, system::BootRecord::bootRecordEntryPoint entry, system::BootRecord::bootRecordEntryPoint BIOS);

			// File node stuff
			static bool isFileOpened(std::PATH &);
			static bool findEmptyNode();

			// RAE stuff
			userspace::StateMachine machine;
			std::STATIC_PAGE<userspace::HyperVisor, 1> hypervisors;
			
			// Kernel Core
			KernelCore core;

			// Fetch the core
			KernelCore &Core();

			// Screen stuff
			static void biosSplashCommand(EFI &);

			// Private constructor
			Kernel();

		public:

			// Boot into the kernel
			static std::errno_t enterKernelSpace(EFI &, FirmwareInterface &);

			// Set error
			static void setError(std::errno_t);

			// File node stuff
			static std::noderef_t allocateINode(std::PATH &);
			static std::noderef_t deallocateINode(std::noderef_t, bool, bool);
			static std::PATH &getNodePath(std::noderef_t);

			// File stuff
			static std::Stack<std::string_element> *openVirtualFile(std::noderef_t, std::Stack<std::string_element> &, std::VOLUME *, std::PATH &);
			static std::Stack<std::string_element> *readFileFromDisk(std::noderef_t, std::PATH &);

			// Query if file is on disk or not
			static bool isFileOnDisk(std::PATH &);

			// Query if path is of type file
			static bool isPathOfTypeFile(std::PATH &);

			// Query if path is of type directory
			static bool isPathOfTypeDirectory(std::PATH &);

			// Does anything even exist at path location
			static bool isPathRealObject(std::PATH &);

			// Query file size for file on disk
			static std::size_t getFilesizeFromDisk(std::PATH &);

			// Read file from disk (this is to stream the entire contents of a file into memory and then dump it)
			static std::size_t readFileFromDisk(std::stream_t, std::PATH &);

			// Write file to disk (Takes all the data from the file and dumps it into a file driver)
			static void writeFileToDisk(std::PATH &, std::stream_t);

			// Inspect a given path. Returns 0 if success, 
			static std::errno_t Inspect(std::PATH &);

			// Step iterator
			static bool stepInspectionIterator();

			// Get name of path
			static void getInspectorPath(std::string &);

			// Get inspector file type
			static std::FileSysInfo::FileSysEntryType getInspectorFileType();

			// Check if can step
			static bool canStepInspector();

			// Time related stuff
			static std::time_t millis();
			static std::time_t micros();

			// Keyboard related stuff
			static std::string_element getKey();
			static std::string_element getKeyAsync();

			// Screen related stuff
			static void printString(std::string &);
			static void clearScreen();

			// RAE
			static userspace::StateMachine &getStateMachine();
			static userspace::HyperVisor &getHypervisor(std::index_t);

			// Syscalls
			static std::errno_t syscall(syscall_t call, syscall_args_t &args);

			// Screen metadata
			static system::DVID_t getScreenDVID();
			static system::HWID_t getScreenHWID();


		};


	}

}

#endif