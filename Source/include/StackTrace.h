#ifndef BBP_STD_STACKTRACE_H
#define BBP_STD_STACKTRACE_H

#include "../include/stddef.h"
#include "../include/FileSys.h"

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

			// Current address in this function
			std::address_t functionAddress;

			// Address of the function that called this frmae
			std::address_t calleeAddress;

			// Reference address
			std::address_t referenceAddress;
		};

		// Function finder
		class stack_trace_db
		{
			// Header file
			std::FILE hdr;

			// bin index
			std::index_t binIndex;

			// line
			std::index_t line;

			// starting address
			std::address_t functionStart;

			// offset
			std::offset_t functionOffset;

			// String for name
			std::static_string<200> name;

		public:

			// Constructor/destructor
			stack_trace_db();
			~stack_trace_db();

			// Bin lookup
			void binLookup(std::address_t);

			// Line lookup
			void lineLookup();

			// Name print
			void printName();

		};

		// Holds the information needed to store a stack trace
		class stack_trace
		{
		public:

			// Active frame being used
			stack_frame activeFrame;

		private:

			// The frame as-is when captured
			stack_frame capturedFrame;

			// How many frames have been moved up
			std::size_t framesRemoved;

			// Reference
			static void stack_trace_ref() {}

			// Print
			void printFrame(stack_trace_db &db, std::address_t);
			void printReference();

		public:

			// Create a new stack trace
			stack_trace();

			// Capture current stack with known reference
			void Capture();

			// Move the active frame up
			bool moveUp();

			// Print everything
			void showStackTrace();

		};

	}
}

#endif