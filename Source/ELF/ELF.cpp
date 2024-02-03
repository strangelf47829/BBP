#include "../include/ELF.h"
#include "../include/Resources.h"

void BBP::std::ELF::ELFBuilder::emitELFHeaders()
{

	// Set phnum and shnum
	phnum(segments.max_elements);
	shnum(sections.max_elements);

	// If phnum is not 0, set offset directly after header
	if (segments.max_elements)
	{
		phoff(52);
		// Then, if shnum is not 0, set shoff directly after program headers
		if (sections.max_elements)
			shoff(52 + segments.max_elements * ELF_SEGMENT_HEADERSIZE);
	}
	else if (sections.max_elements)
	{
		// Otherwise set shoff to 52 if it has sections
		shoff(52);
	}
}

BBP::std::size_t BBP::std::ELF::ELFBuilder::calculateHeaderSize()
{
	return 52 + phnum() * ELF_SEGMENT_HEADERSIZE + shnum() * ELF_SECTION_HEADERSIZE;
}

// This function emits header stuff
void BBP::std::ELF::ELFBuilder::emitHeader()
{
	// Write the 16 magic ident bytes
	ident(0, 0x7F);
	ident(1, 'E');
	ident(2, 'L');
	ident(3, 'F');

	// 32 Bits
	ident(4, 0x01);
	
	// Endian-nes
	ident(5, Endian + 1);

	// ELF header version, always 1
	ident(6, 1);

	// OS ABI (version 1)
	ident(7, 0x1);

	// Default is relocatable
	type(1);

	// Custom instruction set, so set to whatever
	machine(0);

	// Set version to 1
	version(1);

	// Set phnum and shnum
	phnum(segments.max_elements);
	shnum(sections.max_elements);

	// Emit the other headers
	emitELFHeaders();

	// Set the respective pages for each segment
	std::offset_t phOffset = phoff();
	for (std::index_t segment = 0; segment < segments.max_elements; segment++)
		this->segments[segment].header = std::string(ELF_SEGMENT_HEADERSIZE, file.data + phOffset + segment * ELF_SEGMENT_HEADERSIZE);

	// ... and section
	std::offset_t shOffset = shoff();
	for (std::index_t section = 0; section < sections.max_elements; section++)
		this->sections[section].header = std::string(ELF_SECTION_HEADERSIZE, file.data + shOffset + section * ELF_SECTION_HEADERSIZE);

	// No flags
	flags(0);

	// Header size, which is always 52
	ehsize(52);

	// Set sizes
	phentsize(ELF_SEGMENT_HEADERSIZE);
	shentsize(ELF_SECTION_HEADERSIZE);

	// No string index
	shstrndx(0);

}

void BBP::std::ELF::ELFBuilder::recalculateOffsets(std::offset_t after, std::size_t howmuch)
{
	std::size_t PHnum = phnum();
	for (index_t index = 0; index < PHnum; index++)
	{
		// If segment is below or equal to this one, do nothing
		if (segments[index].offset() <= after)
			continue;

		// If segment has no data, do nothing
		if (segments[index].filesz() == 0)
			continue;

		// Add offset
		segments[index].offset(segments[index].offset() + howmuch);
	}

	std::size_t SHnum = shnum();
	for (index_t index = 0; index < SHnum; index++)
	{
		// If section is below or equal to this one, do nothing
		if (sections[index].offset() <= after)
			continue;

		// If section has no data, do nothing
		if (sections[index].size() == 0)
			continue;

		// Add offset
		sections[index].offset(sections[index].offset() + howmuch);
	}

}