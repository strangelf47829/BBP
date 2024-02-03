#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;


void BBP::Services::Binaries::ELF::setELFSegmentCount(ELFBuilder *elf, HALFWORD count)
{
	if (elf == NULL)
		return; // Invalid ELF, do nothing

	if (count == elf->header.e_phnum)
		return; // no change in count, so do nothing

	if (count == 0)
	{
		// Free all segments
		for (int i = 0; i < elf->header.e_phnum; i++)
		{
			// Free data if autodealloc
			if (elf->segments[i]->autoDealloc && elf->segments[i]->data != nullptr)
				free(elf->segments[i]->data);

			// Free the section name, since it is copied.
			free(elf->segments[i]->name);

			// Delete the segment itself
			delete(elf->segments[i]);
		}

		// We can now free the array itself too, to indicate that the data is invalid
		free(elf->segments);

		// Update segment count
		elf->header.e_phnum = 0;

		// Set the segments variable to NULL, then return.
		elf->segments = NULL;

		return;
	}

	if (elf->segments == NULL && elf->header.e_phnum)
		return; // Invalid segments

	if (count > elf->header.e_phnum)
	{
		// Create new array
		segmentBuilder **newArr = (segmentBuilder **)calloc(count, sizeof(segmentBuilder *));

		// Copy over data from old array
		for (int i = 0; i < elf->header.e_phnum; i++)
			newArr[i] = elf->segments[i];

		// Zero init. array.
		for (int i = elf->header.e_phnum; i < count; i++)
			newArr[i] = nullptr;

		// Update header.e_phnum
		elf->header.e_phnum = count;

		// Free old array
		free(elf->segments);

		// Set old array to new array and return
		elf->segments = newArr;

		return;
	}

	// Create new array
	segmentBuilder **newArr = (segmentBuilder **)calloc(count, sizeof(segmentBuilder *));

	// Copy only relevant data from old array
	for (int i = 0; i < count; i++)
		newArr[i] = elf->segments[i];

	// Free old data from array
	for (int i = count; i < elf->header.e_phnum; i++)
	{
		// Free data if autodealloc
		if (elf->segments[i]->autoDealloc && elf->segments[i]->data != nullptr)
			free(elf->segments[i]->data);

		// Free the segment name, since it is copied.
		free(elf->segments[i]->name);

		// Free the segment itself
		delete(elf->segments[i]);
	}

	// Update header.e_phnum
	elf->header.e_phnum = count;

	// Free the old array
	free(elf->segments);

	// Replace the array, and return
	elf->segments = newArr;
}