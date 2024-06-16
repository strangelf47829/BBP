#include "../../../include/stdio.h"
#include "../../../include/Kernel.h"
#include "../include/KeyboardDriver.h"

BBP::std::size_t BBP::std::inputBufferCount()
{
	return BBP::system::getKernelInstance().getKeyboardDriver().softwareDriver.available();
}

BBP::std::string_element BBP::std::getChar()
{
	// Wait for an available key
	while (inputBufferCount() == 0 && Environment::Drivers::keyboard::captureUserInput);

	// If no data available, return 0. (Could be the case if capture user input is set to false)
	if (inputBufferCount() == 0)
		return '\0';

	// Retrieve a single character from the stack
	BBP::std::string_element c = 0;
	system::getKernelInstance().getKeyboardDriver().softwareDriver >> c;

	// Return that character
	return c;
}

BBP::std::InputEncoding::KeyCode BBP::std::getKeyCode()
{
	// Wait for an available key
	while (inputBufferCount() == 0 && Environment::Drivers::keyboard::captureUserInput);

	// If no data available, return 0. (Could be the case if capture user input is set to false)
	if (inputBufferCount() == 0)
		return BBP::std::InputEncoding::NULL;

	// If a single key is pressed, return the key as keycode
	if (inputBufferCount() == 1)
		return (BBP::std::InputEncoding::KeyCode)getChar();

	// Otherwise, just return 'up' for now idk
	return InputEncoding::UP;
}

// Async
BBP::std::string_element BBP::std::getCharAsync()
{
	if (inputBufferCount())
		return getChar();
	return 0;
}

// Async
BBP::std::InputEncoding::KeyCode BBP::std::getKeyCodeAsync()
{
	if (inputBufferCount())
		return getKeyCode();
	return InputEncoding::NULL;
}