#include "../../../include/KeyboardDriver.h"

BBP::std::word Host::Drivers::Keyboard::sendDataToKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	return 0;
}

BBP::std::word Host::Drivers::Keyboard::receiveDataFromKeyboard(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// Poll keys
	Host::Drivers::Keyboard::pollKey(0, nullptr);

	// Get amount of keys that input
	BBP::std::size_t queued = 0;
	Host::Drivers::Keyboard::keyCount(1, &queued);

	// Then get the amount to read -- which is 'amount' or 'queued', whichever is less
	BBP::std::size_t toRead = (queued < amount) ? queued : amount;

	// Then copy
	for (BBP::std::index_t idx = 0; idx < toRead; idx++)
	{
		// Declare key that will get captured
		BBP::std::word captured = 0;
		
		// Capture a key -- if error break out
		if (captureKey(1, &captured))
			return idx;

		// Write
		page[idx] = captured;
	}

	// Return amount of keys read
	return toRead;
}

BBP::std::word Host::Drivers::Keyboard::receiveKeyboardMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	return 0;
}