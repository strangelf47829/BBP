#include "ELFUtils.h"
#include "Kernel.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;

ELFBuilder *BBP::Services::Binaries::ELF::readELF(unsigned char **file, const char *at)
{
	ELFBuilder *elf;

	BBP::IO::File::FILE_HANDLE FILE = BBP::IO::SDMMC::readFile(at);

	unsigned int length = FILE.byteCount;
	unsigned char *data = FILE.fileVector;

	elf = readELF(data, length);

	if (file == nullptr)
		free(data);
	else
		*file = data;

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

	// Since we are going to overwrite this anyway, clear the NULL segment
	BBP::Services::Binaries::ELF::setELFSectionCount(elf, 0);

	// Read magic
	for (uint32_t i = 0; i < 16; i++)
		elf->header.e_ident[i] = data[i];

	elf->is64bit = elf->header.e_ident[4] == 2;

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
	for (HALFWORD ph = 0; ph < phnum; ph++)
		(void) readELFProgramHeader(data, elf, elf->header.e_phoff + ph * elf->header.e_phentsize, ph);

	// Read section headers
	for (HALFWORD sh = 0; sh < shnum; sh++)
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

	HALFWORD symbstrtab = 0;

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

		symbstrtab = sh;
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

	// Copy over names of sections
	if (elf->header.e_shstrndx < elf->header.e_shnum && elf->header.e_shoff != 0)
	{
		// For each section:
		for (unsigned int sh = 0; sh < elf->header.e_shnum; sh++)
		{
			// Get offset
			unsigned int offset = elf->sections[elf->header.e_shstrndx]->header.sh_offset + elf->sections[sh]->header.sh_name;

			// Get char
			unsigned char *name = data + offset;

			// Calculate size
			for (elf->sections[sh]->nameLength = 0; name[elf->sections[sh]->nameLength++] != 0;);

			// Set buffer
			elf->sections[sh]->name = (char *)BBP::Services::c_alloc(elf->sections[sh]->nameLength, sizeof(char));

			// Copy data
			for (unsigned int k = 0; k < elf->sections[sh]->nameLength; k++)
				elf->sections[sh]->name[k] = name[k];
			elf->sections[sh]->name[elf->sections[sh]->nameLength - 1] = 0;

		}
	}

	// Since ELF aux headers have also been read, turn off aux lib emitting
	elf->emittedAuxLibs = true;

	// Return ELF.
	return elf;
}

uint64_t BBP::Services::Binaries::ELF::readEightBytesLSB(unsigned char *data, uint32_t offset)
{
	uint8_t LSLSB = data[offset + 0]; // Least significant Least significant byte
	uint8_t LSKSB = data[offset + 1]; // Least significant Kinda significant byte
	uint8_t LSSSB = data[offset + 2]; // Least significant Somewhat significant byte
	uint8_t LSMSB = data[offset + 3]; // Least significant Most significant byte
	uint8_t MSLSB = data[offset + 4]; // Most significant Least significant byte
	uint8_t MSKSB = data[offset + 5]; // Most significant Kinda significant byte
	uint8_t MSSSB = data[offset + 6]; // Most significant Somewhat significant byte
	uint8_t MSMSB = data[offset + 7]; // Most significant Most significant byte

	uint64_t LL = (LSMSB << 24) | (LSSSB << 16) | (LSKSB << 8) | LSLSB;
	uint64_t MM = (MSMSB << 24) | (MSSSB << 16) | (MSKSB << 8) | MSLSB;

	uint64_t RR = (MM << 32) | LL;

	return RR;
}

