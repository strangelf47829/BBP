#ifndef BBP_STD_STACKTRACE_H
#define BBP_STD_STACKTRACE_H

#include "../include/stddef.h"

namespace BBP
{
	namespace std
	{
		// Holds the information needed to store a single frame of the stack
		struct stack_frame
		{
			// The type used when doing this stuff
			using frame_ptr_t = long *;

			// Address of this stack frame
			frame_ptr_t frameAddress;

			// Address of the function that called this frame
			frame_ptr_t functionAddress;
		};

		// Holds the information needed to store a stack trace
		class stack_trace
		{
			// Active frame being used
			stack_frame activeFrame;

			// Move the active frame up
			bool moveUp();

		public:

			// Create a new stack trace
			stack_trace();

			// Capture current stack
			void Capture();

			// Count how deep the stack is
			std::size_t stackDepth();

		};

		void trace();

	}
}

#endif