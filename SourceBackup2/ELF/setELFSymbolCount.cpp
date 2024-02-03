#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;


void BBP::Services::Binaries::ELF::setELFSymbolCount(ELFBuilder *elf, WORD count)
{
	if (elf == NULL)
		return; // Invalid ELF, do nothing

	if (count == elf->symbolCount)
		return; // no change in count, so do nothing

	if (count == 0)
	{
		// Free all symbols
		for (WORD i = 0; i < elf->symbolCount; i++)
			delete(elf->symbols[i]); // 'SymbolEntry' contains no pointers

		// We can now free the array itself too, to indicate that the data is invalid
		free(elf->symbols);

		// Update symbolCount
		elf->symbolCount = 0;

		// Set the sections variable to NULL, then return.
		elf->symbols = NULL;

		return;
	}

	if (elf->symbols == NULL && elf->symbolCount)
		return; // Invalid symbols array.

	if (count > elf->symbolCount)
	{
		// Create new array
		SymbolEntry **newArr = (SymbolEntry **)calloc(count, sizeof(SymbolEntry *));

		// Copy over data from old array
		for (WORD i = 0; i < elf->symbolCount; i++)
			newArr[i] = elf->symbols[i];

		// Since this array stores external pointers,
		// Values in this array should be initialized
		// To zero.
		for (WORD i = elf->symbolCount; i < count; i++)
			newArr[i] = nullptr;

		// Update symbol count
		elf->symbolCount = count;

		// Free old array
		free(elf->symbols);

		// Set old array to new array and return
		elf->symbols = newArr;

		return;
	}

	// Create new array
	SymbolEntry **newArr = (SymbolEntry **)calloc(count, sizeof(SymbolEntry *));

	// Copy only relevant data from old array
	for (WORD i = 0; i < count; i++)
		newArr[i] = elf->symbols[i];

	// Free old data from array
	for (WORD i = count; i < elf->symbolCount; i++)
		free(elf->symbols); // Since data was alloc'ed externally data must also be free'd externaly.

	// Update symbolsCount
	elf->symbolCount = count;

	// Free the old array
	free(elf->symbols);

	// Replace the array, and return
	elf->symbols = newArr;
}