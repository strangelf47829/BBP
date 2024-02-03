#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;

sectionBuilder *BBP::Services::Binaries::ELF::createSection(const char *name, unsigned char *data, WORD type, WORD size, bool smart)
{
	// Allocate memory for the section
	sectionBuilder *section = new sectionBuilder();

	// Set section data
	section->data = data;

	// Set other variables
	section->header.sh_type = type;
	section->header.sh_size = size;
	section->header.sh_addralign = 1;
	section->autoDealloc = smart;

	// Calculate name length
	while (name[section->nameLength++])
	{
	}

	// Copy over name to header
	section->name = (char *)calloc(section->nameLength, sizeof(char));

	for (unsigned int i = 0; i < section->nameLength; i++)
		section->name[i] = name[i];

	return section;
}