uint64_t BBP::Services::Binaries::ELF::readEightBytesMSB(unsigned char *data, uint32_t offset)
{
	uint8_t LSLSB = data[offset + 7]; // Least significant Least significant byte
	uint8_t LSKSB = data[offset + 6]; // Least significant Kinda significant byte
	uint8_t LSSSB = data[offset + 5]; // Least significant Somewhat significant byte
	uint8_t LSMSB = data[offset + 4]; // Least significant Most significant byte
	uint8_t MSLSB = data[offset + 3]; // Most significant Least significant byte
	uint8_t MSKSB = data[offset + 2]; // Most significant Kinda significant byte
	uint8_t MSSSB = data[offset + 1]; // Most significant Somewhat significant byte
	uint8_t MSMSB = data[offset + 0]; // Most significant Most significant byte

	uint64_t LL = (LSMSB << 24) | (LSSSB << 16) | (LSKSB << 8) | LSLSB;
	uint64_t MM = (MSMSB << 24) | (MSSSB << 16) | (MSKSB << 8) | MSLSB;

	uint64_t RR = (MM << 32) | LL;

	return RR;
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

bool BBP::Services::Binaries::ELF::isELF(mem_t *data)
{
	// Read first 4 bytes
	uint32_t magic = readFourBytesMSB(data, 0);

	return magic == 0x7f454c46;
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

	if (elf->is64bit)
	{
		elf->header.e_type = readTwoBytesLSB(file, 16);
		elf->header.e_machine = readTwoBytesLSB(file, 18);
		elf->header.e_version = readFourBytesLSB(file, 20);
		elf->header.e_entry = readEightBytesLSB(file, 24);
		elf->header.e_phoff = readEightBytesLSB(file, 32);
		elf->header.e_shoff = readEightBytesLSB(file, 40);
		elf->header.e_flags = readFourBytesLSB(file, 48);
		elf->header.e_ehsize = readTwoBytesLSB(file, 52);
		elf->header.e_phentsize = readTwoBytesLSB(file, 54);
		elf->header.e_phnum = readTwoBytesLSB(file, 56);
		elf->header.e_shentsize = readTwoBytesLSB(file, 58);
		elf->header.e_shnum = readTwoBytesLSB(file, 60);
		elf->header.e_shstrndx = readTwoBytesLSB(file, 62);
	}
	else
	{
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
	}

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

	// Fucking dumbass...... You yourself commented that this array is initalized with nullptrs. Of course its gonna fail! dumb fuck.
	if (elf->segments[index] == nullptr)
		elf->segments[index] = new BBP::Services::Binaries::ELF::segmentBuilder();

	if (elf->is64bit)
	{
		// Read fields
		elf->segments[index]->header.type = readFourBytesLSB(file, offset + 0);
		elf->segments[index]->header.flags = readFourBytesLSB(file, offset + 4);
		elf->segments[index]->header.offset = readFourBytesLSB(file, offset + 8);
		elf->segments[index]->header.vaddr = readFourBytesLSB(file, offset + 12);
		elf->segments[index]->header.paddr = readFourBytesLSB(file, offset + 16);
		elf->segments[index]->header.filesz = readFourBytesLSB(file, offset + 20);
		elf->segments[index]->header.memsz = readFourBytesLSB(file, offset + 24);
		elf->segments[index]->header.align = readFourBytesLSB(file, offset + 28);
	}
	else
	{
		// Read fields
		elf->segments[index]->header.type = readFourBytesLSB(file, offset + 0);
		elf->segments[index]->header.offset = readFourBytesLSB(file, offset + 4);
		elf->segments[index]->header.vaddr = readFourBytesLSB(file, offset + 8);
		elf->segments[index]->header.paddr = readFourBytesLSB(file, offset + 12);
		elf->segments[index]->header.filesz = readFourBytesLSB(file, offset + 16);
		elf->segments[index]->header.memsz = readFourBytesLSB(file, offset + 20);
		elf->segments[index]->header.flags = readFourBytesLSB(file, offset + 24);
		elf->segments[index]->header.align = readFourBytesLSB(file, offset + 28);
	}

	// Create data buffer
	elf->segments[index]->data = (unsigned char *)calloc(elf->segments[index]->header.filesz, sizeof(unsigned char));

	// Mark auto-dealloc
	elf->segments[index]->autoDealloc = true;

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
		elf->sections[index] = new BBP::Services::Binaries::ELF::sectionBuilder();

	if (elf->is64bit)
	{
		// Read fields
		elf->sections[index]->header.sh_name = readFourBytesLSB(file, offset + 0);
		elf->sections[index]->header.sh_type = readFourBytesLSB(file, offset + 4);
		elf->sections[index]->header.sh_flags = readEightBytesLSB(file, offset + 8);
		elf->sections[index]->header.sh_addr = readEightBytesLSB(file, offset + 16);
		elf->sections[index]->header.sh_offset = readEightBytesLSB(file, offset + 24);
		elf->sections[index]->header.sh_size = readEightBytesLSB(file, offset + 32);
		elf->sections[index]->header.sh_link = readFourBytesLSB(file, offset + 40);
		elf->sections[index]->header.sh_info = readFourBytesLSB(file, offset + 44);
		elf->sections[index]->header.sh_addralign = readEightBytesLSB(file, offset + 48);
		elf->sections[index]->header.sh_entsize = readEightBytesLSB(file, offset + 56);
	}
	else
	{
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
	}

	

	// Create data buffer
	elf->sections[index]->data = (unsigned char *)calloc(elf->sections[index]->header.sh_size, sizeof(unsigned char));

	// Mark auto-dealloc
	elf->sections[index]->autoDealloc = true;

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

		if (elf->is64bit)
		{
			symbl->st_name = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 0);
			symbl->st_info = file[offset + symb * symtable->header.sh_entsize + 4];
			symbl->st_other = file[offset + symb * symtable->header.sh_entsize + 5];
			symbl->st_shndx = readTwoBytesLSB(file, offset + symb * symtable->header.sh_entsize + 6);
			symbl->st_value = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 8);
			symbl->st_size = readEightBytesLSB(file, offset + symb * symtable->header.sh_entsize + 12);
		}
		else
		{
			symbl->st_name = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 0);
			symbl->st_value = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 4);
			symbl->st_size = readFourBytesLSB(file, offset + symb * symtable->header.sh_entsize + 8);
			symbl->st_info = file[offset + symb * symtable->header.sh_entsize + 12];
			symbl->st_other = file[offset + symb * symtable->header.sh_entsize + 13];
			symbl->st_shndx = readTwoBytesLSB(file, offset + symb * symtable->header.sh_entsize + 14);
		}
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