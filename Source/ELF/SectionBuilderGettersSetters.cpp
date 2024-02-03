#include "../include/ELF.h"

BBP::std::word readWord(BBP::std::ELF::SectionBuilder *sec, BBP::std::offset_t offset)
{
	BBP::std::byte a = sec->header.data[offset];
	BBP::std::byte b = sec->header.data[offset + 1];
	BBP::std::byte c = sec->header.data[offset + 2];
	BBP::std::byte d = sec->header.data[offset + 3];

	if (!sec->elf->Endian)
		return a | (b << 8) | (c << 16) | (d << 24);
	return d | (c << 8) | (b << 16) | (a << 24);
}
void writeWord(BBP::std::ELF::SectionBuilder *sec, BBP::std::word word, BBP::std::offset_t offset)
{
	BBP::std::byte a = (word >> 0) & 0xFF;
	BBP::std::byte b = (word >> 8) & 0xFF;
	BBP::std::byte c = (word >> 16) & 0xFF;
	BBP::std::byte d = (word >> 24) & 0xFF;

	if (!sec->elf->Endian)
	{
		sec->header.data[offset] = a;
		sec->header.data[offset + 1] = b;
		sec->header.data[offset + 2] = c;
		sec->header.data[offset + 3] = d;
	}
	else
	{
		sec->header.data[offset] = d;
		sec->header.data[offset + 1] = c;
		sec->header.data[offset + 2] = b;
		sec->header.data[offset + 3] = a;
	}
}

BBP::std::ELF::SectionBuilder::SectionBuilder(ELFBuilder *elf, std::offset_t offset)
	: header(PAGE<string_element>(ELF_SEGMENT_HEADERSIZE, elf->file.data + offset)),
	stack(&data, 0),
	data(PAGE<string_element>()),
	elf(elf),
	isMemUnique(false) {}

BBP::std::word BBP::std::ELF::SectionBuilder::name() { return readWord(this, ELF_SECTION_NAMEOFFSET); }
void BBP::std::ELF::SectionBuilder::name(BBP::std::word value) { writeWord(this, value, ELF_SECTION_NAMEOFFSET); }

BBP::std::word BBP::std::ELF::SectionBuilder::type() { return readWord(this, ELF_SECTION_TYPEOFFSET); }
void BBP::std::ELF::SectionBuilder::type(BBP::std::word value) { writeWord(this, value, ELF_SECTION_TYPEOFFSET); }

BBP::std::word BBP::std::ELF::SectionBuilder::flags() { return readWord(this, ELF_SECTION_FLAGSOFFSET); }
void BBP::std::ELF::SectionBuilder::flags(BBP::std::word value) { writeWord(this, value, ELF_SECTION_FLAGSOFFSET); }

BBP::std::address_t BBP::std::ELF::SectionBuilder::addr() { return readWord(this, ELF_SECTION_ADDROFFSET); }
void BBP::std::ELF::SectionBuilder::addr(BBP::std::address_t value) { writeWord(this, value, ELF_SECTION_ADDROFFSET); }

BBP::std::offset_t BBP::std::ELF::SectionBuilder::offset() { return readWord(this, ELF_SECTION_OFFSETOFFSET); }
void BBP::std::ELF::SectionBuilder::offset(BBP::std::offset_t value) { writeWord(this, value, ELF_SECTION_OFFSETOFFSET); }

BBP::std::word BBP::std::ELF::SectionBuilder::size() { return readWord(this, ELF_SECTION_SIZEOFFSET); }
void BBP::std::ELF::SectionBuilder::size(BBP::std::word value) { writeWord(this, value, ELF_SECTION_SIZEOFFSET); }

BBP::std::word BBP::std::ELF::SectionBuilder::link() { return readWord(this, ELF_SECTION_LINKOFFSET); }
void BBP::std::ELF::SectionBuilder::link(BBP::std::word value) { writeWord(this, value, ELF_SECTION_LINKOFFSET); }

BBP::std::word BBP::std::ELF::SectionBuilder::info() { return readWord(this, ELF_SECTION_INFOOFFSET); }
void BBP::std::ELF::SectionBuilder::info(BBP::std::word value) { writeWord(this, value, ELF_SECTION_INFOOFFSET); }

BBP::std::word BBP::std::ELF::SectionBuilder::addralign() { return readWord(this, ELF_SECTION_ADDRALIGNOFFSET); }
void BBP::std::ELF::SectionBuilder::addralign(BBP::std::word value) { writeWord(this, value, ELF_SECTION_ADDRALIGNOFFSET); }

BBP::std::word BBP::std::ELF::SectionBuilder::entsize() { return readWord(this, ELF_SECTION_ENTSIZEOFFSET); }
void BBP::std::ELF::SectionBuilder::entsize(BBP::std::word value) { writeWord(this, value, ELF_SECTION_ENTSIZEOFFSET); }