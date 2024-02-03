#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;

void BBP::Services::Binaries::ELF::setELFSectionCount(ELFBuilder *elf, HALFWORD count)
{
	if (elf == NULL)
		return; // Invalid ELF, do nothing

	if (count == elf->header.e_shnum)
		return; // no change in count, so do nothing

	if (count == 0)
	{
		// Free all sections
		for (int i = 0; i < elf->header.e_shnum; i++)
		{
			// Free data if autodealloc
			if (elf->sections[i]->autoDealloc && elf->sections[i]->data != nullptr)
				free(elf->sections[i]->data);

			// Free the section name, since it is copied.
			free(elf->sections[i]->name);

			// Free the section itself
			delete(elf->sections[i]);
		}

		// We can now free the array itself too, to indicate that the data is invalid
		free(elf->sections);

		// Update section count
		elf->header.e_shnum = 0;

		// Set the sections variable to NULL, then return.
		elf->sections = NULL;

		return;
	}

	if (elf->sections == NULL && elf->header.e_shnum)
		return; // Invalid sections

	if (count > elf->header.e_shnum)
	{
		// Create new array
		sectionBuilder **newArr = (sectionBuilder **)calloc(count, sizeof(sectionBuilder *));

		// Copy over data from old array
		for (int i = 0; i < elf->header.e_shnum; i++)
			newArr[i] = elf->sections[i];

		// Zero init. Array
		for (int i = elf->header.e_shnum; i < count; i++)
			newArr[i] = nullptr;

		// Update header.e_shnum
		elf->header.e_shnum = count;

		// Free old array
		free(elf->sections);

		// Set old array to new array and return
		elf->sections = newArr;

		return;
	}

	// Create new array
	sectionBuilder **newArr = (sectionBuilder **)calloc(count, sizeof(sectionBuilder *));

	// Copy only relevant data from old array
	for (int i = 0; i < count; i++)
		newArr[i] = elf->sections[i];

	// Free old data from array
	for (int i = count; i < elf->header.e_shnum; i++)
	{
		// Free the section name, since it is copied.
		free(elf->sections[i]->name);

		// Free the section itself
		free(elf->sections[i]);
	}

	// Update header.e_shnum
	elf->header.e_shnum = count;

	// Free the old array
	free(elf->sections);

	// Replace the array, and return
	elf->sections = newArr;
}