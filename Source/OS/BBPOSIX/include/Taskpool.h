#ifndef BBPOSIX_SCHEDULER_H
#define BBPOSIX_SCHEDULER_H

#include "../../../include/TaskPool.h"

namespace OS
{
	// This class handles all things related to scheduling the tasks carried out by this OS.
	// This includes background tasks (IO related stuff too), RAE stepping, etc...
	// Pretty bareboned since BBPOSIX Doesn't need any fancy handling
	class Scheduler
	{
		
	public:

		// This runs things like I/O Events and such (also just run something in general pls)
		static BBP::std::TaskPool schedulerPool;

	};
}

#endif