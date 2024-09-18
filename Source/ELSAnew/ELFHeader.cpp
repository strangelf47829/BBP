#include "../include/ELSA/ELFHeader.h"
#include "../include/stdio.h"
#include "../include/stdint.h"

// ELF Magic bytes:
const BBP::std::byte ELFMagic[4] = { 0x7f, 'E', 'L', 'F' };

// ELFHeader constructor
BBP::ELF::ELFHeader::ELFHeader()
	:ident(&headerData),
	e_type(ET_NONE),
	e_machine(EM_NONE),
	e_version(1),
	e_entry(0),
	e_flags(0),
	e_ehsize(52),
	e_phoff(0),
	e_shoff(0),
	e_phentsize(0),
	e_shentsize(0),
	e_phnum(0),
	e_shnum(0),
	e_shstrndx(0)
{ }

// Verifies the ident field
bool BBP::ELF::ELFHeader::verifyMagic()
{
	// Read data
	readHeaderFields();

	// Verify magic bytes
	for (std::index_t idx = 0; idx < 4; idx++)
		if (headerData[idx] != ELFMagic[idx])
			return false;

	// If version is invalid, return false
	if (e_version != 1)
		return false;

	// If flags is set, return false
	if (e_flags)
		return false;

	// Read Ident, returns true if successful, false if not.
	return ident.readData();
}


BBP::std::byte &BBP::ELF::ELFHeader::operator[] (std::index_t idx)
{
	return headerData[idx];
}

// Read data
void BBP::ELF::ELFHeader::readHeaderFields()
{
	// Read ident (which also sets endiannes)
	ident.readData();

	// Since data is being passed in big endian format, logic needs to be reversed
	bool bigEndian = !ident.littleEndian;

	// Type is halfword at offset 16
	e_type = (ELFHeaderType)std::stitch(headerData[16], headerData[17], bigEndian);

	// Machine is halfword at offset 18
	e_machine = (ELFHeaderMachine)std::stitch(headerData[18], headerData[19], bigEndian);

	// Version is word at offset 20
	e_version = std::stitch(headerData[20], headerData[21], headerData[22], headerData[23], bigEndian);

	// Entry is word at offset 24
	e_entry = std::stitch(headerData[24], headerData[25], headerData[26], headerData[27], bigEndian);

	// Phoff is word at offset 28
	e_phoff = std::stitch(headerData[28], headerData[29], headerData[30], headerData[31], bigEndian);

	// Shoff is word at offset 32
	e_shoff = std::stitch(headerData[32], headerData[33], headerData[34], headerData[35], bigEndian);

	// Flags is word at offset 36
	e_flags = std::stitch(headerData[36], headerData[37], headerData[38], headerData[39], bigEndian);

	// EHsize is halfword at offset 40
	e_ehsize = std::stitch(headerData[40], headerData[41], bigEndian);

	// phentsize is halfword at offset 42
	e_phentsize = std::stitch(headerData[42], headerData[43], bigEndian);

	// phnum is halfword at offset 44
	e_phnum = std::stitch(headerData[44], headerData[45], bigEndian);

	// shentsize is halfword at offset 46
	e_shentsize = std::stitch(headerData[46], headerData[47], bigEndian);

	// shnum is halfword at offset 48
	e_shnum = std::stitch(headerData[48], headerData[49], bigEndian);

	// shstrndx is halfword at offset 50
	e_shstrndx = std::stitch(headerData[50], headerData[51], bigEndian);

}

// Write data
void BBP::ELF::ELFHeader::writeHeaderFields()
{
	// Write ident data
	ident.writeData();

	// Since data is being passed in big endian format, logic needs to be reversed
	bool bigEndian = !ident.littleEndian;

	// Type is halfword at offset 16
	std::split(e_type, headerData[16], headerData[17], bigEndian);

	// Machine is halfword at offset 18
	std::split(e_machine, headerData[18], headerData[19], bigEndian);

	// Version is word at offset 20
	std::split(e_version, headerData[20], headerData[21], headerData[22], headerData[23], bigEndian);

	// Entry is word at offset 24
	std::split(e_entry,headerData[24], headerData[25], headerData[26], headerData[27], bigEndian);

	// Phoff is word at offset 28
	std::split(e_phoff,headerData[28], headerData[29], headerData[30], headerData[31], bigEndian);

	// Shoff is word at offset 32
	std::split(e_shoff,headerData[32], headerData[33], headerData[34], headerData[35], bigEndian);

	// Flags is word at offset 36
	std::split(e_flags,headerData[36], headerData[37], headerData[38], headerData[39], bigEndian);

	// EHsize is halfword at offset 40
	std::split(e_ehsize,headerData[40], headerData[41], bigEndian);

	// phentsize is halfword at offset 42
	std::split(e_phentsize,headerData[42], headerData[43], bigEndian);

	// phnum is halfword at offset 44
	std::split(e_phnum,headerData[44], headerData[45], bigEndian);

	// shentsize is halfword at offset 46
	std::split(e_shentsize,headerData[46], headerData[47], bigEndian);

	// shnum is halfword at offset 48
	std::split(e_shnum,headerData[48], headerData[49], bigEndian);

	// shstrndx is halfword at offset 50
	std::split(e_shstrndx,headerData[50], headerData[51], bigEndian);

}