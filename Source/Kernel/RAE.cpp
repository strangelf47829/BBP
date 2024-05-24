#include "../include/Kernel.h"

BBP::userspace::HyperVisor &BBP::system::Kernel::getHypervisor(std::index_t idx)
{
	return singleton.hypervisors[idx];
}

BBP::userspace::StateMachine &BBP::system::Kernel::getStateMachine()
{
	return singleton.machine;
}