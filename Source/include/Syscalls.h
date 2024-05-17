#ifndef BBP_STD_SYSCALLS_H
#define BBP_STD_SYSCALLS_H

#include "stddef.h"
#include "errno.h"
#include "Memory.h"

// This header defines the interactions between the userspace side (So the application layer) with the system driver.

namespace BBP
{
	// List of system calls
	namespace systemcalls
	{
		// Memory calls
		void *malloc(std::size_t);
		void *calloc(std::size_t, std::size_t);
		void free(void *);


	}
}

#endif