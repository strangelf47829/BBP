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
	if(argc == 1)
	{
		BBP::Debug::Capture();
		BBP::Debug::SetTerminalColor(30);
		BBP::Debug::SetTerminalColor(47);
		printf("[BOOT]");
		BBP::Debug::SetTerminalColor(37);
		BBP::Debug::SetTerminalColor(40);
		printf(" No boot volume specified. Defaulting to 'V:\\BOOT\\OS.lua'.\n");
		BBP::Debug::Restore();
		status = kernel_entry("V:\\BOOT\\OS.lua");
	}
	else if (argc == 3)
	{
		if((argv[1])[0] != '-' || (argv[1])[1] != 'p')
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

		sprintf(buff, "V:\\usr\\bin\\%s",argv[1]);

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
