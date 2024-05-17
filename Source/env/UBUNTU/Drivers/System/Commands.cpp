#include "../../include/SystemDriver.h"
#include "../../include/Environment.h"
#include "../../../../include/Syscalls.h"
#include "../../../../include/SyscallArgs.h"
#include "../../../../include/SyscallList.h"
#include "../../include/Syscalls.h"

bool Environment::Drivers::System::connectSystem(BBP::std::size_t, BBP::std::word *)
{
	// Does nothing
	return true;
}

bool Environment::Drivers::System::disconnectSystem(BBP::std::size_t, BBP::std::word *)
{
	// Always works
	return true;
}

bool Environment::Drivers::System::handleSystemCall(BBP::std::size_t systemcall_, BBP::std::word *args_ptr)
{
	// Convert stuff
	BBP::system::syscall_t call = systemcall_;
	BBP::system::syscall_args_t *args = (BBP::system::syscall_args_t *)args_ptr;

	// Then handle system call
	switch (systemcall_)
	{
	case BBP::systemcalls::system_malloc:
		
		// Simple malloc
		BBP::system::getOutValue<void *>(*args, 0) = Environment::syscalls::sys_malloc(BBP::system::getInValue<BBP::std::size_t>(*args, 0));
		
		return true;
	case BBP::systemcalls::system_calloc:

		// Simple calloc
		BBP::system::getOutValue<void *>(*args, 0) = Environment::syscalls::sys_calloc(BBP::system::getInValue<BBP::std::size_t>(*args, 0), BBP::system::getInValue<BBP::std::size_t>(*args, 1));
		
		return true;
	case BBP::systemcalls::system_free:

		// Simple free
		Environment::syscalls::sys_free(BBP::system::getInValue<void *>(*args, 0));

	default:
		
		return false;
	}
}