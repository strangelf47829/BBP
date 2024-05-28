#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

#include <cstdio>
#include <termios.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

BBP::std::word Environment::Drivers::screen::sendDataToScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// Read string length
	std::size_t strLength = BBP::std::strlen(page);

	// 'printf'
	std::size_t bytesSent = ::write(STDOUT_FILENO, page.data, strLength);

	return bytesSent;
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