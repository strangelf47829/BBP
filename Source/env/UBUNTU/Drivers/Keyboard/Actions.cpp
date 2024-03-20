#include "../../include/KeyboardDriver.h"
#include "../../include/Environment.h"

BBP::std::word Environment::Drivers::keyboard::sendDataToKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing
	return 0; // Wrote 0 bytes
}

BBP::std::word Environment::Drivers::keyboard::receiveDataFromKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Get pointer to active stdin
	BBP::std::Stack<BBP::std::string_element> *stdin = &BBP::system::kernelSS()->getKernelSystemContext()->STDIN;

	// If it is nullptr, read 0 bytes
	if (stdin == nullptr)
		return 0; // Read 0 bytes

	// Store size before write to calculate difference later
	BBP::std::size_t preWrite = keyboardStack.atElement;

	// Write data from keyboard stack into STDIN
	keyboardStack >>= BBP::system::kernelSS()->getKernelSystemContext()->STDIN;

	// Calculate difference
	BBP::std::size_t wroteBytes = preWrite - keyboardStack.atElement;

	// Return that amount
	return wroteBytes;
}

BBP::std::word Environment::Drivers::keyboard::receiveKeyboardMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &)
{
	// Does nothing.
	return 0; // Wrote 0 bytes
}