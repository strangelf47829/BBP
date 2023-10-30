#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;

ELFDamageReport BBP::Services::Binaries::ELF::fixAllDamage(ELFBuilder *elf, unsigned char *binary)
{
	ELFDamageReport report = ELFDamageReport({0, false});

	if (checkForLeadingNullStrings(elf, &report))
		fixLeadingNullStrings(elf, elf->sections[elf->header.e_shstrndx], &elf->shstrndx, &report);
	if (checkForDisorganizedSymbolNames(elf, &report))
	{
		for (WORD sh = 0; sh < elf->header.e_shnum; sh++)
		{
			sectionBuilder *section = elf->sections[sh];

			if (section->header.sh_type != SHT_SYMTAB)
				continue;
			fixDisorganizedSymbolNames(binary, elf, sh, &report);
			break;
		}
	}
		

	return report;
}

bool BBP::Services::Binaries::ELF::checkForLeadingNullStrings(ELFBuilder *elf, ELFDamageReport *report)
{
	// Check for invalid ELF
	if (elf == nullptr)
		return false;

	// Get string table index, and check validity
	if (elf->header.e_shoff != 0 && elf->header.e_shnum > elf->header.e_shstrndx)
	{
		// Get handle
		sectionBuilder *shstrndx = elf->sections[elf->header.e_shstrndx];

		uint8_t nullStringCount = 0;
		WORD lastNullAt = 0;
		// TODO Implement checking
		for (WORD ptr = 0; ptr < shstrndx->header.sh_size; ptr++)
		{
			// Read character
			char c = shstrndx->data[ptr];

			// Continue if char is non-null
			if (c != 0)
				break; // Checking for leading!

			if (ptr == 0)
			{
				// Edge case
				nullStringCount++;
				continue;
			}

			// Null found
			if(ptr - 1 == lastNullAt)
				nullStringCount++; // Nullstring
			lastNullAt = ptr;
		}

		if (report != nullptr)
			report->leadingNullStrings = nullStringCount;

		if (nullStringCount > 1)
			return true; // Too many leading null strings
	}

	if (report != nullptr)
		report->leadingNullStrings = 1; // May overwrite, but whatevs

	return false; // Nothing found
}

bool BBP::Services::Binaries::ELF::fixLeadingNullStrings(sectionBuilder *section, stringTable *table, ELFDamageReport *report)
{
	return fixLeadingNullStrings(nullptr, section, table, report);
}

bool BBP::Services::Binaries::ELF::fixLeadingNullStrings(ELFBuilder *elf, sectionBuilder *section, stringTable *table, ELFDamageReport *report)
{
	// Check for invalid section
	if (section == nullptr)
		return false;

	// Check for invalid table
	if (table == nullptr)
		return false;

	uint32_t readCounter = 0;
	uint32_t offs = 0;

	// Read string table until success
	while (true)
	{
		// Read string table
		int status = readELFStringTable(section->data, offs, section->header.sh_size, table);

		if (status < 4)
			break; // Success!
	
		if (++readCounter >= ELFREPAIR_MAXATTEMPTS)
		{
			// Ran out of attempts

			// If damage report is not null, set corresponding field
			if (report != nullptr)
				report->leadingNullStrings += status - 4;

			// Not success
			return false;
		}
	
		// Damage detected, fix damage
		WORD nullStringCount = status - 4;

		// Attempt fix by shrinking table
		section->header.sh_offset += nullStringCount;
		section->header.sh_size -= nullStringCount;
		offs += nullStringCount;

		// If ELF is not null, offset everything by one
		if (elf != nullptr)
		{
			// Now offset everything by one (except for first header)
			for (WORD sh = 1; sh < elf->header.e_shnum; sh++)
				elf->sections[sh]->header.sh_name--;
			elf->sections[0]->header.sh_name = 0; // The first one gets offset 0
		}


		// Clear shstrndx
		setStringTableSize(table, 0);

		// Try reading again
	}

	// Get data to write
	unsigned char *dat = emitStringTableWithNull(table);

	// Free old data
	free(section->data);

	// Overwrite data
	section->data = dat;

	// Success !
	return true;
}


bool BBP::Services::Binaries::ELF::checkForDisorganizedSymbolNames(ELFBuilder *elf, ELFDamageReport *report)
{
	// Set counters
	uint32_t lastOffset = 0;

	// Declare accumulator
	bool acc = false;

	// If report exists, handle it
	if (report != nullptr)
	{
		// Remember the state of report
		acc = report->DisorganizedSymbolNames;

		// Assume true for now
		report->DisorganizedSymbolNames = true;
	}

	// Loop over each symbol, then compare its name to the last name
	for (WORD symb = 0; symb < elf->symbolCount; symb++)
	{
		SymbolEntry *symbl = elf->symbols[symb];
		if (symbl->st_name <= lastOffset && symb != 0)
			return true;
		lastOffset = symbl->st_name;
	}

	// Nothing found, reset report if it exists
	if (report != nullptr)
		report->DisorganizedSymbolNames = acc;
		
	return false;
}

bool BBP::Services::Binaries::ELF::fixDisorganizedSymbolNames(unsigned char *data, ELFBuilder *elf, WORD symtableIndex, ELFDamageReport *report)
{
	uint32_t lastOffset = 0;

	for (WORD symb = 0; symb < elf->symbolCount; symb++)
	{
		// Update offset 
		elf->symbols[symb]->st_name = lastOffset;

		// Update file (header)
		data[elf->sections[symtableIndex]->header.sh_offset + symb * elf->sections[symtableIndex]->header.sh_entsize + 0] = lastOffset;

		// Update data
		elf->sections[symtableIndex]->data[symb * elf->sections[symtableIndex]->header.sh_entsize] = lastOffset;

		// Update offset
		lastOffset += elf->genericStrings->lengths[symb];
	}

	// Checking for damage
	if(checkForDisorganizedSymbolNames(elf, report))
	{ 
		if (report != nullptr)
			report->DisorganizedSymbolNames = false; 
		return false; 
	}

	// Damage fixed, now rewrite symbtab, since it has been invalidated.
	free(elf->sections[symtableIndex]->data);
	elf->sections[symtableIndex]->data = nullptr;

	readELFSectionHeader(data, elf, elf->header.e_shoff + symtableIndex * elf->header.e_shentsize, symtableIndex);

	if (report != nullptr)
		report->DisorganizedSymbolNames = true;

	return true;
}