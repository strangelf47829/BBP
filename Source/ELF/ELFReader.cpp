#include "ELFUtils.h"
#include "Kernel.h"

using namespace BBP::Services::Binaries::ELF;

ELFBuilder *BBP::Services::Binaries::ELF::readELF(const char *at)
{
	ELFBuilder *elf;

	BBP::IO::File::FILE_HANDLE FILE = BBP::IO::SDMMC::readFile(at);

	unsigned int length = FILE.byteCount;
	unsigned char *data = FILE.fileVector;

	elf = readELF(data, length);
	return elf;
}

ELFBuilder *BBP::Services::Binaries::ELF::readELF(unsigned char *data, uint32_t fileSize)
{
	if (data == nullptr)
		return nullptr; // Invalid data

	if (fileSize < 54)
		return nullptr; // Invalid file size

	// Create ELF Builder
	ELFBuilder *elf = createELF(0, 0);

	// Read magic
	for (uint32_t i = 0; i < 16; i++)
		elf->header.e_ident[i] = data[i];

	// Verify Header
	ELFVerification headerValid = verifyELF(elf);

	if (headerValid != VALID)
		return nullptr; // Invalid header.

	// Read rest of ELF
	int ELFHeaderStatus = readELFHeader(data, elf);

	// Extend program and section headers based on read data
	HALFWORD phnum = elf->header.e_phnum;
	HALFWORD shnum = elf->header.e_shnum;
	elf->header.e_phnum = 0;
	elf->header.e_shnum = 0;
	setELFSegmentCount(elf, phnum);
	setELFSectionCount(elf, shnum);

	// Read program headers
	for (HALFWORD ph = 0; ph < elf->header.e_phnum; ph++)
		(void) readELFProgramHeader(data, elf, elf->header.e_phoff + ph * elf->header.e_phentsize, ph);

	// Read section headers
	for (HALFWORD sh = 0; sh < elf->header.e_shnum; sh++)
		(void) readELFSectionHeader(data, elf, elf->header.e_shoff + sh * elf->header.e_shentsize, sh);

	// If shstrndx is valid, read it
	if (elf->header.e_shstrndx < elf->header.e_shnum && elf->header.e_shoff != 0)
	{
		// Get section handle
		sectionBuilder *section = elf->sections[elf->header.e_shstrndx];

		// Read
		int shstrndxStatus = readELFStringTable(data, section->header.sh_offset, section->header.sh_size, &elf->shstrndx);

		// Damage control
		if(shstrndxStatus >= 4)
			fixLeadingNullStrings(elf, section, &elf->shstrndx, nullptr);
		
	}

	elf->genericStrings->strings = nullptr;
	elf->genericStrings->lengths = nullptr;

	// Find string table section, and if found, read strings
	for (HALFWORD sh = 0; sh < elf->header.e_shnum; sh++)
	{
		// If not type, just skip
		if (elf->sections[sh]->header.sh_type != SHT_STRTAB)
			continue;

		// Skip string table
		if (sh == elf->header.e_shstrndx)
			continue;

		// Read
		(void) readELFStringTable(data, elf->sections[sh]->header.sh_offset, elf->sections[sh]->header.sh_size, elf->genericStrings);
	}

	// Find symbol table section, and if found, read symbols
	for (HALFWORD sh = 0; sh < elf->header.e_shnum; sh++)
	{
		// If not type, just skip
		if (elf->sections[sh]->header.sh_type != SHT_SYMTAB)
			continue;
	
		// Symbol table found
		int symbolStatus = readELFSymbols(data, elf, sh);
	}

	// Since ELF aux headers have also been read, turn off aux lib emitting
	elf->emittedAuxLibs = true;

	// Return ELF.
	return elf;
}

uint32_t BBP::Services::Binaries::ELF::readFourBytesLSB(unsigned char *data, uint32_t offset)
{
	uint8_t LSB = data[offset + 0]; // Least significant byte
	uint8_t KSB = data[offset + 1]; // Kinda significant byte
	uint8_t SSB = data[offset + 2]; // Somewhat significant byte
	uint8_t MSB = data[offset + 3]; // Most significant byte
	return (MSB << 24) | (SSB << 16) | (KSB << 8) | LSB;
}

uint32_t BBP::Services::Binaries::ELF::readFourBytesMSB(unsigned char *data, uint32_t offset)
{
	uint8_t LSB = data[offset + 3]; // Least significant byte
	uint8_t KSB = data[offset + 2]; // Kinda significant byte
	uint8_t SSB = data[offset + 1]; // Somewhat significant byte
	uint8_t MSB = data[offset + 0]; // Most significant byte
	return (MSB << 24) | (SSB << 16) | (KSB << 8) | LSB;
}

