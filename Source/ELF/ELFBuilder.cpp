#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;

ELFBuilder *BBP::Services::Binaries::ELF::createELF(HALFWORD type, HALFWORD machine)
{
	ELFBuilder *elf = new ELFBuilder();

	// Set magic numbers
	elf->header.e_ident[0] = 0x7F;
	elf->header.e_ident[1] = 0x45;
	elf->header.e_ident[2] = 0x4c;
	elf->header.e_ident[3] = 0x46;

	// 32 Bit ELF
	elf->header.e_ident[4] = 0x01;

	// LSB first (little endian)
	elf->header.e_ident[5] = 0x1;

	// ELF version 1
	elf->header.e_ident[6] = 1;

	// Set rest to 0.
	for (int i = 7; i < 16; i++)
		elf->header.e_ident[i] = 0;

	// Set elf type and arch according to arguments
	elf->header.e_type = type;
	elf->header.e_machine = machine;

	// Using ELF version 1
	elf->header.e_version = 1;

	// Setting Section table offset to end of this header, and section offset is yet unkown.
	elf->header.e_phoff = 52;
	elf->header.e_shoff = 0;

	// No flags
	elf->header.e_flags = 0;

	// Size of the ELF header is 52.
	elf->header.e_ehsize = 52;

	// We have no sections or segments yet.
	elf->header.e_phnum = 0;
	elf->header.e_shnum = 0;

	// Set section and segment size data here.
	elf->header.e_phentsize = 32;
	elf->header.e_shentsize = 40;

	// We have no shstrndx yet, set index to 0.
	elf->header.e_shstrndx = 0;

	// Now that the header has been created, we need to set the arrays to NULL
	elf->segments = NULL;
	elf->sections = NULL;
	elf->symbols = NULL;

	// Now do the same for shstrndx table
	elf->shstrndx.strings = NULL;
	elf->shstrndx.lengths = NULL;
	elf->shstrndx.stringCount = 0;

	// And also for symbol array
	elf->symbols = NULL;
	elf->symbolCount = 0;

	// Create an empty section
	addELFSection(elf, createSection("\0", NULL, SHT_NILL, 0, false));

	// Create empty generic strings
	elf->genericStrings = new stringTable();

	return elf;
}

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

void BBP::Services::Binaries::ELF::addELFSymbol(ELFBuilder *elf, SymbolEntry *symbol, char *name)
{
	if (elf == NULL)
		return; // Invalid ELF

	if (symbol == NULL)
		return; // Invalid symbol

	// If name is nullptr, just skip naming
	if(name != nullptr)
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
	while(name[segment->nameLength++])
	{}

	// Copy over name to header
	segment->name = (char *)calloc(segment->nameLength, sizeof(char));
	for(unsigned int i = 0; i < segment->nameLength; i++)
		segment->name[i] = name[i];

	return segment;
}

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
	while(name[section->nameLength++])
	{}

	// Copy over name to header
	section->name = (char *)calloc(section->nameLength, sizeof(char));

	for(unsigned int i = 0; i < section->nameLength; i++)
		section->name[i] = name[i];

	return section;
}


stringTable *BBP::Services::Binaries::ELF::createStringTable()
{
	// Allocate memory to string table
	stringTable *table = new stringTable();

	// Initialize data
	table->strings = NULL;
	table->lengths = NULL;
	table->stringCount = 0;

	return table;
}

