#include "../include/Kernel.h"
#include "../include/Daemon.h"
#include "../include/stdctrl.h"

// Statically allocated stuff
BBP::system::Kernel kernel;

BBP::system::Kernel &BBP::system::getKernelInstance()
{
	return kernel;
}

// Constructor
BBP::system::Kernel::Kernel()
	: keyboardDriver(nullptr, nullptr, "Generic Keyboard", keyboardActions, keyboardCMDCount, keyboardCMD),
	screenDriver(nullptr, nullptr, "Generic TTY", screenActions, screenCMDCount, screenCMD),
	fileDriver(nullptr, nullptr, "Generic FileSystem", fileSystemActions, fileSystemCMDCount, fileSystemCMD)
{

}

BBP::system::SystemContext *BBP::system::Kernel::getCurrentSystemContext()
{
	return subsystems.activeContext;
}

BBP::std::index_t BBP::system::Kernel::findFirstEmptyFrame()
{
	// This value is what is returned.
	std::index_t empty = subsystems.processes.dataSize;

	// Check if marked index really is lowest
	if (subsystems.processes[emptyFrame].state == procFrame::unallocated)
		empty = emptyFrame;
	else
	{
		// Frame has been allocated. Look for index higher than empty
		for (std::index_t idx = emptyFrame; idx < subsystems.processes.dataSize; idx++)
		{
			// If not unallocated, continue
			if (subsystems.processes[idx].state != procFrame::unallocated)
				continue;

			// Allocate that shit
			empty = idx;
			break;
		}

		// If still not allocated, throw exception
		if (empty = subsystems.processes.dataSize)
			std::exception("Out of unallocated procframes.", EBUSY);
	}

	return empty;
}

void BBP::system::Kernel::AllocateFrame(std::index_t idx)
{
	subsystems.processes[idx].state = procFrame::allocated;
}

BBP::system::procFrame *BBP::system::Kernel::allocateDaemonFrame(DaemonRecord *rec, std::string str)
{
	// Get first empty frame
	std::index_t frameIndex = findFirstEmptyFrame();

	// Allocate that frame
	AllocateFrame(frameIndex);

	// Fetch frame
	procFrame *frame = &subsystems.processes[frameIndex];

	// Set timer
	frame->initiatedAt = std::millis();

	// Set type
	frame->identifier.type = procFrame::procType::daemon;

	// Set PID
	frame->identifier.id = getNextDaemonPID();

	// Set thread owner
	frame->identifier.owner = &getActiveUser();

	// This process owns itself
	frame->owner = frame->identifier;

	// Set active system context
	frame->systemContext = subsystems.activeContext;

	// Create process context
	frame->processContext = system::ProcessContext(*frame->systemContext, "/home/kangel/", "/home/kangel/");

	// Now set daemon specific stuff
	frame->record = rec;
	frame->spawnedBy = str;

	// Return frame
	return frame;
}


bool BBP::system::Kernel::executeFrame(procFrame *frame)
{
	// If frame is nullptr, stop execution
	if (frame == nullptr)
		return false;

	// Surround entire switch-statement in try-catch block since error may occur anywhere
	try
	{

		// Switch daemon type
		switch (frame->identifier.type)
		{
		case procFrame::daemon:
		{
			// Check if record exists
			if (frame->record == nullptr)
				return false;

			// Check if record has owner
			if (frame->record->owner == nullptr)
				return false;

			// Get service
			DaemonService *service = frame->record->owner->lookupFunctor(frame->spawnedBy);

			// If service does not exist, exit
			if (service == nullptr)
				return false;

			// Set execution status
			frame->state = procFrame::procState::executing;

			// Store old active frame
			procFrame *oldActiveFrame = subsystems.activeContext->activeFrame;

			// Override frame
			subsystems.activeContext->activeFrame = frame;

			// Execute functor
			frame->record->owner->functorFunctor(service->hash, 0, nullptr);

			// Restore old frame
			subsystems.activeContext->activeFrame = oldActiveFrame;

			break;
		}

		default:
			// Unkown type
			throw std::exception("Unkown proc frame type.", EINVAL);
		}
	}
	catch (BBP::std::ProcSignal &signal)
	{
		std::printf("Stopped frame: Caught signal.\n");
	}

	return true;
}
