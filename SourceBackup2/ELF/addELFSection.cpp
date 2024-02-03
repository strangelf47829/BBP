#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;


void BBP::Services::Binaries::ELF::addELFSection(ELFBuilder *elf, sectionBuilder *section)
{
	if (elf == NULL)
		return; // Invalid ELF

	if (section == NULL)
		return; // Invalid section

	// Add section name to shstrndx
	unsigned int index = addStringToTable(&elf->shstrndx, section->name);

	// Get offset, and assign to section
	unsigned int offset = getStringOffset(&elf->shstrndx, index);
	section->header.sh_name = offset;

	// Increase the section count by one
	setELFSectionCount(elf, elf->header.e_shnum + 1);

	// Assign 'section' to the last section
	elf->sections[elf->header.e_shnum - 1] = section;
}