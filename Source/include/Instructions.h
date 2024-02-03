#ifndef BBP_STDLIB_INSTRUCTIONS_H
#define BBP_STDLIB_INSTRUCTIONS_H

#include "stddef.h"

namespace BBP
{
	namespace userspace
	{

		struct Instruction
		{
			// The head struct stores information about the opcodes
			struct Head
			{
				// Op-code data
				std::byte prefix;
				std::byte opcode;
				std::byte suffix;

				// Flags
				std::nibble flags;
				std::nibble args;
			} header;

			// The Arg struct stores information about the type and remaining info needed to decode arguments
			struct Arg
			{
				// If literal argument, this is where the data lives.
				std::word data;

				// Otherwise,
				bool referenced;
				bool dereferenced;

				std::nibble offset;
				std::crumb trailerCount;

				// Trailers
				std::word trailers[4];

				// Is literal?
				bool isLiteralArg;

				// Can this be any value?
				bool anyvalue;
			} args[8];
		};



	}
}

#endif