uint16_t BBP::Services::Binaries::ELF::readTwoBytesLSB(unsigned char *data, uint32_t offset)
{
	uint8_t LSB = data[offset + 0]; // Least significant byte
	uint8_t MSB = data[offset + 1]; // Kinda significant byte
	return (MSB << 8) | LSB;
}

uint16_t BBP::Services::Binaries::ELF::readTwoBytesMSB(unsigned char *data, uint32_t offset)
{
	uint8_t LSB = data[offset + 1]; // Least significant byte
	uint8_t MSB = data[offset + 0]; // Kinda significant byte
	return (MSB << 8) | LSB;
}

ELFVerification BBP::Services::Binaries::ELF::verifyELF(ELFBuilder *ELF)
{
	ELFVerification flags = VALID;

	// Read first 4 bytes
	uint32_t magic = readFourBytesMSB(ELF->header.e_ident, 0);

	// Verify first 4 bytes
	if (magic != 0x7f454c46)
		flags = (ELFVerification)(flags | INVALID_MAGIC); 

	// Return verification data
	return flags;
}

int BBP::Services::Binaries::ELF::readELFHeader(unsigned char *file, ELFBuilder *elf)
{
	// If file is invalid, return 1
	if (file == nullptr)
		return 1;

	// If ELF invalid, return 2
	if (elf == nullptr)
		return 2;

	// Read magic
	for (int i = 0; i < 16; i++)
		elf->header.e_ident[i];

	// Get file data
	elf->header.e_type = readTwoBytesLSB(file, 16);
	elf->header.e_machine = readTwoBytesLSB(file, 18);
	elf->header.e_version = readFourBytesLSB(file, 20);
	elf->header.e_entry = readFourBytesLSB(file, 24);
	elf->header.e_phoff = readFourBytesLSB(file, 28);
	elf->header.e_shoff = readFourBytesLSB(file, 32);
	elf->header.e_flags = readFourBytesLSB(file, 36);
	elf->header.e_ehsize = readTwoBytesLSB(file, 40);
	elf->header.e_phentsize = readTwoBytesLSB(file, 42);
	elf->header.e_phnum = readTwoBytesLSB(file, 44);
	elf->header.e_shentsize = readTwoBytesLSB(file, 46);
	elf->header.e_shnum = readTwoBytesLSB(file, 48);
	elf->header.e_shstrndx = readTwoBytesLSB(file, 50);
	

	// Done reading
	return 0;
}

int BBP::Services::Binaries::ELF::readELFProgramHeader(unsigned char *file, ELFBuilder *elf, uint32_t offset, uint32_t index)
{
	// Check for invalid file
	if (file == nullptr)
		return 1;
	
	// Check for invalid elf
	if (elf == nullptr)
		return 2;
	
	// Check for invalid offset
	if (offset == 0)
		return 3;
	
	// Check for invalid index
	if (index >= elf->header.e_phnum)
		return 4;
	
	if (elf->segments[index] == nullptr)
		return 5;

	// Read fields
	elf->segments[index]->header.type = readFourBytesLSB(file, offset + 0);
	elf->segments[index]->header.offset = readFourBytesLSB(file, offset + 4);
	elf->segments[index]->header.vaddr = readFourBytesLSB(file, offset + 8);
	elf->segments[index]->header.paddr = readFourBytesLSB(file, offset + 12);
	elf->segments[index]->header.filesz = readFourBytesLSB(file, offset + 16);
	elf->segments[index]->header.memsz = readFourBytesLSB(file, offset + 20);
	elf->segments[index]->header.flags = readFourBytesLSB(file, offset + 24);
	elf->segments[index]->header.align = readFourBytesLSB(file, offset + 28);

	// Create data buffer
	elf->segments[index]->data = (unsigned char *)calloc(elf->segments[index]->header.filesz, sizeof(unsigned char));

	// Copy over data
	for (WORD idx = 0; idx < elf->segments[index]->header.filesz; idx++)
		elf->segments[index]->data[idx] = file[elf->segments[index]->header.offset + idx];

	// Success
	return 0;
	
}

