#include "../BIOSServices.h"
#include "../../include/EFI.h"
#include "../../include/DriverCommands.h"
#include "../../include/Kernel.h"
#include "../../include/Daemon.h"
#include "include/Taskpool.h"
#include "../../include/StackTrace.h"
#include "../../include/Daemon.h"


bool OS::BBPOSIX::EntryPoint(BBP::system::EFI *EFI, BBP::std::TaskPool *&pool)
{
	// Set taskpool
	pool = &OS::Scheduler::schedulerPool;

	// Create errno
	BBP::std::errno_t error = ENONE;

	// Array of arguments for shell initialisation
	BBP::std::c_string initArgs[1] = { (BBP::std::c_string)EFI->system.volumePath };

	// Configure Shell Primary Volume, which should be stored in the EFI record
	BBP::system::DaemonRecord::executeService("initd", "shell-init", error, 1, initArgs);

	// Then Start shell
	BBP::system::DaemonRecord::executeService("initd", "shell-app", error, 0, nullptr);

	return true;
}
