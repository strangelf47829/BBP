#include "../../../include/ScreenDriver.h"
#include "../../include/BIOS.h"
#include "../../include/ScreenData.h"

#define RED "\e[0;31m"
#define NORM "\e[0;37m"
#define PADDING "%s"
#define PROCPADDING "\t"

bool Host::Drivers::Screen::clearScreen(BBP::std::size_t, BBP::std::word *)
{
	// The string that actually clears the string
	BBP::std::string clearScreenString = BBP::std::String("\033[2J\033[1;1H");

	// print
	BBP::std::word dataSent = sendDataToScreen(clearScreenString.dataSize, clearScreenString);

	// Return success
	return dataSent > 0;
}

// Display splash screen: Print small logo
bool Host::Drivers::Screen::printSplash(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Get BIOS
	BBP::BIOS *bios = Host::getBIOS();

	// Print something
	bios->print("+-+ |  " "\n"\
	            "| | +-+" "\n"\
	            "+-+ +-+" "\n");
}

bool Host::Drivers::Screen::connectToScreen(BBP::std::size_t, BBP::std::word *)
{
	// Initialize
	Host::terminal.initialize();

	BBP::std::string str = "Hello, world!";

	// Do some test
	Host::terminal.render(str);

	// Always works
	return true;
}

bool Host::Drivers::Screen::disconnectFromScreen(BBP::std::size_t, BBP::std::word *)
{
	return true;
}