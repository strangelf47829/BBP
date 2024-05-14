#include "../../include/SystemDriver.h"
#include "../../include/Environment.h"

BBP::std::word Environment::Drivers::System::sendDataToSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// Does nothing.
	return 0;
}

BBP::std::word Environment::Drivers::System::receiveDataFromSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0;
}

BBP::std::word Environment::Drivers::System::receiveSystemMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0;
}