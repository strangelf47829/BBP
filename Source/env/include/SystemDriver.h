#ifndef ENV_DRIVERS_SYSTEMDRIVER_H
#define ENV_DRIVERS_SYSTEMDRIVER_H

#include "../../include/DeviceDriver.h"
#include "../../include/drvcmd.h"

namespace Host
{
	namespace Drivers
	{
		namespace System
		{

			// Send data to and from the system
			BBP::std::word sendDataToSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);		// Doesn't actually do anything
			BBP::std::word receiveDataFromSystem(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Moves stuff from keyboard stack into active STDIN
			BBP::std::word receiveSystemMetadata(BBP::std::size_t, BBP::std::PAGE<BBP::std::string_element> &);	// Doesn't do anything.

			// Load the system driver
			void loadSystemDriver(BBP::system::DeviceDriver &driver);

			// Connect to the system -- expects two arguments -- username and password
			bool connectSystem(BBP::std::size_t, BBP::std::word *);			// 0
			bool disconnectSystem(BBP::std::size_t, BBP::std::word *);		// 1

			// Handle a system call -- Calling format is syscall_args_t
			bool handleSystemCall(BBP::std::size_t, BBP::std::word *);		// 2

			// Retrieve time information from system
			bool initClock(BBP::std::size_t, BBP::std::word *);				// 3
			bool getClockMillis(BBP::std::size_t, BBP::std::word *);		// 4
			bool getClockMicros(BBP::std::size_t, BBP::std::word *);		// 5

			// Get current time (using unix timestamp)
			bool getUnixTimestamp(BBP::std::size_t, BBP::std::word *);		// 6
			bool getStartupTimestamp(BBP::std::size_t, BBP::std::word *);	// 7


		}
	}
}

#endif