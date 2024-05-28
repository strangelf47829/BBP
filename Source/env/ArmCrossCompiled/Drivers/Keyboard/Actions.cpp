#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"

BBP::std::word Environment::Drivers::keyboard::sendDataToKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing
	return 0; // Wrote 0 bytes
}

BBP::std::word Environment::Drivers::keyboard::receiveDataFromKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	return 0;
}

BBP::std::word Environment::Drivers::keyboard::receiveKeyboardMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0; // Wrote 0 bytes
}