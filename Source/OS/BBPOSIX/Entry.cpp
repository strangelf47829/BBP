#include "../BIOSServices.h"
#include "../../include/EFI.h"
#include "../../include/DriverCommands.h"
#include "../../include/Kernel.h"
#include "../../include/Daemon.h"
#include "include/Taskpool.h"
#include "../../include/StackTrace.h"



bool OS::BBPOSIX::EntryPoint(BBP::system::EFI *EFI, BBP::std::TaskPool *&pool)
{
	// Set taskpool
	pool = &OS::Scheduler::schedulerPool;

	// Show stack
	BBP::std::stack_trace st;

	st.Capture();
	st.showStackTrace();

	return true;
}