#include "../include/StackTrace.h"
#include "../include/stddef.h"
#include "../include/stdio.h"

// Move a stack trace up
bool BBP::std::stack_trace::moveUp()
{
    // Get pointer to current active frame
    stack_frame::frame_ptr_t currentFrame = activeFrame.frameAddress;

    // Set this equal to lastFrame
    stack_frame::frame_ptr_t lastFrame = currentFrame;

    // Subtract 0x10 from lastFrame
    asm volatile("subq $0x10,%0"
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

    // Then update currentFrame and lastFrame
    activeFrame.frameAddress = currentFrame;

    // Return true for success
    return true;
}

void BBP::std::stack_trace::Capture()
{
    // Declare current frame
    stack_frame::frame_ptr_t currentFrame;

    // Get rbp and add 0x10
    asm volatile ("mov %%rbp, %0"
        : "=r" (currentFrame));

    // Add 0x10

    // Store that in activeFrame
    activeFrame.frameAddress = 

}


void BBP::std::trace()
{
    // Using directive
    using frame_ptr_t = long *;

    // Get pointer to current active frame
    frame_ptr_t currentFrame;

    // Set this equal to lastFrame
    frame_ptr_t lastFrame;

    // Move rbp into frame_pointer
    asm volatile ("mov %%rbp, %0"
        : "=r" (lastFrame));

    // Go through stack trace
    while (lastFrame) 
    {
        // Write lastFrame to current Frame
        currentFrame = lastFrame;

        // Add 0x10 to currentFrame
        asm volatile ("addq $0x10,%0"
            : "=r" (currentFrame));

        // Then dereference lastFrame
        lastFrame = ((frame_ptr_t)*lastFrame);

        // Print out data
        std::printf("Current frame is %p, which came from frame at %p\n", currentFrame, lastFrame);    }
}