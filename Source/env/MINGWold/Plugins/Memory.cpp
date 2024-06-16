#define USE_32_BITS
#include "../../../include/Resources.h"
#include <stdlib.h>

void *BBP::std::ext_calloc(std::size_t count, std::size_t size)
{
	return ::calloc(count, size);
}

void *BBP::std::ext_malloc(std::size_t size)
{
	return ::malloc(size);
}

void BBP::std::ext_free(void *ptr)
{
	::free(ptr);
}