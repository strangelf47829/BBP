#ifndef BBP_ELSA_SECTIONBUILDER_H
#define BBP_ELSA_SECTIONBUILDER_H

#include "../SequentialMemory.h"

namespace BBP
{
	
	namespace elsa
	{

		// A section 
		class Section
		{
			// Stores the stuff for memory
			std::SequentialMemory<std::byte> memory;

			// Own index
			std::index_t _index;

			// Next Section
			Section *nextSection;

		public:

			// Allocate some memory
			std::offset_t Allocate(std::PAGE<std::byte> &, std::size_t);
			std::offset_t Allocate(std::PAGE<std::byte> &, std::size_t, std::size_t);

			// Free up a pointer, could be used to reallocate certain blocks
			void Free(std::PAGE<std::byte> &);

			// Index of this section
			std::index_t &index();

			// Free everything
			void Reset();

			// Retrieve some data
			std::byte &operator[] (std::index_t);

			// Retrieve total size
			std::size_t size();

			// Link after a page
			Section &Link(Section &);

		};

	}

}

#endif