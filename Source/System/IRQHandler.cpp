#include "../include/Interrupts.h"

BBP::std::size_t BBP::std::KernelIRQHandler::getAvailableCount()
{
	// If available count is valid, get that
	if (availableCountValid)
		return availableCount;

	// Keep track of active IRQs
	size_t activeCount = 0;

	// Go over each irq
	for (index_t idx = 0; idx < IRQBacklog.dataSize; idx++)
	{
		// If irq is not active, skip
		if (isIRQActive(IRQBacklog[idx]) == false)
			continue;

		// Increase active count
		activeCount++;
	}

	// Set availableCount
	availableCount = activeCount;

	// Validate cache
	availableCountValid = true;

	// Return result
	return availableCount;
}

// Is available if available count is less than max available count
bool BBP::std::KernelIRQHandler::available()
{
	return getAvailableCount() < IRQBacklog.dataSize;
}

// Create handle
void BBP::std::KernelIRQHandler::handle(IRQ &irq)
{
	// If irq is not active do nothing
	if (isIRQActive(irq) == false)
		return;

	// If not available do nothing
	if (available() == false)
		return;

	// Look for free IRQ
	for (index_t idx = 0; idx < IRQBacklog.dataSize; idx++)
	{
		// If active IRQ, skip
		if (isIRQActive(IRQBacklog[idx]) == false)
			continue;

		// Assign value
		write(&IRQBacklog, irq, idx);

		// Breakout
		break;
	}
}

// The update handle does the following
void BBP::std::KernelIRQHandler::updateHandler()
{
	// Get now
	time_t now = millis();

	// Loop over irqs
	for (index_t idx = 0; idx < IRQBacklog.dataSize; idx++)
	{
		// If IRQ is inactive, do nothing
		if (isIRQActive(IRQBacklog[idx]) == false)
			continue;

		// Get pointer to irq
		IRQ *irq = &IRQBacklog[idx];

		// Call interrupt if this is not a timed irq
		if (irq->isTimed == false)
			irq->handle(irq->argumentCount, irq->argumentVector);
		else
		{
			// Otherwise check if timer has run out. If not, skip this irq
			if (irq->refreshRate > now - lastUpdate)
				continue;

			// Call interrupt
			irq->handle(irq->argumentCount, irq->argumentVector);

			// Update last call
			irq->lastCall = now;
		}

		// If remove from queue, remove from queue
		if (irq->removeFromQueue)
			deactivateInterrupt(IRQBacklog[idx]);

		// Invalidate cache
		availableCountValid = false;

	}

	// Last updated is now
	lastUpdate = now;

}