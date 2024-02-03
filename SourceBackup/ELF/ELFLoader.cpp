#include "ELFUtils.h"
#include <sys/mman.h>


using namespace BBP::Services::Binaries::ELF::Runtime;

static inline uint64_t page_align(uint64_t n)
{
	return (n + (4096 - 1)) & ~(4096 - 1);
}

Executable *BBP::Services::Binaries::ELF::Runtime::E_LoadExecutable(const char *at)
{
	ELFBuilder *elf;

	BBP::IO::File::FILE_HANDLE FILE = BBP::IO::SDMMC::readFile(at);

	unsigned int length = FILE.byteCount;
	unsigned char *data = FILE.fileVector;

	elf = BBP::Services::Binaries::ELF::readELF(data, length);

	Executable *executable = E_LoadExecutable(data, elf);
	executable->fileSize = length;

	free(FILE.fileName);
	return executable;
}

Executable *BBP::Services::Binaries::ELF::Runtime::E_LoadExecutable(unsigned char *file, BBP::Services::Binaries::ELF::ELFBuilder *elf)
{
	// Create new executable
	Executable *executable = new Executable();

	// Set executable elf to elf
	executable->elf = elf;

	// Set data to file
	executable->data = file;

	// Get section info
	E_LoadSectionInfo(executable);

	// Now find the RELA 
	BBP::Services::Binaries::ELF::sectionBuilder *relocation = executable->elf->sections[executable->relocationTable];

	if (relocation->header.sh_type == SHT_REL)
	{
		unsigned int entries = relocation->header.sh_size / relocation->header.sh_entsize;

		for (unsigned int k = 0; k < entries; k++)
		{
			RelEntry *entry = new RelEntry({
			BBP::Services::Binaries::ELF::readFourBytesLSB(relocation->data, 0 + 8 * k),
			BBP::Services::Binaries::ELF::readFourBytesLSB(relocation->data, 4 + 8 * k)
			});

			E_doRelocation(executable, entry);

			delete(entry);
		}

	}
	if (relocation->header.sh_type == SHT_RELA)
	{
		unsigned int entries = relocation->header.sh_size / relocation->header.sh_entsize;

		for (unsigned int k = 0; k < entries; k++)
		{
			RelaEntry *entry = new RelaEntry({
			BBP::Services::Binaries::ELF::readEightBytesLSB(relocation->data, 0 + 24 * k),
			BBP::Services::Binaries::ELF::readEightBytesLSB(relocation->data, 8 + 24 * k),
			BBP::Services::Binaries::ELF::readEightBytesLSB(relocation->data, 16 + 24 * k),
				});

			E_doRelocation(executable, entry);

			delete(entry);
		}
	}

	// Create executable memory section
	executable->text = (unsigned char *)mmap(NULL, page_align(executable->elf->sections[executable->textSection]->header.sh_size), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	// Copy .text into this section
	for (unsigned int k = 0; k < executable->elf->sections[executable->textSection]->header.sh_size; k++)
		executable->text[k] = executable->data[k + executable->elf->sections[executable->textSection]->header.sh_offset];

	// Now make executable
	mprotect(executable->text, page_align(executable->elf->sections[executable->textSection]->header.sh_size), PROT_READ | PROT_EXEC);

	// Load functions
	E_readFunctions(executable);

	return executable;
}

void BBP::Services::Binaries::ELF::Runtime::E_doRelocation(BBP::Services::Binaries::ELF::Runtime::Executable *executable, RelaEntry *relocation)
{
	// Decode info on relocations
	WORD relocationType = relocation->r_info & 0xFFFF;
	WORD symbol = relocation->r_info >> 32;
	DWORD offset = relocation->r_offset;

	// Where the relocation happens
	unsigned int sectionAt = executable->elf->sections[executable->elf->symbols[symbol]->st_shndx]->header.sh_offset;
	unsigned char *_toRelocate = executable->data + sectionAt + offset;

	unsigned int REL = BBP::Services::Binaries::ELF::readFourBytesLSB(_toRelocate, 0);

	switch (relocationType)
	{
	case 4:
	{
		unsigned int S = executable->elf->symbols[symbol]->st_value;
		int A = relocation->r_addend;
		unsigned int P = offset;

		REL = S + A - P;

		break;
	}
	default:
		printf("Relocation type 0x%02x not supported.\n", relocationType);
		return;
	}


	setWORD(_toRelocate, 0, REL);

}

void BBP::Services::Binaries::ELF::Runtime::E_doRelocation(BBP::Services::Binaries::ELF::Runtime::Executable *executable, RelEntry *relocation)
{
	// Decode info on relocations
	HALFWORD relocationType = relocation->r_info & 0xFF;
	HALFWORD symbol = relocation->r_info >> 8;
	OFFSET offset = relocation->r_offset;

	// Where the relocation happens
	unsigned int sectionAt = executable->elf->sections[executable->elf->symbols[symbol]->st_shndx]->header.sh_offset;
	unsigned char *_toRelocate = executable->data + sectionAt + offset;

	unsigned int REL = BBP::Services::Binaries::ELF::readFourBytesLSB(_toRelocate, 0);

	switch (relocationType)
	{
	case R_386_PC32:
	{
		unsigned int S = executable->elf->symbols[symbol]->st_value;
		unsigned int A = 0;
		unsigned int P = offset;

		REL = S + A - P;

		break;
	}
	default:
		printf("Relocation type 0x%02x not supported.\n", relocationType);
		return;
	}
	

	setWORD(_toRelocate, 0, REL);

}

void BBP::Services::Binaries::ELF::Runtime::E_readFunctions(BBP::Services::Binaries::ELF::Runtime::Executable *executable)
{
	// If function table exists, stop reading
	if (executable->loadedFunctions)
	{
		printf("Cannot read functions: function table already populated.\n");
		return;
	}

	// Create function table
	executable->loadedFunctions = BBP::IO::Buffers::createStringTable();

	// Count the amount of functions
	for (unsigned int k = 0; k < executable->elf->symbolCount; k++)
	{
		// If not function, skip.
		if ((executable->elf->symbols[k]->st_info & 0x0F) != 2)
			continue;

		// Print message and count
		executable->funcCount++;

		char *name = E_getSymbolName(executable, k);
		BBP::IO::Buffers::addStringToTable(executable->loadedFunctions, name);
	}

	// Create buffer
	executable->funcs = (void **)BBP::Services::c_alloc(executable->funcCount, sizeof(void *));
	unsigned int loadedFuncs = 0;

	// Load functions
	// Count the amount of functions
	for (unsigned int k = 0; k < executable->elf->symbolCount; k++)
	{
		// If not function, skip.
		if ((executable->elf->symbols[k]->st_info & 0x0F) != 2)
			continue;

		// Calculate function offsets
		OFFSET functionOffset = executable->elf->symbols[k]->st_value;
		unsigned char *functionAddress = executable->text + functionOffset;

		executable->funcs[loadedFuncs++] = (void *)functionAddress;
	}

}

unsigned int BBP::Services::Binaries::ELF::Runtime::E_getSymbolIndex(BBP::Services::Binaries::ELF::Runtime::Executable *executable, const char *symbol)
{
	// Search for symbol
	for (unsigned int str = 0; str < executable->elf->symbolCount; str++)
	{
		char *name = E_getSymbolName(executable, str);

		// Loop for each character
		for (unsigned int idx = 0;name[idx]; idx++)
		{
			// If character does not match expected value, go to next string
			if (name[idx] != symbol[idx])
				goto cont_symb_search;
		}

		// Something was found, return str function
		return str;

	cont_symb_search:
		continue;

	}
	return executable->elf->symbolCount;
}

unsigned int BBP::Services::Binaries::ELF::Runtime::E_getFunctionIndex(BBP::Services::Binaries::ELF::Runtime::Executable *executable, const char *function)
{
	// Search for symbol
	for (unsigned int str = 0; str < executable->loadedFunctions->stringCount; str++)
	{
		// Loop for each character
		for (unsigned int idx = 0; idx < executable->loadedFunctions->lengths[str]; idx++)
		{
			// If character does not match expected value, go to next string
			if ((executable->loadedFunctions->strings[str])[idx] != function[idx])
				goto cont_func_search;
		}

		// Something was found, return str function
		return str;

	cont_func_search:
		continue;

	}
	return executable->loadedFunctions->stringCount;
}

void *BBP::Services::Binaries::ELF::Runtime::E_getFunction(BBP::Services::Binaries::ELF::Runtime::Executable *executable, const char *function)
{
	// If table does not exist, return nullptr
	if (executable->loadedFunctions == nullptr)
		return nullptr;

	unsigned int funcIndex = E_getFunctionIndex(executable, function);

	if (funcIndex >= executable->loadedFunctions->stringCount)
		return nullptr;

	return executable->funcs[funcIndex];
	
}

void BBP::Services::Binaries::ELF::Runtime::E_LoadSectionInfo(BBP::Services::Binaries::ELF::Runtime::Executable *executable)
{
	// Get section information
	for (unsigned int sh = 0; sh < executable->elf->header.e_shnum; sh++)
	{
		// Get section handle
		BBP::Services::Binaries::ELF::sectionBuilder *section = executable->elf->sections[sh];

		// Do things based on switch
		switch (section->header.sh_type)
		{
		case SHT_PROGBITS:
			if (executable->textSection == 0)
				executable->textSection = sh;
			break;
		case SHT_REL:
			if (executable->relocationTable == 0)
				executable->relocationTable = sh;
			break;
		case SHT_RELA:
			if (executable->relocationTable == 0)
				executable->relocationTable = sh;
			break;
		case SHT_SYMTAB:
			executable->symbolNameTable = section->header.sh_link;
			break;
		}
	}
}

char *BBP::Services::Binaries::ELF::Runtime::E_getSymbolName(BBP::Services::Binaries::ELF::Runtime::Executable *executable, unsigned int index)
{
	return (char *)executable->elf->sections[executable->symbolNameTable]->data + executable->elf->symbols[index]->st_name;
}

void BBP::Services::Binaries::ELF::Runtime::E_DestroyExecutable(BBP::Services::Binaries::ELF::Runtime::Executable *executable)
{
	BBP::Services::Binaries::ELF::destroyELF(executable->elf);

	if (executable->funcCount)
		BBP::Services::Free(executable->funcs);
	executable->funcCount = 0;

	BBP::IO::Buffers::setStringTableSize(executable->loadedFunctions, 0);
	delete(executable->loadedFunctions);

	delete(executable);
}