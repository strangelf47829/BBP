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

#define BBP_DEBUG
#include "../include/Kernel.h"


#include <cstdio>
#include <cstdlib>

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
		BBP::Debug::Capture();
		BBP::Debug::SetTerminalColor(30);
		BBP::Debug::SetTerminalColor(47);
		printf("[BOOT]");
		BBP::Debug::SetTerminalColor(37);
		BBP::Debug::SetTerminalColor(40);
		printf(" No boot volume specified. Defaulting to " BBP_KERNEL_BOOTLOCATION ".\n");
		BBP::Debug::Restore();
		status = kernel_entry(BBP_KERNEL_BOOTLOCATION);
	}
	else if (argc == 3)
	{
		if ((argv[1])[0] != '-' || (argv[1])[1] != 'p')
		{
			printf("Unkown flag '%s'.\n", argv[1]);
			return 0xFFFF;
		}

		BBP::Debug::Capture();
		BBP::Debug::SetTerminalColor(30);
		BBP::Debug::SetTerminalColor(47);
		printf("[BOOT]");
		BBP::Debug::SetTerminalColor(37);
		BBP::Debug::SetTerminalColor(40);
		printf(" Full path specified. Booting to %s\n", argv[2]);
		BBP::Debug::Restore();
		status = kernel_entry(argv[2]);
	}
	else
	{

		char buff[48];

		sprintf(buff, "V:\\usr\\bin\\%s", argv[1]);

		BBP::Debug::Capture();
		BBP::Debug::SetTerminalColor(30);
		BBP::Debug::SetTerminalColor(47);
		printf("[BOOT]");
		BBP::Debug::SetTerminalColor(37);
		BBP::Debug::SetTerminalColor(40);
		printf(" Booting to %s.\n", buff);
		BBP::Debug::Restore();
		status = kernel_entry(argv[2]);
	}


	BBP::Graphics::Driver::destructGraphics();
	BBP::Services::Interrupts::terminateInterrupts();


	BBP::Debug::Capture();
	BBP::Debug::SetTerminalColor(30);
	BBP::Debug::SetTerminalColor(47);
	printf("[BOOT]");
	BBP::Debug::SetTerminalColor(37);
	BBP::Debug::SetTerminalColor(40);
	printf(" Kernel terminated. Services terminated. Exited with code 0x%06x\n", status);
	BBP::Debug::Restore();

	return status;
}