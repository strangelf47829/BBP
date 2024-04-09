#ifndef BBP_STD_INTERRUPTS_H
#define BBP_STD_INTERRUPTS_H

#include "stddef.h"
#include "Time.h"
#include "Memory.h"

namespace BBP
{
	namespace std
	{

		// Signature of the arguments
		using interrupt_arg_t = void *;

		// Signature of a function called by the interrupt
		using interrupt_t = void (*)(size_t, interrupt_arg_t *);

		// A structure for an interrupt request
		struct IRQ
		{

			// The function request
			interrupt_t handle;

			// Interrupt urgency
			byte urgency;

			// Argument count
			byte argumentCount;

			// Vector to arguments 
			interrupt_arg_t *argumentVector;

			// Flag specifies if IRQ should be treated as timer
			bool isTimed;

			// Flag handles request lifespan
			bool removeFromQueue;

			// Amount of milliseconds per request
			time_t refreshRate;
			time_t lastCall;

			// Unique hash for interrupt
			hash_t hash;

		};

		// Create an interrupt
		void createInterrupt(IRQ &, interrupt_t);
		void createInterrupt(IRQ &, interrupt_t, byte, interrupt_arg_t *);

		// Create a timed interrupt
		void createTimedInterrupt(IRQ &, interrupt_t, time_t);
		void createTimedInterrupt(IRQ &, interrupt_t, byte, interrupt_arg_t *, time_t);

		// Destroy an interrupt (or otherwise de-initialize it)
		void deactivateInterrupt(IRQ &);

		// Checks if IRQ is active
		bool isIRQActive(IRQ &);

		// Calculate the hash given a interrupt
		hash_t calculateIRQHash(IRQ &);

		// Interrupt request handler
		class IRQHandler
		{
			public:

				// Check if handler can handle a request
				virtual bool available() = 0;

				// Handle a request
				virtual void handle(IRQ &) = 0;

				// Functions used to handle the backqueue of interrupts
				virtual void updateHandler() = 0;

		};

		// Max amount of IRQ entries in the kernel stuff
		constexpr size_t kernelIRQSize = 32;

		// Kernel IRQ
		class KernelIRQHandler : public IRQHandler
		{

			// Static thing of IRQ entries
			STATIC_PAGE<IRQ, kernelIRQSize> IRQBacklog;

			// Static array marks boolean
			STATIC_PAGE<bool, kernelIRQSize> activeIRQBacklogs;

			// Last time IRQ was updated
			time_t lastUpdate;

			// Cached value
			size_t availableCount;
			bool availableCountValid;

			// Calculate amount of available items
			size_t getAvailableCount();

		public:

			// Override stuff
			bool available();

			// Add stuff to the handler
			void handle(IRQ &);

			// This updates the handler 
			void updateHandler();

		};

	}
}

#endif