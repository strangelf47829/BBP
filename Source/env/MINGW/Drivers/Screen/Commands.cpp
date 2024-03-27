#include "../../include/ScreenDriver.h"
#include "../../include/Environment.h"

bool Environment::Drivers::screen::clearScreen(BBP::std::size_t, BBP::std::word *)
{
	// Tell R2D to do shit

	// Return success
	return true;
}

bool Environment::Drivers::screen::connectToScreen(BBP::std::size_t, BBP::std::word *)
{
	// Initialize R2D shit

	// Always works
	return true;
}