#include "../include/ELF.h"

BBP::std::word readWord(BBP::std::ELF::SegmentBuilder *seg, BBP::std::offset_t offset)
{
	BBP::std::byte a = seg->header.data[offset];
	BBP::std::byte b = seg->header.data[offset + 1];
	BBP::std::byte c = seg->header.data[offset + 2];
	BBP::std::byte d = seg->header.data[offset + 3];

	if (!seg->elf->Endian)
		return a | (b << 8) | (c << 16) | (d << 24);
	return d | (c << 8) | (b << 16) | (a << 24);
}
void writeWord(BBP::std::ELF::SegmentBuilder *seg, BBP::std::word word, BBP::std::offset_t offset)
{
	BBP::std::byte a = (word >> 0) & 0xFF;
	BBP::std::byte b = (word >> 8) & 0xFF;
	BBP::std::byte c = (word >> 16) & 0xFF;
	BBP::std::byte d = (word >> 24) & 0xFF;

	if (!seg->elf->Endian)
	{
		seg->header.data[offset] = a;
		seg->header.data[offset + 1] = b;
		seg->header.data[offset + 2] = c;
		seg->header.data[offset + 3] = d;
	}
	else
	{
		seg->header.data[offset] = d;
		seg->header.data[offset + 1] = c;
		seg->header.data[offset + 2] = b;
		seg->header.data[offset + 3] = a;
	}
}

BBP::std::ELF::SegmentBuilder::SegmentBuilder(ELFBuilder *elf, std::offset_t offset)
	: header(PAGE<string_element>(ELF_SEGMENT_HEADERSIZE, elf->file.data + offset)),
	stack(&data, 0),
	data(PAGE<string_element>()),
	elf(elf),
	isMemUnique(false) {}

BBP::std::word BBP::std::ELF::SegmentBuilder::type() { return readWord(this, ELF_SEGMENT_TYPEOFFSET); }
void BBP::std::ELF::SegmentBuilder::type(BBP::std::word value) { writeWord(this, value, ELF_SEGMENT_TYPEOFFSET); }

BBP::std::word BBP::std::ELF::SegmentBuilder::flags() { return readWord(this, ELF_SEGMENT_FLAGSOFFSET); }
void BBP::std::ELF::SegmentBuilder::flags(BBP::std::word value) { writeWord(this, value, ELF_SEGMENT_FLAGSOFFSET); }

BBP::std::offset_t BBP::std::ELF::SegmentBuilder::offset() { return readWord(this, ELF_SEGMENT_OFFSETOFFSET); }
void BBP::std::ELF::SegmentBuilder::offset(BBP::std::offset_t value) { writeWord(this, value, ELF_SEGMENT_OFFSETOFFSET); }

BBP::std::address_t BBP::std::ELF::SegmentBuilder::vaddr() { return readWord(this, ELF_SEGMENT_VADDROFFSET); }
void BBP::std::ELF::SegmentBuilder::vaddr(BBP::std::address_t value) { writeWord(this, value, ELF_SEGMENT_VADDROFFSET); }

BBP::std::address_t BBP::std::ELF::SegmentBuilder::paddr() { return readWord(this, ELF_SEGMENT_PADDROFFSET); }
void BBP::std::ELF::SegmentBuilder::paddr(BBP::std::address_t value) { writeWord(this, value, ELF_SEGMENT_PADDROFFSET); }

BBP::std::word BBP::std::ELF::SegmentBuilder::filesz() { return readWord(this, ELF_SEGMENT_FILESZOFFSET); }
void BBP::std::ELF::SegmentBuilder::filesz(BBP::std::word value) { writeWord(this, value, ELF_SEGMENT_FILESZOFFSET); }

BBP::std::word BBP::std::ELF::SegmentBuilder::memsz() { return readWord(this, ELF_SEGMENT_MEMSZOFFSET); }
void BBP::std::ELF::SegmentBuilder::memsz(BBP::std::word value) { writeWord(this, value, ELF_SEGMENT_MEMSZOFFSET); }

BBP::std::word BBP::std::ELF::SegmentBuilder::align() { return readWord(this, ELF_SEGMENT_ALIGNOFFSET); }
void BBP::std::ELF::SegmentBuilder::align(BBP::std::word value) { writeWord(this, value, ELF_SEGMENT_ALIGNOFFSET); }