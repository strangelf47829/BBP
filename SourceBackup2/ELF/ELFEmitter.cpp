#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;


WORD BBP::Services::Binaries::ELF::calculateFileSize(ELFBuilder *elf)
{
	// File is at least 52 bytes
	WORD size = elf->header.e_ehsize;

	// For each 'phnum', add 'e_phentsize'.
	size += elf->header.e_phentsize * elf->header.e_phnum;

	// For >each 'shnum', add 'e_shentsize'
	size += elf->header.e_shentsize * elf->header.e_shnum;

	// For each program header, sum their sizes to 'size'.
	// This is because segments do not overlap, so each 'index' is unique.
	for (HALFWORD ph = 0; ph < elf->header.e_phnum; ph++)
		size += elf->segments[ph]->header.filesz;

	// Sections are not unique: they often overlap with segments.
	// This means that sections are not part of file size calculations.
	// However, some sections may not be inside a segment.
	// Sections with who have 'data' (i.e., whose 'data' is not null)
	// Must also be taken into consideration.
	for (HALFWORD sh = 0; sh < elf->header.e_shnum; sh++)
	{
		// If no data, skip
		if (elf->sections[sh]->data == NULL)
			continue;

		// Calculate where the end of this section would be.
		WORD finalOffset = elf->sections[sh]->header.sh_offset + elf->sections[sh]->header.sh_size;

		// The file size must be adjusted if the final offset is greater than file length
		if (finalOffset > size)
			size = finalOffset;
	}

	return size;
}

WORD BBP::Services::Binaries::ELF::calculateHeaderSize(ELFBuilder *elf)
{
	// Start at end of elf header
	WORD address = elf->header.e_ehsize;

	// Since this is where the segment headers will be, update e_sh, but only if there are actually segments present
	if (elf->header.e_phnum)
		elf->header.e_phoff = address;
	else
		elf->header.e_phoff = 0;

	// For each program header, add 'e_phentsize'
	address += elf->header.e_phnum * elf->header.e_phentsize;

	// Since this is where the section headers will be, update e_sh, but only if there are actually sections present
	if(elf->header.e_shnum)
		elf->header.e_shoff = address;
	else
		elf->header.e_shoff = address;

	// For each section header, add 'e_shentsize'
	address += elf->header.e_shnum * elf->header.e_shentsize;

	return address;
}

OFFSET BBP::Services::Binaries::ELF::calculateProgramHeaderOffsets(ELFBuilder *elf)
{
	// Start at end of all headers
	OFFSET address = calculateHeaderSize(elf);

	// Calculate each offset for headers
	for (HALFWORD ph = 0; ph < elf->header.e_phnum; ph++)
	{
		// If data is null, skip, since the caller manually set the program stuff
		if (elf->segments[ph]->data == nullptr)
			continue;

		// Set offset to current address
		elf->segments[ph]->header.offset = address;

		// Increment address by physical size of contents
		address += elf->segments[ph]->header.filesz;
	}

	// Also calculate offsets for sections with data
	for (HALFWORD sh = 0; sh < elf->header.e_shnum; sh++)
	{
		// If section has no data, skip
		if (elf->sections[sh]->data == NULL)
			continue;

		// Set offset to current address
		elf->sections[sh]->header.sh_offset = address;

		// Increment address by size of section
		address += elf->sections[sh]->header.sh_size;

	}

	return address;
}


unsigned char *BBP::Services::Binaries::ELF::emitHeader(ELFBuilder *elf)
{
	// Get header size
	WORD size = calculateHeaderSize(elf);

	// Create array with size 'size'
	unsigned char *data = (unsigned char *)calloc(size, sizeof(unsigned char));

	// 		-- EMIT MAGIC
	for (int i = 0; i < 16; i++)
		data[i] = elf->header.e_ident[i];

	// 		-- EMIT DATA
	setHALFWORD(data, 16, elf->header.e_type);
	setHALFWORD(data, 18, elf->header.e_machine);
	setWORD(data, 20, elf->header.e_version);
	setWORD(data, 24, elf->header.e_entry);
	setWORD(data, 28, elf->header.e_phoff);
	setWORD(data, 32, elf->header.e_shoff);
	setWORD(data, 36, elf->header.e_flags);
	setHALFWORD(data, 40, elf->header.e_ehsize);
	setHALFWORD(data, 42, elf->header.e_phentsize);
	setHALFWORD(data, 44, elf->header.e_phnum);
	setHALFWORD(data, 46, elf->header.e_shentsize);
	setHALFWORD(data, 48, elf->header.e_shnum);
	setHALFWORD(data, 50, elf->header.e_shstrndx);

	WORD address = 52;

	// Append program header data
	for (HALFWORD ph = 0; ph < elf->header.e_phnum; ph++)
	{
		// Get program header data
		unsigned char *phData = emitProgramHeader(elf, elf->segments[ph]);

		for (HALFWORD i = 0; i < elf->header.e_phentsize; i++)
			data[address++] = phData[i];

		// Free alloc'ed data
		free (phData);
	}

	// Append section header data
	for (HALFWORD sh = 0; sh < elf->header.e_shnum; sh++)
	{
		// Get section header data
		unsigned char *shData = emitSectionHeader(elf, elf->sections[sh]);

		for (HALFWORD i = 0; i < elf->header.e_shentsize; i++)
			data[address++] = shData[i];

		// Free alloc'ed data
		free (shData);
	}

	return data;
}

