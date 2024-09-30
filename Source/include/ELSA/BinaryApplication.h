#ifndef BBP_ELSA_BINARYAPPLICATION_H
#define BBP_ELSA_BINARYAPPLICATION_H

#include "Symbol.h"
#include "SectionDB.h"
#include "ELFHeader.h"
#include "ELF.h"

namespace BBP
{
	namespace elsa
	{
		
		class BinaryApplication
		{
			// Stack for Section data
			std::STATIC_PAGE<Section *, 14> sectionPointers;

			// Header information
			ELF::ELF elf;

			// Standard sections
			Section handlers;
			Section text;
			Section got;
			Section data;
			Section bss;
			Section rodata;
			Section reltext;
			Section relgot;
			Section symtab;
			Section strtab;

		public:

			// Constructor
			BinaryApplication();

			// Define sections
			void defineSections();
			
			// Define segments
			void defineSegments();

			// Get a byte from this file
			std::byte &operator[] (std::index_t);

			// Get section from this file
			Section *operator[] (std::string);

			// Is littleEndian?
			bool isLittleEndian();

			// Set up symbol db
			void setup(symbol_db &);

			// Set data section for symbol db
			void setDataSection(symbol_db &, std::string &);

			// Emit file
			void emitFile(std::conststring);



		};

	}
}

#endif