#ifndef BBP_ELSA_ELFRELOCATION_H
#define BBP_ELSA_ELFRELOCATION_H

#include "../Memory.h"

namespace BBP
{
	namespace ELF
	{

		struct Relocation
		{
			// Page with data in it
			std::PAGE<std::byte> data;

			// ET_REL: Offset within section, ET_EXEC: Virtual memory
			std::offset_t offset;

			// 'value' is actually 24 bits for some reason
			std::word value;
			std::byte type;

			// Then read/write
			void readData(bool endian);
			void writeData(bool endian);

			// Nullify
			void Nullify();

			// Constructor
			Relocation();
		};

	}
}

#endif