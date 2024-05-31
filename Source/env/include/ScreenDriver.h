#ifndef ENV_DRIVERS_SCREENDRIVER_H
#define ENV_DRIVERS_SCREENDRIVER_H

#include "../../include/DeviceDriver.h"
#include "../../include/drvcmd.h"

namespace Host
{
	namespace Drivers
	{
		namespace Screen
		{
			// Load the screen driver (and get HWID-DVID)
			void loadScreenDriver(BBP::system::DeviceDriver &driver);

			// Data sent to the screen gets print. Data retrieval may be used in the future.
			BBP::std::word sendDataToScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Prints whatever is in the stack. Usually this is STDOUT.
			BBP::std::word receiveDataScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Doesn't do anything
			BBP::std::word receiveScreenMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Doesn't do anything.

			// Connect to a terminal (arguments are determined per platform)
			bool connectToScreen(BBP::std::size_t, BBP::std::word *);
			bool disconnectFromScreen(BBP::std::size_t, BBP::std::word *);

			// Clears the screen
			bool clearScreen(BBP::std::size_t, BBP::std::word *);

			// Print splash screen for screen driver
			bool printSplash(BBP::std::size_t, BBP::std::word *);

			// Print a stack to the screen
			void print_stack_to_string(BBP::std::Stack<BBP::std::string_element> *);

		}
	}
}

#endif