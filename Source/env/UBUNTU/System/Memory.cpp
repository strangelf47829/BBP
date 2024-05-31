#include "../../include/Syscalls.h"
#include <cstdlib>

// Memory system calls
void *Host::syscalls::sys_malloc(BBP::std::size_t size)
{
	return malloc(size);
}
void *Host::syscalls::sys_calloc(BBP::std::size_t count, BBP::std::size_t size)
{
	return calloc(count, size);
}
void Host::syscalls::sys_free(void *ptr)
{
	free(ptr);
}