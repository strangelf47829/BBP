#include "../include/Hyperv.h"
#include "../include/StateMachine.h"
#include "../include/Time.h"
#include "../include/Kernel.h"

bool BBP::userspace::HyperVisor::advanceThread(userspace::StateMachine &state, std::word millis)
{
	// Get current time
	std::time_t now = system::Kernel::millis();

	// Calculate until when to execute
	std::time_t executeUntil = now + millis;

	// Now execute until that condition is met
	while (system::Kernel::millis() <= executeUntil)
	{

		// Check if argument stack is empty. If it is, return 
		if (threads[activeThread].isThreadCold())
			break;

		// Get the statemachine to advance one cycle.
		state.cycleThread();
	}

	return threads[activeThread].isThreadCold() == false;
}

bool BBP::userspace::HyperVisor::advanceThread(userspace::StateMachine &state)
{
	// Check for valid thread
	if (activeThread >= threadCount)
		std::exception("Invalid active thread.", EINVAL);

	// Check for active thread
	if (threads.data[activeThread].active == false)
		return false;

	// If thread is cold, stop execution
	if (threads.data[activeThread].argumentStack.atElement == 0)
		return false;

	// Execute once
	state.cycleThread();

	return threads[activeThread].isThreadCold() == false;
}