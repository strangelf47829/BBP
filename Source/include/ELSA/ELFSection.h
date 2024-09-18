#ifndef BBP_ELSA_ELFSECTION_H
#define BBP_ELSA_ELFSECTION_H

#include "../Memory.h"

namespace BBP
{
	namespace ELF
	{

		struct Section
		{
			// Data actually stored on the ELF 
			std::PAGE<std::byte> data;

			// Fields
			std::word name;
			std::word type;
			std::word flags;
			std::address_t address;
			std::offset_t offset;
			std::word size;
			std::word link;
			std::word info;
			std::word addralign;
			std::word entsize;

			// Read/write
			void readData(bool endian);
			void writeData(bool endian);
		};

	}
}

#endif