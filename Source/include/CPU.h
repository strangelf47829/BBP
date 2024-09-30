#ifndef BBP_HW_CPU_H
#define BBP_HW_CPU_H

#include "Strings.h"
#include "Time.h"

namespace BBP
{
	namespace system
	{
		// Object used to interface with CPU
		class CPUInterface
		{

		};

		// Hardware information for CPU
		struct CPU_HW
		{
			// Interface to interact with CPU (nullptr if not priveliged to receive information).
			CPUInterface *_interface;

			// Time of data being written
			std::time_t timeUpdated;

			// Speed information
			std::word currentSpeed;
			std::word maxSpeed;

			// Thermal information
			std::byte currentTemp;
			std::byte maxTemp;

			// Other information
			


			// Static string for vendor information and stuff
		};
	}
}

#endif
