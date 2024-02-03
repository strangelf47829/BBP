#include "../include/ELF.h"

BBP::std::byte readByte(BBP::std::ELF::ELFBuilder *elf, BBP::std::offset_t offset)
{
	return elf->file.data[offset];
}
void writeByte(BBP::std::ELF::ELFBuilder *elf, BBP::std::byte byte, BBP::std::offset_t offset)
{
	elf->file.data[offset] = byte;
}
BBP::std::halfword readHalfWord(BBP::std::ELF::ELFBuilder *elf, BBP::std::offset_t offset)
{
	BBP::std::byte a = elf->file.data[offset];
	BBP::std::byte b = elf->file.data[offset + 1];

	if (!elf->Endian)
		return a | (b << 8);
	return b | (a << 8);
}
void writeHalfWord(BBP::std::ELF::ELFBuilder *elf, BBP::std::halfword halfword, BBP::std::offset_t offset)
{
	BBP::std::byte a = (halfword >> 0) & 0xFF;
	BBP::std::byte b = (halfword >> 8) & 0xFF;

	if (!elf->Endian)
	{
		elf->file.data[offset] = a;
		elf->file.data[offset + 1] = b;
	}
	else
	{
		elf->file.data[offset] = b;
		elf->file.data[offset + 1] = a;
	}
}
BBP::std::word readWord(BBP::std::ELF::ELFBuilder *elf, BBP::std::offset_t offset)
{
	BBP::std::byte a = elf->file.data[offset];
	BBP::std::byte b = elf->file.data[offset + 1];
	BBP::std::byte c = elf->file.data[offset + 2];
	BBP::std::byte d = elf->file.data[offset + 3];

	if (!elf->Endian)
		return a | (b << 8) | (c << 16) | (d << 24);
	return d | (c << 8) | (b << 16) | (a << 24);
}
void writeWord(BBP::std::ELF::ELFBuilder *elf, BBP::std::word word, BBP::std::offset_t offset)
{
	BBP::std::byte a = (word >> 0) & 0xFF;
	BBP::std::byte b = (word >> 8) & 0xFF;
	BBP::std::byte c = (word >> 16) & 0xFF;
	BBP::std::byte d = (word >> 24) & 0xFF;

	if (!elf->Endian)
	{
		elf->file.data[offset] = a;
		elf->file.data[offset + 1] = b;
		elf->file.data[offset + 2] = c;
		elf->file.data[offset + 3] = d;
	}
	else
	{
		elf->file.data[offset] = d;
		elf->file.data[offset + 1] = c;
		elf->file.data[offset + 2] = b;
		elf->file.data[offset + 3] = a;
	}
}


BBP::std::byte BBP::std::ELF::ELFBuilder::ident(BBP::std::byte idx) { return readByte(this, ELF_HEADER_IDENTOFFSET + idx); }
void BBP::std::ELF::ELFBuilder::ident(BBP::std::byte idx, BBP::std::byte value) { writeByte(this, value, ELF_HEADER_IDENTOFFSET + idx); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::type(void) { return readHalfWord(this, ELF_HEADER_TYPEOFFSET); }
void BBP::std::ELF::ELFBuilder::type(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_TYPEOFFSET); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::machine(void) { return readHalfWord(this, ELF_HEADER_MACHINEOFFSET); }
void BBP::std::ELF::ELFBuilder::machine(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_MACHINEOFFSET); }

BBP::std::word BBP::std::ELF::ELFBuilder::version(void) { return readWord(this, ELF_HEADER_VERSIONOFFSET); }
void BBP::std::ELF::ELFBuilder::version(BBP::std::word value) { writeWord(this, value, ELF_HEADER_VERSIONOFFSET); }

BBP::std::address_t BBP::std::ELF::ELFBuilder::entry(void) { return readWord(this, ELF_HEADER_ENTRYOFFSET); }
void BBP::std::ELF::ELFBuilder::entry(BBP::std::address_t value) { writeWord(this, value, ELF_HEADER_ENTRYOFFSET); }

BBP::std::offset_t BBP::std::ELF::ELFBuilder::phoff(void) { return readWord(this, ELF_HEADER_PHOFFOFFSET); }
void BBP::std::ELF::ELFBuilder::phoff(BBP::std::offset_t value) { writeWord(this, value, ELF_HEADER_PHOFFOFFSET); }

BBP::std::offset_t BBP::std::ELF::ELFBuilder::shoff(void) { return readWord(this, ELF_HEADER_SHOFFOFFSET); }
void BBP::std::ELF::ELFBuilder::shoff(BBP::std::offset_t value) { writeWord(this, value, ELF_HEADER_SHOFFOFFSET); }

BBP::std::word BBP::std::ELF::ELFBuilder::flags(void) { return readWord(this, ELF_HEADER_FLAGSOFFSET); }
void BBP::std::ELF::ELFBuilder::flags(BBP::std::word value) { writeWord(this, value, ELF_HEADER_FLAGSOFFSET); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::ehsize(void) { return readHalfWord(this, ELF_HEADER_EHSIZEOFFSET); }
void BBP::std::ELF::ELFBuilder::ehsize(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_EHSIZEOFFSET); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::phentsize(void) { return readHalfWord(this, ELF_HEADER_PHENTSIZEOFFSET); }
void BBP::std::ELF::ELFBuilder::phentsize(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_PHENTSIZEOFFSET); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::phnum(void) { return readHalfWord(this, ELF_HEADER_PHNUMOFFSET); }
void BBP::std::ELF::ELFBuilder::phnum(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_PHNUMOFFSET); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::shentsize(void) { return readHalfWord(this, ELF_HEADER_SHENTSIZEOFFSET); }
void BBP::std::ELF::ELFBuilder::shentsize(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_SHENTSIZEOFFSET); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::shnum(void) { return readHalfWord(this, ELF_HEADER_SHNUMOFFSET); }
void BBP::std::ELF::ELFBuilder::shnum(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_SHNUMOFFSET); }

BBP::std::halfword BBP::std::ELF::ELFBuilder::shstrndx(void) { return readHalfWord(this, ELF_HEADER_SHSTRNDXOFFSET); }
void BBP::std::ELF::ELFBuilder::shstrndx(BBP::std::halfword value) { writeHalfWord(this, value, ELF_HEADER_SHSTRNDXOFFSET); }