unsigned char *BBP::Services::Binaries::ELF::emitProgramHeader(ELFBuilder *elf, segmentBuilder *segment)
{
	// Create buffer of size 'e_phentsize'
	unsigned char *buffer = (unsigned char *)calloc(elf->header.e_phentsize, sizeof(unsigned char));

	// 		-- EMIT DATA
	setWORD(buffer, 0, segment->header.type);
	setWORD(buffer, 4, segment->header.offset);
	setWORD(buffer, 8, segment->header.vaddr);
	setWORD(buffer, 12, segment->header.paddr);
	setWORD(buffer, 16, segment->header.filesz);
	setWORD(buffer, 20, segment->header.memsz);
	setWORD(buffer, 24, segment->header.flags);
	setWORD(buffer, 28, segment->header.align);

	// Return buffer
	return buffer;
}

unsigned char *BBP::Services::Binaries::ELF::emitSectionHeader(ELFBuilder *elf, sectionBuilder *section)
{
	// Create buffer of size 'e_shentsize'
	unsigned char *buffer = (unsigned char *)calloc(elf->header.e_shentsize, sizeof(unsigned char));

	// 		-- EMIT DATA
	setWORD(buffer, 0, section->header.sh_name);
	setWORD(buffer, 4, section->header.sh_type);
	setWORD(buffer, 8, section->header.sh_flags);
	setWORD(buffer, 12, section->header.sh_addr);
	setWORD(buffer, 16, section->header.sh_offset);
	setWORD(buffer, 20, section->header.sh_size);
	setWORD(buffer, 24, section->header.sh_link);
	setWORD(buffer, 28, section->header.sh_info);
	setWORD(buffer, 32, section->header.sh_addralign);
	setWORD(buffer, 36, section->header.sh_entsize);

	// Return buffer
	return buffer;
}

WORD BBP::Services::Binaries::ELF::calculateStringTableLength(stringTable *table)
{
	WORD totalLength = 0;
	for (unsigned int strIndex = 0; strIndex < table->stringCount; strIndex++)
		totalLength += (table->lengths[strIndex] + 1);
	return totalLength;
}

unsigned char *BBP::Services::Binaries::ELF::emitStringTable(stringTable *table)
{
	// Calculate total length
	WORD totalLength = calculateStringTableLength(table);

	// Create buffer of length 'totalLength'
	unsigned char *buffer = (unsigned char *)calloc(totalLength, sizeof(unsigned char));

	// Aux indexer
	unsigned int position = 0;

	// Copy over data
	for (unsigned int strIndex = 0; strIndex < table->stringCount; strIndex++)
		for (unsigned int charIndex = 0; charIndex < table->lengths[strIndex]; charIndex++)
			buffer[position++] = (charIndex + 1 == table->lengths[strIndex]) ? 0 : (table->strings[strIndex])[charIndex];

	// Return buffer
	return buffer;
}

unsigned char *BBP::Services::Binaries::ELF::emitStringTableWithNull(stringTable *table)
{
	// Calculate total length
	WORD totalLength = calculateStringTableLength(table) + 2;

	// Create buffer of length 'totalLength'
	unsigned char *buffer = (unsigned char *)calloc(totalLength, sizeof(unsigned char));

	// Aux indexer
	unsigned int position = 1;

	// Copy over data
	for (unsigned int strIndex = 0; strIndex < table->stringCount; strIndex++)
		for (unsigned int charIndex = 0; charIndex < table->lengths[strIndex]; charIndex++)
			buffer[position++] = (table->strings[strIndex])[charIndex];

	// Set null character
	buffer[0] = 0;
	buffer[position] = 0;

	// Return buffer
	return buffer;
}


unsigned char *BBP::Services::Binaries::ELF::emitSymbol(SymbolEntry *symbol)
{
	unsigned char *buffer = (unsigned char *)calloc(16, sizeof(unsigned char));

	setWORD(buffer, 0, symbol->st_name);
	setWORD(buffer, 4, symbol->st_value);
	setWORD(buffer, 8, symbol->st_size);
	buffer[12] = symbol->st_info;
	buffer[13] = symbol->st_other;
	setHALFWORD(buffer, 14, symbol->st_shndx);

	return buffer;
}

