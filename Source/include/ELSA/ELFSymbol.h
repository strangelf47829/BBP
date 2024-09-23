#ifndef BBP_ELSA_ELFSYMBOL_H
#define BBP_ELSA_ELFSYMBOL_H

#include "../Memory.h"

namespace BBP
{
	namespace ELF
	{

		struct Symbol
		{
			// Page with data in it
			std::PAGE<std::byte> data;

			// Members
			std::word name;
			std::address_t value;
			std::size_t size;
			std::byte info;
			std::byte other;
			std::halfword shndx;

			// Then read/write
			void readData(bool endian);
			void writeData(bool endian);

			// Nullify
			void Nullify();

			// Constructor
			Symbol();
		};

	}
}

#endif