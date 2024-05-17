#ifndef BBP_SYSCALLS_LIST_H
#define BBP_SYSCALLS_LIST_H

#include "SyscallArgs.h"

// A list of system calls.
namespace BBP
{
	namespace systemcalls
	{
		// Memory allocation
		constexpr system::syscall_t system_malloc = 0;
		constexpr system::syscall_t system_calloc = 1;
		constexpr system::syscall_t system_free = 2;
	}
}

#endif