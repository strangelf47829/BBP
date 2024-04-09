#include "include/Environment.h"
#include <cstdio>

// Call bootloader main
int main()
{
	int result = BBP::system::bootloaderMain();
	//printf("\e[0;34m<Bootloader>\e[0;37m Exited with code: %d.\n", result);
	return result;
}