#include "../BIOSServices.h"
#include "../../include/UEFI.h"
#include "../../include/DriverCommands.h"
#include "../../include/Kernel.h"
#include "../../include/Daemon.h"

bool OS::BIOS::BBPOSIX::EntryPoint(BBP::system::UEFI *uefi)
{

	// Get kernel instance
	BBP::system::Kernel *kernel = &BBP::system::getKernelInstance();


	// massive try catch
	try {

		// Set active context to kernel context
		kernel->setKernelContext();

		// Set active context memory
		kernel->setSystemProgmem();

		// Set system volume
		kernel->setSystemVolume(uefi->system.volumeLabel, uefi->system.volumePath);

		// Now switch to hypervisors
		kernel->setHypervisorContext(0);

		// Load hypervisors
		for (BBP::std::index_t index = 0; index < kernel->SubSystems().hypervisors.dataSize; index++)
		{
			// Switch to hypervisor
			kernel->setHypervisorContext(index);

			// Set context progmem
			kernel->setSystemProgmem();

			// Allocate string
			BBP::std::static_string<32> pathName;

			// print into buffer
			BBP::std::sprintf(pathName.static_data, "/hyper/%d", index);

			// Set volume
			kernel->setSystemVolume('h', pathName.data);

		}

		// And set volume
		kernel->setSystemVolume('h', "/hyper/0/");

		// Configure root user
		kernel->configureRoot(uefi);

		// Set kernel context
		kernel->setKernelContext();

		// Allocate a new proc frame, spawned from itself.
		BBP::system::procFrame *frame = kernel->allocateDaemonFrame(&BBP::system::DaemonRecord::records[0], BBP::std::String("shell-app"));

		// set working directory to frame stuff
		kernel->SubSystems().activeContext->workingDirectory = &frame->processContext.workingDirectory;

		// Execute frame.
		BBP::std::errno_t result = 0;
		if (kernel->executeFrame(frame))
			result = kernel->getFrameResult(frame);

		return 0;
	}
	catch (BBP::std::KernelSignal &signal)
	{
		// If unwinding was desired, do nothing.
		if (signal.intendedUnwind == false)
			OSPanic(0, "KSignal received.");
	}
	catch (BBP::std::except &e)
	{
		// Try to handle error
		try
		{

			// Get error from stderr
			BBP::std::c_string errStr = 0;
			kernel->SubSystems().activeContext->STDERR >>= &errStr;

			OSPanic(e.code, errStr);
		}
		catch (BBP::std::except &e)
		{
			// If even that fails, panic harder
			OSPanic(0xFFFFFFFF, "(ERROR TRYING TO READ ERRNO MESSAGE)");
		}
	}
	catch (...)
	{
		OSPanic(0xFF00FF00, "System threw object.");
	}

}