void BBP::Services::Binaries::ELF::setStringTableSize(stringTable *table, unsigned int count)
{
	if (table == NULL)
		return; // Invalid ELF, do nothing

	if (count == table->stringCount)
		return; // no change in count, so do nothing

	if (count == 0)
	{
		// Free all chars, If it exists
		for (WORD i = 0; i < table->stringCount; i++)
			if(table->strings != nullptr)
				free(table->strings[i]);

		// We can now free the array itself too, to indicate that the data is invalid, if it exists
		if (table->strings != nullptr)
			free(table->strings);
		if (table->lengths != nullptr)
			free(table->lengths);

		// Update symbolCount
		table->stringCount = 0;

		// Set the sections variable to NULL, then return.
		table->strings = nullptr;
		table->lengths = nullptr;

		return;
	}

	if (table->strings == NULL && table->stringCount)
		return; // Invalid symbols array.

	if (count > table->stringCount)
	{
		// Create new array
		char **newArr = (char **)calloc(count, sizeof(char *));
		unsigned int *lengths = (unsigned int *)calloc(count, sizeof(unsigned int));

		// Copy over data from old array
		for (unsigned int i = 0; i < table->stringCount; i++)
		{
			newArr[i] = table->strings[i];
			lengths[i] = table->lengths[i];
		}

		// Alloc new data.
		for (unsigned int i = table->stringCount; i < count; i++)
		{
			newArr[i] = NULL; // Has to be assigned later
			lengths[i] = 0;
		}

		// Update string count
		table->stringCount = count;

		// Free old array, if it exists.
		if (table->strings != nullptr)
			free(table->strings);
		if (table->lengths != nullptr)
			free(table->lengths);

		// Set old array to new array and return
		table->strings = newArr;
		table->lengths = lengths;

		return;
	}

	// Create new array
	char **newArr = (char **)calloc(count, sizeof(char *));
	unsigned int *lengths = (unsigned int *)calloc(count, sizeof(unsigned int *));

	// Copy only relevant data from old array
	for (unsigned int i = 0; i < count; i++)
	{
		newArr[i] = table->strings[i];
		lengths[i] = table->lengths[i];
	}

	// Free old data from array
	for (unsigned int i = count; i < table->stringCount; i++)
		free(table->strings[i]); // Internally alloc'ed data must be freed manually.

	// Update string count
	table->stringCount = count;

	// Free the old array
	free(table->strings);
	free(table->lengths);

	// Replace the array, and return
	table->strings = newArr;
	table->lengths = lengths;
}

unsigned int BBP::Services::Binaries::ELF::addStringToTable(stringTable *table, char *string)
{
	if (table == NULL)
		return 0; // Invalid table

	// Increase table size by one
	setStringTableSize(table, table->stringCount + 1);

	// Calculate string length
	unsigned int length = 0;
	for (; string[length++];);



	// Create buffer with length 'length'
	char *buffer = (char *)calloc(length + 1, sizeof(char));

	// Copy data
	for (unsigned int i = 0; i < length; i++)
		buffer[i] = string[i];
	buffer[length] = 0;


	if(string[0] == 0)
		length++;

	// Update values accordingly.
	table->strings[table->stringCount - 1] = buffer;
	table->lengths[table->stringCount - 1] = length;

	return table->stringCount - 1;
}

unsigned int BBP::Services::Binaries::ELF::addStringToTable(stringTable *table, const char *string, char ignore)
{
	if (table == NULL)
		return 0; // Invalid table

	// Increase table size by one
	setStringTableSize(table, table->stringCount + 1);

	// Store the amount of ignored characters
	unsigned int ignoredChars = 0;

	// Calculate string length, and get ignored char amount
	unsigned int length = 0;
	while (string[length++])
	{
		if (string[length] == ignore)
			ignoredChars++;
	}

	// Create buffer with length 'length - ignoredChars'
	char *buffer = (char *)calloc(length - ignoredChars + 1, sizeof(char));

	// Copy data
	unsigned int mod = 0;
	for (unsigned int i = 0; i < length; i++)
	{
		char c = string[i];
		if (c == ignore)
			mod++;
		else
			buffer[i - mod] = c;
	}

	buffer[length - ignoredChars] = 0;

	if (string[0] == 0)
		length++;

	// Update values accordingly.
	table->strings[table->stringCount - 1] = buffer;
	table->lengths[table->stringCount - 1] = length - ignoredChars;

	return table->stringCount - 1;
}

unsigned int BBP::Services::Binaries::ELF::getStringOffset(stringTable *table, unsigned int index)
{
	if (table == NULL)
		return 0; // Invalid table

	if (index >= table->stringCount)
		return 0; // Invalid argument

	if (index == 0)
		return 0; // Return 0 if index is 0

	unsigned int cummul = 0;
	for (unsigned int i = 0; i < index; i++)
		cummul += table->lengths[i];

	return cummul;
}
