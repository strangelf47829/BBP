#include "../include/Interrupts.h"

// Stubs
void BBP::std::createInterrupt(IRQ &irq, interrupt_t function)
{
	createInterrupt(irq, function, 0, nullptr);
}

void BBP::std::createTimedInterrupt(IRQ &irq, interrupt_t function, time_t time)
{
	createTimedInterrupt(irq, function, 0, nullptr, time);
}

// Hashing function
BBP::std::hash_t BBP::std::calculateIRQHash(IRQ &irq)
{
	// If irq isn't pointing to anything, return a 0
	if (irq.handle == nullptr)
		return 0;

	// Otherwise copy number
	word irqHash = *((word *)&irq.handle);

	// Right bitshift with argCount
	irqHash >> irq.argumentCount;

	// Xor it with argument vector
	irqHash ^= *((word *)&irq.argumentVector);

	// Flip the 2nd, 5th, and 9th bit if must be removed from queue
	if (irq.removeFromQueue)
		irqHash ^= 0b0100100001;
}

// Disable IRQ
void BBP::std::deactivateInterrupt(IRQ &irq)
{
	// Set handle to 0
	irq.handle = nullptr;

	// Remove all stuff
	irq.argumentCount = 0;
	irq.argumentVector = nullptr;

	// Zero out hash
	irq.hash = 0;
}

bool BBP::std::isIRQActive(IRQ &irq)
{
	return (irq.hash && irq.handle);
}

// Create irq without timer
void BBP::std::createInterrupt(IRQ &irq, interrupt_t function, byte argCount, interrupt_arg_t *argVector)
{
	// Set function
	irq.handle = function;

	// If argCount is 0 or argVector is nullptr, set empty
	if (argCount == 0 || argVector == nullptr)
	{
		irq.argumentCount = 0;
		irq.argumentVector = nullptr;
	}
	else
	{
		irq.argumentCount = argCount;
		irq.argumentVector = argVector;
	}

	// Set Times to 0
	irq.refreshRate = 0;
	irq.lastCall = 0;

	// set flags to false
	irq.urgency = false;
	irq.removeFromQueue = true;
	irq.isTimed = false;

	// Calculate hash
	irq.hash = calculateIRQHash(irq);
}

// Create irq with timer
void BBP::std::createTimedInterrupt(IRQ &irq, interrupt_t function, byte argCount, interrupt_arg_t *argVector, time_t time)
{
	// Create normal IRQ
	createInterrupt(irq, function, argCount, argVector);

	// Set refresh rate
	irq.refreshRate = time;

	// Set flags
	irq.removeFromQueue = false;
	irq.isTimed = true;

	// Set timer to now
	irq.lastCall = millis();

	// Recalculate hash
	irq.hash = calculateIRQHash(irq);
}