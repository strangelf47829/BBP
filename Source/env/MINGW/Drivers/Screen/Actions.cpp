#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

#include <cstdio>

BBP::std::word Environment::Drivers::screen::sendDataToScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// Go through each element. Check for ECMA35 stuff.
	return ::std::puts(page.data);
}

BBP::std::word Environment::Drivers::screen::receiveDataScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0;
}

BBP::std::word Environment::Drivers::screen::receiveScreenMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0;
}