#include "../../../include/KeyboardDriver.h"

BBP::std::word Host::Drivers::Keyboard::sendDataToKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	return 0;
}

BBP::std::word Host::Drivers::Keyboard::receiveDataFromKeyboard(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &page)
{
	// Get amount of keyboard keys queued up
	BBP::std::size_t queued;
	keyCount(1, &queued);

	// Get amount to be read ('amount' or 'queued', whichever is less)
	BBP::std::size_t toRead = (queued < amount) ? queued : amount;

	// While that is true;
	for (BBP::std::index_t idx = 0; idx < toRead; idx++)
	{
		// Declare key
		BBP::std::word key;

		// Read
		captureKey(1, &key);

		// Set
		page[idx] = key;
	}

	// Return amount of keys read
	return toRead;
}

BBP::std::word Host::Drivers::Keyboard::receiveKeyboardMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &page)
{
	return 0;
}