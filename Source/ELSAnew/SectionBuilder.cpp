#include "../include/ELSA/SectionBuilder.h"


// Allocate some memory
BBP::std::offset_t BBP::elsa::Section::Allocate(std::PAGE<std::byte> &page, std::size_t _size)
{
	// Get current offset into section
	std::size_t currentlyAt = memory.dataSize();

	// Get memory
	std::byte *data = memory.Malloc(_size);

	// Set page
	page = std::PAGE<std::byte>(_size, data);

	// Return offset
	return currentlyAt;
}

BBP::std::offset_t BBP::elsa::Section::Allocate(std::PAGE<std::byte> &page, std::size_t _count, std::size_t _size)
{
	// Get current offset into section
	std::size_t currentlyAt = memory.dataSize();

	// Get memory
	std::byte *data = memory.Calloc(_count, _size);

	// Set page
	page = std::PAGE<std::byte>(_count * _size, data);

	// Return offset
	return currentlyAt;
}

BBP::std::index_t &BBP::elsa::Section::index()
{
	return _index;
}

// Free some memory
void BBP::elsa::Section::Reset()
{
	memory.freeAllData();
}

// Retrieve some data
BBP::std::byte &BBP::elsa::Section::operator[] (std::index_t idx)
{
	// Get size
	std::size_t size = memory.dataSize();

	// If index is above size, and nextSection exists, return that instead
	if (idx >= size && nextSection)
		return (*nextSection)[idx - size];

	// Otherwise simply return memory
	return memory[idx];
}

// Retrieve total size
BBP::std::size_t BBP::elsa::Section::size()
{
	return memory.dataSize();
}

// Link something
BBP::elsa::Section &BBP::elsa::Section::Link(Section &section)
{
	// Write next section
	nextSection = &section;

	// Return section
	return section;
}