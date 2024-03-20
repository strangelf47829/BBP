#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

bool Environment::Drivers::screen::clearScreen(BBP::std::size_t, BBP::std::word *)
{
	// The string that actually clears the string
	BBP::std::string clearScreenString = BBP::std::String("\033[2J\033[1;1H");

	// print
	BBP::std::word dataSent = BBP::std::printf("%s", clearScreenString.data);

	// Return success
	return dataSent > 0;
}

bool Environment::Drivers::screen::connectToScreen(BBP::std::size_t, BBP::std::word *)
{
	// Always works
	return true;
}