#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;

void BBP::Services::Binaries::ELF::addELFSymbol(ELFBuilder *elf, SymbolEntry *symbol, char *name)
{
	if (elf == NULL)
		return; // Invalid ELF

	if (symbol == NULL)
		return; // Invalid symbol

	// If name is nullptr, just skip naming
	if (name != nullptr)
	{
		// Add section name to shstrndx
		unsigned int index = addStringToTable(elf->genericStrings, name);

		// Get offset, and assign to section
		unsigned int offset = getStringOffset(elf->genericStrings, index);

		symbol->st_name = offset;
	}

	// Increase the symbol count by one
	setELFSymbolCount(elf, elf->symbolCount + 1);

	// Assign 'symb' to the last symbol
	elf->symbols[elf->symbolCount - 1] = symbol;
}