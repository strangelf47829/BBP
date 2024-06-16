#ifndef BBP_SCREEN_H
#define BBP_SCREEN_H

#include "../include/Environment.h"

namespace Environment
{
	namespace Drivers
	{
		namespace screen
		{

			void loadScreenDriver(BBP::system::DeviceDriver &driver);

			BBP::std::word sendDataToScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Prints whatever is in the stack. Usually this is STDOUT.
			BBP::std::word receiveDataScreen(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Doesn't do anything
			BBP::std::word receiveScreenMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Doesn't do anything.

			bool clearScreen(BBP::std::size_t, BBP::std::word *);
			bool connectToScreen(BBP::std::size_t, BBP::std::word *);

			extern BBP::std::Stack<BBP::std::string_element> stdoutStack;

			void print_stack_to_string(BBP::std::Stack<BBP::std::string_element> *);

		}
	}
}


#endif
