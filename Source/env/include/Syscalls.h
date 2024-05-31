#ifndef ENV_SYSCALLS_H
#define ENV_SYSCALLS_H

#include "../../include/SyscallArgs.h"
#include "../../include/SyscallList.h"

namespace Host
{
	namespace syscalls
	{
		// Memory related system calls
		void *sys_malloc(BBP::std::size_t);
		void *sys_calloc(BBP::std::size_t, BBP::std::size_t);
		void sys_free(void *);
	}
}

#endif