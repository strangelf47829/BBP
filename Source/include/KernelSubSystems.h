#ifndef BBP_SYSTEM_KERNELSUBSYSTEMS_H
#define BBP_SYSTEM_KERNELSUBSYSTEMS_H

#include "Hyperv.h"
#include "StateMachine.h"
#include "SystemContext.h"
#include "SysConfig.h"
#include "DeviceDriver.h"
#include "UEFI.h"
#include "stdctrl.h"

// The kernel subsystem class handles with the state of the kernel, rather than performing tasks.
// This includes for example, keeping track of active proc frames, or whatever else.

namespace BBP
{
	namespace system
	{

		// This struct holds information about users.
		struct UserInformation
		{
			// Name of the user
			std::string username;
			std::hash_t usernameHash;

			// Password of the user
			std::string password;
			std::hash_t passwordHash;

			// How the user connected onto the system
			std::string connectionMethod;	// (Usually just the name of the device, however if connected over SSH, this could be SSH).

			// Permissions 
			bool isRoot;
		};

		// Using PID_t 
		using pid_t = std::word;

		// A proc-frame is a 'frame' that stores information about an executing 'process' (such that of a daemon, or user initiated command).
		// The use case for these procframes is
		struct procFrame
		{
			// Time frame was initiated
			std::time_t initiatedAt;

			// Stores the type of process that started this process
			enum procType
			{
				daemon,		// If the process is a daemon
				bin,		// If the process spawned from a binary
				script		// If the process is a shell script
			};

			// Stores the state of this frame
			enum procState
			{
				unallocated,	// This frame is not doing anything
				allocated,		// This frame is waiting to be given a task
				queued,			// This frame is queued for execution
				executing,		// This frame is currently executing
				finished,		// This frame is done executing
				zombie,			// This frame is has no owner, and as such cannot continue execution. Either kill, or set new owner
				sleeping		// This frame is not currently executing
			} state;

			// Proc frame identifier
			struct PFID
			{
				// The ID of the process
				pid_t id;

				// Type
				procType type;

				// The user who owns this process
				UserInformation *owner;
			};

			// Own process identifier
			PFID identifier;

			// Owner
			PFID owner;

			// Does it run on a thread?
			std::index_t threadID;

			// Does it run on a hypervisor?
			std::index_t hypervisorID;

			// The system context this process falls under
			SystemContext *systemContext;

			// The process context this process falls under
			ProcessContext processContext;

			union
			{
				DaemonRecord *record;										// If a frame of type daemon, this is the record for the daemon.
				struct { userspace::Thread *t; userspace::HyperVisor *v; };	// If a frame of type binary, this is the pointer to the thread and hypervisor.
				std::string *cmd;											// If a frame of type shell, this is the string that spawned the command.
			};

			// This string holds the entire shell line (or just string thingy) when done.
			std::string spawnedBy;
		};

		class KernelSubSystems
		{
		public:

			// State machine for the kernel subsystem
			userspace::StateMachine state;

			// Hypervisor page
			std::PAGE<userspace::HyperVisor> hypervisors;

			// This is the context used for processes without a hypervisor.
			SystemContext kernelContext;

			// The maximum amount of maximum concurrent processes.
			static constexpr std::size_t maxConcurrentProcesses = 128;

			// List of processes
			std::STATIC_PAGE<procFrame, maxConcurrentProcesses> processes;			

			// Constructor sets state ready for the bootloader.
			KernelSubSystems();

			// Get pointer to kernel context
			SystemContext *getKernelSystemContext();

			// The context currently being used. If neither hypervisor is being used (I.E., none of them are currently consuming resources), this should be set to kernelContext.
			SystemContext *activeContext;

		};


	}
}

#endif