#define SDL_MAIN_HANDLED

#include "../include/Kernel.h"
#include "../include/Localization.h"
#include "../include/SDL.h"
#include <cstdio>


void BBP::Services::Free(void *ptr)
{
	free(ptr);
}

void *BBP::Services::c_alloc(unsigned long long int count, unsigned long long int size)
{
	return calloc(count, size);
}

void *BBP::Services::m_alloc(unsigned long long int size)
{
	return malloc(size);
}

int main(int argc, char **argv)
{
	KERNEL_STATUS status = kernel_entry();

	printf("Kernel terminated. Services terminated. Exited with code 0x%06x\n", status);

	return status;
}