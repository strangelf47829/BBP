#ifndef BBP_ELSA_ELFSEGMENT_H
#define BBP_ELSA_ELFSEGMENT_H

#include "../Memory.h"

namespace BBP
{
	namespace ELF
	{

		struct Segment
		{

			std::PAGE<std::byte> data;

			std::word type;
			std::offset_t offset;
			std::address_t vaddr;
			std::address_t paddr;
			std::word filesz;
			std::word memsz;
			std::word flags;
			std::word align;

			// Read and write
			void readData(bool endian);
			void writeData(bool endian);

			// Nullify
			void nullify();

			// Constructor
			Segment();

			enum Type : std::word
			{

				PT_NULL, // Nothing
				PT_LOAD, // Load into memory
				PT_DYNAMIC, // Dynamic linking info
				PT_INTERP, // Path to interpreter
				PT_SHLIB, // Path to shared library
				PT_PHDR, // Segment that contains segments

			};

		};

	}
}

#endif