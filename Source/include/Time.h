#ifndef BBP_STDLIB_TIME_H
#define BBP_STDLIB_TIME_H

#include "stdint.h"

namespace BBP
{
	namespace std
	{

		/* The definition used to track time. */
		typedef uint32_t time_t;


		/* Returns the amount of milliseconds since startup */
		time_t millis();

		/* Waits a specified amount of milliseconds */
		void delay(time_t);

		/* Get the current time */
		time_t now();

		/* Called to initialize timers and shit */
		void initClock();


	}
}


#endif