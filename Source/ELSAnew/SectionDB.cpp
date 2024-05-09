#include "../include/ELSA/SectionDB.h"

// Does nothing so far
void BBP::elsa::section_db::init()
{

}

// Constructors / Destructors
BBP::elsa::section_db::section_db() {}
BBP::elsa::section_db::~section_db() {}

// Create a new section
void BBP::elsa::section_db::createSection(std::string str)
{
	// Get last page
	std::PAGE<Section> *lastSection = &std::last(sections);

	// Get sequence length
	std::size_t sectionCount = std::seqlen(sections);

	// Create new page
	std::STATIC_PAGE<Section, 1> *nextSection = allocator.Register(new std::STATIC_PAGE<Section, 1>());

	// Assign page
	lastSection->nextPage = nextSection;

	// Then add dictionary entry
	indicies.add(str, sectionCount);
}

// Lookup section
BBP::elsa::Section &BBP::elsa::section_db::operator[] (std::string str)
{
	// Lookup section
	std::index_t idx = indicies[str];

	// Return section
	return sections[idx];
}

// Get byte
BBP::std::byte BBP::elsa::section_db::get(std::index_t idx)
{
	return 0;
}

// Reset
void BBP::elsa::section_db::Reset()
{
	// Reset all indicies
	indicies.Reset();

	// Clear out all sections
	allocator.deleteAll();

	// Clear out section
	sections.nextPage = nullptr;
}