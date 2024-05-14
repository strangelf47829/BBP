#include "../../include/SystemDriver.h"
#include "../../include/Environment.h"

bool Environment::Drivers::System::connectSystem(BBP::std::size_t, BBP::std::word *)
{
	// Does nothing
	return true;
}

bool Environment::Drivers::System::disconnectSystem(BBP::std::size_t, BBP::std::word *)
{
	// Always works
	return true;
}