/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/

#define SDL_MAIN_HANDLED

#define BBP_C_ENTRY
#include "Kernel.h"
#include "Localization.h"
#include "SDL.h"
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
	KERNEL_STATUS status;
	if (argc == 1)
	{
		printf("[BOOT]");
		printf(" No boot volume specified. Defaulting to " BBP_KERNEL_BOOTLOCATION ".\n");
		status = kernel_entry(BBP_KERNEL_BOOTLOCATION);
	}
	else if (argc == 3)
	{
		if ((argv[1])[0] != '-' || (argv[1])[1] != 'p')
		{
			printf("Unkown flag '%s'.\n", argv[1]);
			return 0xFFFF;
		}

		printf("[BOOT]");
		printf(" Full path specified. Booting to %s\n", argv[2]);
		status = kernel_entry(argv[2]);
	}
	else
	{

		char buff[48];

		sprintf(buff, "V:\\usr\\bin\\%s", argv[1]);
		printf("[BOOT]");
		printf(" Booting to %s.\n", buff);
		status = kernel_entry(argv[2]);
	}


	BBP::Graphics::Driver::destructGraphics();
	BBP::Services::Interrupts::terminateInterrupts();


	printf("[BOOT]");
	printf(" Kernel terminated. Services terminated. Exited with code 0x%06x\n", status);

	return status;
}