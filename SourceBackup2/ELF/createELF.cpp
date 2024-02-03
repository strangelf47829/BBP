#include "ELFUtils.h"

using namespace BBP::Services::Binaries::ELF;
using stringTable = BBP::IO::Buffers::stringTable;

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