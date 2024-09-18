#ifndef BBP_ELSA_BINARYAPPLICATION_H
#define BBP_ELSA_BINARYAPPLICATION_H

#include "Symbol.h"
#include "SectionDB.h"
#include "ELFHeader.h"

namespace BBP
{
	namespace elsa
	{
		
		class BinaryApplication
		{
			// Header information
			ELF::ELFHeader header;

			// The section containing the sections
			Section sections;

			// Standard sections
			Section handlers;
			Section text;
			Section got;
			Section dynstr;
			Section data;
			Section bss;
			Section data1;
			Section rodata;
			Section reltext;
			Section symtab;
			Section shstrtab;
			Section strtab;
			Section hashtab;

		public:

			// Constructor
			BinaryApplication();

			// Get a byte from this file
			std::byte &operator[] (std::index_t);

			// Emit file
			void emitFile();



		};

	}
}

#endif