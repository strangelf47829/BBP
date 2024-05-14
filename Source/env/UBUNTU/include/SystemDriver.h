#ifndef BBP_SYSTEM_H
#define BBP_SYSTEM_H

#include "../include/Environment.h"

namespace Environment
{
	namespace Drivers
	{
		namespace System
		{

			// Keyboard specific stuff
			BBP::std::word sendDataToSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Doesn't actually do anything
			BBP::std::word receiveDataFromSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Moves stuff from keyboard stack into active STDIN
			BBP::std::word receiveSystemMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Doesn't do anything.

			// Load driver
			void loadSystemDriver(BBP::system::DeviceDriver &driver);

			// Keyboard commands
			bool connectSystem(BBP::std::size_t, BBP::std::word *);			// 0
			bool disconnectSystem(BBP::std::size_t, BBP::std::word *);		// 1


		}
	}
}

#endif
