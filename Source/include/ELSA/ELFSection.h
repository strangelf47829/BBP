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

			// Reset all to 0
			void nullify();

			// Constructor
			Section();

			// Type
			enum SectionType : std::word
			{
				SHT_NULL = 0,	// Null does nothing
				SHT_PROGBITS,	// Program Binary Data
				SHT_SYMTAB,		// Symbol table
				SHT_STRTAB,		// String table
				SHT_RELA,		// Relocation with addends
				SHT_HASH,		// Hash table 
				SHT_DYNAMIC,	// Dynamic linking data
				SHT_NOTE,		// Contains ElfN_Nhdr
				SHT_NOBITS,		// Contains no bits on file
				SHT_REL,		// Relocation
				SHT_SHLIB,		// Shared library stuff
				SHT_DYNSYM		// Dynamic symbol data
			};

			// Flags
			enum SectionFlags : std::word
			{
				SHF_WRITE = 1,
				SHF_ALLOC = 2,
				SHF_EXECINSTR = 4,
			};
		};

	}
}

#endif