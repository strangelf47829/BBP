#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;


void BBP::Services::Binaries::ELF::destroyELF(ELFBuilder *elf)
{
	// Return if ELF is invalid
	if (elf == nullptr)
		return;

	// Clear data of each program header
	for (WORD ph = 0; ph < elf->header.e_phnum; ph++)
	{
		// Check if program header valid
		if (elf->segments[ph] == nullptr)
			continue;

		// Check if data valid
		if (elf->segments[ph]->data == nullptr)
			continue;

		// Free data
		//free(elf->segments[ph]->data);
	}

	// Free program headers
	setELFSegmentCount(elf, 0);

	// Clear data of each section
	for (WORD sh = 0; sh < elf->header.e_shnum; sh++)
	{
		// Check if section is valid
		if (elf->sections[sh] == nullptr)
			continue;

		// Check if data is valid
		if (elf->sections[sh]->data == nullptr)
			continue;

		// Free data
		//free(elf->sections[sh]->data);
	}

	// Free section headers
	setELFSectionCount(elf, 0);

	// Free and clear symbols
	setELFSymbolCount(elf, 0);

	// Clear both string tables
	setStringTableSize(&elf->shstrndx, 0);
	setStringTableSize(elf->genericStrings, 0);

	// Free generic string table (shstrndx is statically allocated!)
	if (elf->genericStrings != nullptr)
	{
		delete(elf->genericStrings);
		elf->genericStrings = nullptr;
	}

	// Free ELF header itself
	delete(elf);

	// Done!
}