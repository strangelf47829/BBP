#ifndef BBP_ELSA_SECTIONDB_H
#define BBP_ELSA_SECTIONDB_H

#include "SectionBuilder.h"
#include "../Dictionary.h"
#include "../State.h"
#include "../Strings.h"

namespace BBP
{
	namespace elsa
	{

		// Create a database storing sections of a binary file
		class section_db : public std::State
		{
			// To relate the name of a section with it's index
			std::HashedDictionary<std::string, std::index_t> indicies;

			// Allocator used to allocate sections
			std::objectAllocator allocator;

			// Page of sections
			std::PAGE<Section> sections;

		public:

			// Initialize
			void init() override;

			// Construcors and destructors
			section_db();
			~section_db();

			// Create a new section
			void createSection(std::string);

			// Lookup section
			Section &operator[] (std::string);

			// Get byte
			std::byte get(std::index_t);

			// Reset
			void Reset() override;

		};

	}
}

#endif