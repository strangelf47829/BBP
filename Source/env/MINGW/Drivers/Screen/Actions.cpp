#include "../../../include/ScreenDriver.h"
#include "../../include/ScreenData.h"

BBP::std::word Host::Drivers::Screen::sendDataToScreen(BBP::std::size_t req, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// 'printf'
	Host::terminal.render(page);
	return req;
}

BBP::std::word Host::Drivers::Screen::receiveDataScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0;
}

BBP::std::word Host::Drivers::Screen::receiveScreenMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0;
}