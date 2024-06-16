#include "../../../include/KeyboardDriver.h"
#include "../../../include/Bootloader.h"

// Create page for input
BBP::std::STATIC_PAGE<BBP::std::string_element, 128> inputPage;

// Create input stack
BBP::std::Stack<BBP::std::string_element> inputStack = &inputPage;

bool Host::Drivers::Keyboard::connectKeyboard(BBP::std::size_t, BBP::std::word *)
{
	// Start capturing input (from BIOS)
	BBP::BIOS *bios = Host::getBIOS();
	
	// If no BIOS available return false (how was this function even called??)
	if (bios == nullptr)
		return false;

	// Otherwise capture already started

	return true;
}

bool Host::Drivers::Keyboard::disconnectKeyboard(BBP::std::size_t, BBP::std::word *)
{
	// Stop capturing input (from BIOS)
	BBP::BIOS *bios = Host::getBIOS();

	// If no BIOS available return false (how was this function even called??)
	if (bios == nullptr)
		return false;

	// Otherwise capture already stopped

	return true;
}

bool Host::Drivers::Keyboard::keyCount(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If no arguments, return false
	if (argc == 0 || argv == nullptr)
		return false;

	// Poll
	pollKey(0, nullptr);

	// Return stack
	argv[0] = inputStack.atElement;
	return true;
}

bool Host::Drivers::Keyboard::captureKey(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If no arguments, return false
	if (argc == 0 || argv == nullptr)
		return false;

	// If no elements available, return error
	if (inputStack.atElement == 0)
		return false;

	// Declare element and read
	BBP::std::string_element element;
	inputStack >> element;
	argv[0] = element;

	return true;
}

bool Host::Drivers::Keyboard::pollKey(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Get BIOS
	BBP::BIOS *bios = Host::getBIOS();

	// If none present, false
	if (bios == nullptr)
		return false;

	// If stack at max, do nothing
	if (inputStack.atElement >= inputStack.max_elements)
		return false;

	// Declare key
	BBP::std::string_element key;

	// If something available
	bool captured = bios->getKeyboardInput(key);

	// If Captured, push to stack
	if (captured)
		inputStack << key;

	// If argc and argv set, set captured
	if (argc == 1 && argv != nullptr)
		argv[0] == !!(captured);

	// Success
	return true;
}