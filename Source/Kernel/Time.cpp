#include "../include/Kernel.h"
#include "../include/drvcmd.h"


BBP::std::time_t BBP::system::Kernel::millis()
{
	// Core implemented
	return singleton.Core().firmware->millis();
}

BBP::std::time_t BBP::system::Kernel::micros()
{
	// Core implemented
	return singleton.Core().firmware->micros();
}