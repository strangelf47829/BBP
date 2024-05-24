#include "../include/Time.h"
#include "../include/Kernel.h"

void BBP::std::Delay(std::time_t millis)
{
	// If no delay, do nothing
	if (millis == 0)
		return;

	// Get kernel time
	std::time_t now = system::Kernel::millis();

	// Now wait
	while (system::Kernel::millis() < now + millis);

	// Done!
}