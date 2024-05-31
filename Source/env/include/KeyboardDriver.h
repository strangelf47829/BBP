#ifndef ENV_DRIVERS_KEYBOARDDRIVER_H
#define ENV_DRIVERS_KEYBOARDDRIVER_H

#include "../../include/DeviceDriver.h"
#include "../../include/drvcmd.h"

namespace Host
{
	namespace Drivers
	{
		namespace Keyboard
		{

			// Keyboard specific stuff
			BBP::std::word sendDataToKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Not assigned use yet
			BBP::std::word receiveDataFromKeyboard(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Read appropriate values from keyboard
			BBP::std::word receiveKeyboardMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Not assigned use yet

			// Load driver
			void loadKeyboardDriver(BBP::system::DeviceDriver &driver);

			// Keyboard commands
			bool connectKeyboard(BBP::std::size_t, BBP::std::word *);			// 0
			bool disconnectKeyboard(BBP::std::size_t, BBP::std::word *);		// 1

			// Capture keys
			bool keyCount(BBP::std::size_t, BBP::std::word *);					// 2
			bool captureKey(BBP::std::size_t, BBP::std::word *);				// 3
			bool pollKey(BBP::std::size_t, BBP::std::word *);					// 4 (Used to poll external input methods -- usually hooked to a task)

		}
	}
}

#endif