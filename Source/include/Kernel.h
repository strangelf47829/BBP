#ifndef BBP_SYSTEM_KERNEL_H
#define BBP_SYSTEM_KERNEL_H

#include "Hyperv.h"
#include "StateMachine.h"
#include "SystemContext.h"


namespace BBP
{

	namespace system
	{

		class KernelSubSystems
		{

			// State machine for the kernel subsystem
			userspace::StateMachine state;

			// Hypervisor page
			std::PAGE<userspace::HyperVisor> hypervisors;

			// Just for fun
			SystemContext kernelContext;
		
		public:

			// Constructor sets state ready for the bootloader.
			KernelSubSystems();

			// Get pointer to kernel context
			SystemContext* getKernelSystemContext();

			// The context currently being used. If neither hypervisor is being used (I.E., none of them are currently consuming resources), this should be set to kernelContext.
			SystemContext *activeContext;

		
		};

		KernelSubSystems *kernelSS();


		// Kernel functions





	}

}

#endif