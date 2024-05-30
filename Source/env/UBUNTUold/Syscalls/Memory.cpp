#include "../include/Syscalls.h"
#include <cstdlib>

// Memory system calls
void *Environment::syscalls::sys_malloc(BBP::std::size_t size)
{
	return malloc(size);
}
void *Environment::syscalls::sys_calloc(BBP::std::size_t count, BBP::std::size_t size)
{
	return calloc(count, size);
}
void Environment::syscalls::sys_free(void *ptr)
{
	free(ptr);
}