#ifndef BBP_STDLIB_TIME_H
#define BBP_STDLIB_TIME_H

#include "stdint.h"

namespace BBP
{
	namespace std
	{

		/* The definition used to track time. */
		typedef uint32_t time_t;

		// Delay action (delays entire system)
		void Delay(time_t millis);

	}
}


#endif