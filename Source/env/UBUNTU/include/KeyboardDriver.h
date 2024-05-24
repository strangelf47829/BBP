#ifndef BBP_KEYBOARD_H
#define BBP_KEYBOARD_H

#include "../include/Environment.h"

namespace Environment
{
	namespace Drivers
	{
		namespace keyboard
		{

			// Keyboard specific stuff
			BBP::std::word sendDataToKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Doesn't actually do anything
			BBP::std::word receiveDataFromKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Moves stuff from keyboard stack into active STDIN
			BBP::std::word receiveKeyboardMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Doesn't do anything.

			// Load driver
			void loadKeyboardDriver(BBP::system::DeviceDriver &driver);

			// Keyboard commands
			bool connectKeyboard(BBP::std::size_t, BBP::std::word *);			// 0
			bool disconnectKeyboard(BBP::std::size_t, BBP::std::word *);		// 1
			bool keyCount(BBP::std::size_t, BBP::std::word *);					// 2
			bool captureKey(BBP::std::size_t, BBP::std::word *);				// 3

			extern volatile bool captureUserInput;
			extern volatile BBP::std::index_t count;
			extern volatile BBP::std::string_element keys[64];

			void captureInput();
			void stopCapturingInput();

		}
	}
}

#endif