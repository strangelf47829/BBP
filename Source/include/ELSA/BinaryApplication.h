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
			//Section dynstr;
			Section data;
			Section bss;
			//Section data1;
			Section rodata;
			Section reltext;
			Section relgot;
			Section symtab;
			Section strtab;
			//Section hashtab;

		public:

			// Constructor
			BinaryApplication();

			// Define sections
			void defineSections();
			
			// Define segments
			void defineSegments();

			// Get a byte from this file
			std::byte &operator[] (std::index_t);

			// Emit file
			void emitFile(std::conststring);



		};

	}
}

#endif