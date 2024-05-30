#include "../BIOSServices.h"
#include "../../include/EFI.h"
#include "../../include/DriverCommands.h"
#include "../../include/stdio.h"

bool OS::WINDOWSE20::EntryPoint(BBP::system::EFI *EFI, BBP::std::TaskPool *&)
{
	BBP::std::printf("This is where Windowse20 would boot.\n");
	return true;
}