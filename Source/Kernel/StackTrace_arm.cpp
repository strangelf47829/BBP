#include "../include/StackTrace.h"
#include "../include/stddef.h"
#include "../include/stdio.h"

// Only do this for arm architectures
#ifdef ARCH_ARM

// Constructor
BBP::std::stack_trace::stack_trace()
{

}

// Move a stack trace up
bool BBP::std::stack_trace::moveUp()
{
	return false;
}

void BBP::std::stack_trace::printFrame(stack_trace_db &db, address_t referencePhysAddress)
{

}

void BBP::std::stack_trace::Capture()
{

}

void BBP::std::stack_trace::showStackTrace()
{
	// Get a stack_trace_db here
	stack_trace_db db;

	// Get reference address
	std::address_t refAddress = 0x1f2ab;

	// Loop over stacktrace elements
	do
	{
		// Print frame
		printFrame(db, refAddress);

		// If could not step up, break
		if (moveUp() == false)
			break;

	} while (1);


}


#endif