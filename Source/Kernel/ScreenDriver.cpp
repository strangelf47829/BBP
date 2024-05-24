#include "../include/Kernel.h"

BBP::system::DVID_t BBP::system::Kernel::getScreenDVID()
{
	return singleton.screenDriver.hardwareDriver.dvid;
}

BBP::system::HWID_t BBP::system::Kernel::getScreenHWID()
{
	return singleton.screenDriver.hardwareDriver.hwid;
}