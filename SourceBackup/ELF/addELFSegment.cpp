#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;

void BBP::Services::Binaries::ELF::addELFSegment(ELFBuilder *elf, segmentBuilder *segment)
{
	if (elf == NULL)
		return; // Invalid ELF

	if (segment == NULL)
		return; // Invalid segment

	// Increase the segment count by one
	setELFSegmentCount(elf, elf->header.e_phnum + 1);

	// Assign 'symb' to the last segment
	elf->segments[elf->header.e_phnum - 1] = segment;
}