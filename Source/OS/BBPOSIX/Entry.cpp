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

	// Start shell
	BBP::std::errno_t error = ENONE;
	BBP::system::DaemonRecord::executeService("initd", "shell-app", error, 0, nullptr);

	return true;
}