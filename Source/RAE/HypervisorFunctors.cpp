#include "../include/Hyperv.h"
#include "../include/StateMachine.h"
#include "../include/Time.h"

void BBP::userspace::HyperVisor::advanceThread(userspace::StateMachine &state, std::word millis)
{
	// Get current time
	std::time_t now = std::millis();

	// Calculate until when to execute
	std::time_t executeUntil = now + millis;

	// Now execute until that condition is met
	while (std::millis() <= executeUntil)
	{

		// Check if argument stack is empty. If it is, return 
		if (threads.data[activeThread].argumentStack.atElement == 0)
			break;

		// Get the statemachine to advance one cycle.
		state.cycleThread();
	}
	

}