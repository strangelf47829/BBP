#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"

bool Environment::Drivers::keyboard::connectKeyboard(BBP::std::size_t, BBP::std::word *)
{
	// Return true, since nothing is done
	return true;
}

bool Environment::Drivers::keyboard::disconnectKeyboard(BBP::std::size_t, BBP::std::word *)
{
	// Return true, since nothing is done
	return true;
}

bool Environment::Drivers::keyboard::enableKeyboardInput(BBP::std::size_t, BBP::std::word *)
{
	captureInput();
	return true;
}

bool Environment::Drivers::keyboard::disableKeyboardInput(BBP::std::size_t, BBP::std::word *)
{
	stopCapturingInput();
	return true;
}