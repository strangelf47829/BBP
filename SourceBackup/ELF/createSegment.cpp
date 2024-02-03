#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;

segmentBuilder *BBP::Services::Binaries::ELF::createSegment(const char *name, unsigned char *data, WORD size, WORD mem_size, WORD flags, bool smart)
{
	// Allocate memory for the segment
	segmentBuilder *segment = new segmentBuilder();

	// Set segment data
	segment->data = data;

	// Set other variables
	segment->header.filesz = size;
	segment->header.memsz = mem_size;
	segment->header.flags = flags;
	segment->autoDealloc = smart;

	// Calculate name length
	while (name[segment->nameLength++])
	{
	}

	// Copy over name to header
	segment->name = (char *)calloc(segment->nameLength, sizeof(char));
	for (unsigned int i = 0; i < segment->nameLength; i++)
		segment->name[i] = name[i];

	return segment;
}