#include "../include/Kernel.h"

BBP::system::Kernel BBP::system::Kernel::singleton;

BBP::system::Kernel::Kernel()
	: taskpool(taskpoolData),
	core()
{
}

BBP::system::KernelCore &BBP::system::Kernel::Core()
{
	return singleton.core;
}