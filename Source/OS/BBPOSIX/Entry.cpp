#include "../BIOSServices.h"
#include "../../include/UEFI.h"
#include "../../include/DriverCommands.h"
#include "../../include/Kernel.h"
#include "../../include/Daemon.h"
#include "include/Taskpool.h"

bool OS::BIOS::BBPOSIX::EntryPoint(BBP::system::UEFI *uefi, BBP::std::TaskPool *&pool)
{
	// Set taskpool
	pool = &OS::Scheduler::schedulerPool;

	return true;
}