#ifndef BBP_ELSA_ELF_H
#define BBP_ELSA_ELF_H

#include "ELFHeader.h"
#include "SectionDB.h"
#include "ELFSection.h"
#include "../FileSys.h"

namespace BBP
{
	namespace ELF
	{

		class ELF
		{

		public:

			// The header for this ELF file
			ELFHeader header;

			// Section table
			elsa::Section sectionTable;

			// Segment table
			elsa::Section segmentTable;

			// Section name table
			elsa::Section sectionStringTable;

			// Page to hold all sections in 
			std::PAGE<elsa::Section *> *sections;

			// Constructor
			ELF(BBP::std::PAGE<elsa::Section *> *sects);

			// Register a section
			std::index_t registerSection(elsa::Section &, BBP::ELF::Section &, std::string);

			// Look for a section
			std::index_t find(std::string);
			elsa::Section *find(std::index_t);

			// Get section from name
			elsa::Section *get(std::string);

			// Pack sections
			std::offset_t pack();
			
			// Save to path
			void saveFile(std::conststring);

		};

	}
}

#endif