int BBP::Services::Binaries::ELF::readELFSectionHeader(unsigned char *file, ELFBuilder *elf, uint32_t offset, uint32_t index)
{
	// Check for invalid file
	if (file == nullptr)
		return 1;

	// Check for invalid elf
	if (elf == nullptr)
		return 2;

	// Check for invalid offset
	if (offset == 0)
		return 3;

	// Check for invalid index
	if (index >= elf->header.e_shnum)
		return 4;

	if (elf->sections[index] == nullptr)
		return 5;

	// Read fields
	elf->sections[index]->header.sh_name = readFourBytesLSB(file, offset + 0);
	elf->sections[index]->header.sh_type = readFourBytesLSB(file, offset + 4);
	elf->sections[index]->header.sh_flags = readFourBytesLSB(file, offset + 8);
	elf->sections[index]->header.sh_addr = readFourBytesLSB(file, offset + 12);
	elf->sections[index]->header.sh_offset = readFourBytesLSB(file, offset + 16);
	elf->sections[index]->header.sh_size = readFourBytesLSB(file, offset + 20);
	elf->sections[index]->header.sh_link = readFourBytesLSB(file, offset + 24);
	elf->sections[index]->header.sh_info = readFourBytesLSB(file, offset + 28);
	elf->sections[index]->header.sh_addralign = readFourBytesLSB(file, offset + 32);
	elf->sections[index]->header.sh_entsize = readFourBytesLSB(file, offset + 36);

	// Create data buffer
	elf->sections[index]->data = (unsigned char *)calloc(elf->sections[index]->header.sh_size, sizeof(unsigned char));

	// Copy over data
	for (WORD idx = 0; idx < elf->sections[index]->header.sh_size; idx++)
		elf->sections[index]->data[idx] = file[elf->sections[index]->header.sh_offset + idx];

	// Success
	return 0;
	
}

int BBP::Services::Binaries::ELF::readELFSymbols(unsigned char *file, ELFBuilder *elf, uint32_t symtableindex)
{
	// Check file validity
	if (file == nullptr)
		return 1;

	// Check elf validity
	if (elf == nullptr)
		return 2;

	// Check symtable validity
	if (elf->header.e_shnum <= symtableindex || elf->header.e_shoff == 0)
		return 3;

	// Get symtable handle
	sectionBuilder *symtable = elf->sections[symtableindex];

	// Check symtable validity
	if (symtable == nullptr)
		return 4;

	// Check size validity
	if (symtable->header.sh_entsize == 0)
		return 5;

	// Get symbol count
	WORD symbolCount = symtable->header.sh_size / symtable->header.sh_entsize;

	// Get offset
	OFFSET offset = symtable->header.sh_offset;

	for (WORD symb = 0; symb < symbolCount; symb++)
	{
		SymbolEntry *symbl = new SymbolEntry();
		symbl->st_name = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 0);
		symbl->st_value = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 4);
		symbl->st_size = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 8);
		symbl->st_info = file[offset + symb * symtable->header.sh_entsize + 12];
		symbl->st_other = file[offset + symb * symtable->header.sh_entsize + 13];
		symbl->st_shndx = readTwoBytesLSB(file, offset + symb * symtable->header.sh_entsize + 14);

		// Add symbol to ELF
		addELFSymbol(elf, symbl, nullptr);
	}

	bool damagedSymbols = checkForDisorganizedSymbolNames(elf, nullptr);

	// If symbols are ok, return here
	if (!damagedSymbols)
		return 0;

	if(fixDisorganizedSymbolNames(file, elf, symtableindex, nullptr))
		return 0; // Damage fixed
	else
		return 6; // Damage still present
	
}

int BBP::Services::Binaries::ELF::readELFStringTable(unsigned char *file, uint32_t offset, uint32_t length, stringTable *table)
{
	// Check for valid file
	if (file == nullptr)
		return 1;

	// Check for valid length
	if (length == 0)
		return 0; // Technically allowed

	// Check for table validity
	if (table == nullptr)
		return 3;

	// Create buffer with length 'length'
	char *buffer = (char *)calloc(length, sizeof(char));

	// Variable to keep track of current string length
	uint32_t strLength = 0;

	// Variable to keep track of null string amount
	uint32_t nullStrings = 0;

	// Has had non-null
	bool hasHadNonNull = false;

	// Read data
	for (uint32_t index = 0; index < length; index++)
	{
		// Get char
		char currChar = file[index + offset];
		
		// Add char to buffer
		buffer[strLength] = currChar;

		// Increase string length
		strLength++;

		// If character is not null, just continue
		if (currChar)
			continue;

		if (strLength == 1 && !hasHadNonNull)
			nullStrings++;
		else if (strLength > 1)
			hasHadNonNull = true;

		// Null character, so encode string
		if(strLength != 1)
			addStringToTable(table, buffer);

		// set 'strLength' to 0 to 'reset' the buffer
		strLength = 0;
	}

	if (strLength > 1)
	{
		buffer[strLength] = 0;
		addStringToTable(table, buffer);
	}

	// Free the buffer
	free(buffer);

	// Return with success, if acceptable amount of nullstrings found
	if (nullStrings <= 1)
		return 0;

	return 4 + nullStrings;
}