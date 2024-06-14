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

	// Show message of the day
	BBP::std::string str = "init-motd";
	BBP::system::DaemonRecord::records[0].owner->functorFunctor(BBP::system::DaemonRecord::records[0].owner->lookupFunctor(str)->hash, 0, nullptr);

	// Start shell
	str = "shell-app";
	BBP::system::DaemonRecord::records[0].owner->functorFunctor(BBP::system::DaemonRecord::records[0].owner->lookupFunctor(str)->hash, 0, nullptr);

	return true;
}