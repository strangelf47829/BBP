#include "../include/StackTrace.h"
#include "../include/stddef.h"
#include "../include/stdio.h"

// Only do this for x86 architectures
#ifdef ARCH_x86_64

// Constructor
BBP::std::stack_trace::stack_trace()
{

}

// Move a stack trace up
bool BBP::std::stack_trace::moveUp()
{

	// Get pointer to current active frame
	stack_frame::frame_ptr_t currentFrame = activeFrame.frameAddress;

	// Set this equal to lastFrame
	stack_frame::frame_ptr_t lastFrame = currentFrame;

	// Subtract 0x10 from lastFrame
	asm volatile("subq $0x10, %0"
		: "=r" (lastFrame));

	// If this is equal to '0' return false
	if (lastFrame == nullptr)
		return false;

	// Otherwise get lastFrame
	lastFrame = ((stack_frame::frame_ptr_t)*lastFrame);

	// Set current frame to lastFrame
	currentFrame = lastFrame;

	// Add 0x10
	asm volatile ("addq $0x10, %0"
		: "=r" (currentFrame));

	// Move caller function into this function
	activeFrame.functionAddress = activeFrame.calleeAddress;

	// Then update currentFrame and lastFrame
	activeFrame.frameAddress = currentFrame;

	// Moved up, so one removed
	framesRemoved++;

	// If lastFrame is 0, no caller available
	if (lastFrame == nullptr)
	{
		activeFrame.calleeAddress = 0;
		return true;
	}

	// Get pointer to saved rip
	stack_frame::frame_ptr_t callee = lastFrame;

	// Add 0x08
	asm volatile ("addq $0x08, %0"
		: "=r" (callee));

	// Then dereference into address
	long calleeAddr = (*callee);

	// Get caller
	activeFrame.calleeAddress = calleeAddr;

	// Return true for success
	return true;
}

void BBP::std::stack_trace::Capture()
{
	// Reference
	stack_frame::frame_ptr_t reference = (stack_frame::frame_ptr_t)stack_trace_ref;

	// Store reference
	activeFrame.referenceAddress = *((std::address_t *)&reference);

	// Declare current frame and func
	stack_frame::frame_ptr_t currentFrame;
	stack_frame::frame_ptr_t calleePtr;

	// Get rbp and add 0x10
	asm volatile ("mov %%rbp, %0\n\t"
		"addq $0x10, %0"
		: "=r" (currentFrame));

	// Store that in activeFrame
	activeFrame.frameAddress = currentFrame;

	// Set funcptr to currentFrame
	calleePtr = currentFrame;

	// Subtract 0x08 from rbp
	asm volatile ("subq $0x08, %0"
		: "=r" (calleePtr));

	// Save that as funcPtr
	activeFrame.calleeAddress = *calleePtr;

	// Get RIP
	activeFrame.functionAddress = 0;

	// Then save active frame
	capturedFrame = activeFrame;

	// 0 Frames away now
	framesRemoved = 0;
}

void BBP::std::stack_trace::printFrame(stack_trace_db &db, address_t referencePhysAddress)
{
	// Print
	std::printf("#%u ", framesRemoved);

	// If address known of function, print it (also not if frames removed is 0)
	if (framesRemoved == 0)
	{
		std::printf("at \e[0;33mCapture\e[0;37m ()\n");
		return;
	}

	else
	{
		// Show address
		std::printf("\e[0;34m0x%016x\e[0;37m ", activeFrame.functionAddress);
	}

	// If address unkown, show it as such
	if (activeFrame.functionAddress == 0)
	{
		std::printf("at \e[0;33m??\e[0;37m\n");
		return;
	}

	// Get the offset to the virtual address
	std::offset_t referenceOffset = activeFrame.functionAddress - activeFrame.referenceAddress;

	// Now get physical address of this function
	std::address_t realAddress = referencePhysAddress + referenceOffset;

	// Now look up db
	db.binLookup(realAddress);

	// Then line lookup
	db.lineLookup();

	// Then print information
	std::printf("at <\e[0;33m");
	db.printName();
	std::printf(" ()\n");
}

void BBP::std::stack_trace::printReference()
{
	// Print
	std::printf("#R \e[0;34m0x%016x\e[0;37m - \e[0;33mReference\e[0;37m\n", activeFrame.referenceAddress);
}

void BBP::std::stack_trace::showStackTrace()
{
	// Get a stack_trace_db here
	stack_trace_db db;

	// Get reference address
	std::address_t refAddress = 0x22972;

	// Move up once. If unable to, break out
	if (moveUp() == false)
		return;

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