void BBP::Services::Binaries::ELF::emitELFAuxHeaders(ELFBuilder *elf)
{
	// If already emitted, don't emit again
	if(elf->emittedAuxLibs)
		return;

	// Get size of symbol table size
	WORD symbolTableSize = elf->symbolCount * 16;

	// Create buffer with that size
	unsigned char *symbTableData = (unsigned char *)calloc(symbolTableSize, sizeof(unsigned char));

	// Copy over data from emitted symbols
	for (unsigned int symb = 0; symb < elf->symbolCount; symb++)
	{
		// Get data
		unsigned char *symbData = emitSymbol(elf->symbols[symb]);

		// Copy data
		for(int i = 0; i < 16; i++)
			symbTableData[i+(symb+0)*16] = symbData[i];

		// Free data
		free(symbData);
	}

	// create symbol table thing
	sectionBuilder *symb_table = createSection(".symbtab", symbTableData, SHT_SYMTAB, symbolTableSize, true);
	symb_table->header.sh_entsize = 16;
	addELFSection(elf, symb_table);

	// 		-- GENERIC STRTABLE
	if(elf->genericStrings != NULL && elf->genericStrings->stringCount)
	{
		unsigned char *gen_data = emitStringTable(elf->genericStrings);
		WORD gen_length = calculateStringTableLength(elf->genericStrings);

		sectionBuilder *gen_table = createSection(".strtab", gen_data, SHT_STRTAB, gen_length, true);
		addELFSection(elf, gen_table);
	}
	else
	{
		sectionBuilder *gen_table = createSection(".strtab", NULL, SHT_STRTAB, 0, false);
		addELFSection(elf, gen_table);
	}

	symb_table->header.sh_link = elf->header.e_shnum - 1;
	symb_table->header.sh_info = elf->header.e_shnum;

	// 		-- SHSTRNDX table
	sectionBuilder *SHSTRNDX_table = createSection(".shstrndx", NULL, SHT_STRTAB, 0, true);
	addELFSection(elf, SHSTRNDX_table);

	unsigned char *SHSTRNDX_data = emitStringTable(&elf->shstrndx);
	WORD SHSTRNDX_length = calculateStringTableLength(&elf->shstrndx);
	SHSTRNDX_table->data = SHSTRNDX_data;
	SHSTRNDX_table->header.sh_size = SHSTRNDX_length;

	elf->header.e_shstrndx = elf->header.e_shnum - 1;

	elf->emittedAuxLibs = 1;
}

unsigned char *BBP::Services::Binaries::ELF::emitELF(ELFBuilder *elf)
{
	// Calculate header size
	WORD headerLength = 0;

	// Calculate program header content offsets
	calculateProgramHeaderOffsets(elf);

	// Now that the offsets have been calculated, calculate file length
	WORD fileLength = calculateFileSize(elf);

	// Create buffer
	unsigned char *buffer = (unsigned char *)calloc(fileLength, sizeof(unsigned char));

	// Size may have been invalidated. Recalculate.
	headerLength = calculateHeaderSize(elf);

	// Copy over data from header into buffer
	unsigned char *headerData = emitHeader(elf);
	for (WORD i = 0; i < headerLength; i++)
		buffer[i] = headerData[i];

	// Free header buffer
	free (headerData);

	// Now insert the data per program header
	for (HALFWORD ph = 0; ph < elf->header.e_phnum; ph++)
	{
		unsigned char *data = elf->segments[ph]->data;
		WORD length = elf->segments[ph]->header.filesz;
		WORD offset = elf->segments[ph]->header.offset;

		if (data != nullptr)
			for (WORD i = 0; i < length; i++)
				buffer[offset+i] = data[i];
	}

	// Now insert the data per section header, if data is present
	for (HALFWORD sh = 0; sh < elf->header.e_shnum; sh++)
	{
		// If no data present, continue
		if (elf->sections[sh]->data == NULL)
			continue;

		// Data is present. Copy over data.
		unsigned char *data = elf->sections[sh]->data;
		WORD length = elf->sections[sh]->header.sh_size;
		WORD offset = elf->sections[sh]->header.sh_offset;

		for (WORD i = 0; i < length; i++)
			buffer[offset+i] = data[i];
	}

	return buffer;
}

void BBP::Services::Binaries::ELF::normalizeELF(ELFBuilder *elf)
{
	// Emit aux headers
	if (!elf->emittedAuxLibs)
		emitELFAuxHeaders(elf);

	// Update headers, just in case
	(void)calculateProgramHeaderOffsets(elf);

	// emit ELF
	unsigned char *binaryData = emitELF(elf);

	// Get file size
	WORD fileSize = calculateFileSize(elf);

	// Repair 
	fixAllDamage(elf, binaryData);

	free(binaryData);
}

void BBP::Services::Binaries::ELF::saveELFFile(ELFBuilder *elf, const char *at)
{
	// emit ELF
	unsigned char *binaryData = emitELF(elf);

	// Get file size
	WORD fileSize = calculateFileSize(elf);

	// Write data from elf into file
	BBP::IO::SDMMC::writeFile(at, binaryData, fileSize);

	// Free ELF binary
	free (binaryData);
}
