#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"

bool Environment::Drivers::keyboard::connectKeyboard(BBP::std::size_t, BBP::std::word *)
{
	// Start capturing input anyway
	captureInput();
	return true;
}

bool Environment::Drivers::keyboard::disconnectKeyboard(BBP::std::size_t, BBP::std::word *)
{
	// Return true, since nothing is done
	stopCapturingInput();
	return true;
}

bool Environment::Drivers::keyboard::keyCount(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If no arguments, return false
	if (argc == 0 || argv == nullptr)
		return false;

	// Otherwise, write keycount
	argv[0] = Environment::Drivers::keyboard::count;

	return true;
}

bool Environment::Drivers::keyboard::captureKey(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If no arguments, return false
	if (argc == 0 || argv == nullptr)
		return false;
	
	// If no keys present, return 0
	if (Environment::Drivers::keyboard::count == 0)
	{
		argv[0] = 0;
		return true;
	}

	// Otherwise, write keycount
	argv[0] = Environment::Drivers::keyboard::keys[--Environment::Drivers::keyboard::count];
	return true;
}