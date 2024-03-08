#include "../include/ELF.h"

BBP::std::byte readByte(BBP::std::ELF::SymbolBuilder *sym, BBP::std::offset_t offset)
{
	return sym->header.data[offset];
}
void writeByte(BBP::std::ELF::SymbolBuilder *sym, BBP::std::byte byte, BBP::std::offset_t offset)
{
	sym->header.data[offset] = byte;
}
BBP::std::halfword readHalfWord(BBP::std::ELF::SymbolBuilder *sym, BBP::std::offset_t offset)
{
	BBP::std::byte a = sym->header.data[offset];
	BBP::std::byte b = sym->header.data[offset + 1];

	if (!sym->elf->Endian)
		return a | (b << 8);
	return b | (a << 8);
}
void writeHalfWord(BBP::std::ELF::SymbolBuilder *sym, BBP::std::halfword halfword, BBP::std::offset_t offset)
{
	BBP::std::byte a = (halfword >> 0) & 0xFF;
	BBP::std::byte b = (halfword >> 8) & 0xFF;

	if (!sym->elf->Endian)
	{
		sym->header.data[offset] = a;
		sym->header.data[offset + 1] = b;
	}
	else
	{
		sym->header.data[offset] = b;
		sym->header.data[offset + 1] = a;
	}
}
BBP::std::word readWord(BBP::std::ELF::SymbolBuilder *sym, BBP::std::offset_t offset)
{
	
	BBP::std::byte a = BBP::std::read(&sym->header, offset + 0);
	BBP::std::byte b = BBP::std::read(&sym->header, offset + 1);
	BBP::std::byte c = BBP::std::read(&sym->header, offset + 2);
	BBP::std::byte d = BBP::std::read(&sym->header, offset + 3);

	if (!sym->elf->Endian)
		return a | (b << 8) | (c << 16) | (d << 24);
	return d | (c << 8) | (b << 16) | (a << 24);
}
void writeWord(BBP::std::ELF::SymbolBuilder *sym, BBP::std::word word, BBP::std::offset_t offset)
{
	BBP::std::byte a = (word >> 0) & 0xFF;
	BBP::std::byte b = (word >> 8) & 0xFF;
	BBP::std::byte c = (word >> 16) & 0xFF;
	BBP::std::byte d = (word >> 24) & 0xFF;

	if (!sym->elf->Endian)
	{
		sym->header.data[offset] = a;
		sym->header.data[offset + 1] = b;
		sym->header.data[offset + 2] = c;
		sym->header.data[offset + 3] = d;
	}
	else
	{
		sym->header.data[offset] = d;
		sym->header.data[offset + 1] = c;
		sym->header.data[offset + 2] = b;
		sym->header.data[offset + 3] = a;
	}
}

BBP::std::ELF::SymbolBuilder::SymbolBuilder(ELFBuilder *elf, std::offset_t offset)
	: header(PAGE<string_element>(ELF_SYMBOL_ENTRYSIZE, elf->file.data + offset)),
	stack(&data, 0),
	data(PAGE<string_element>()),
	elf(elf),
	isMemUnique(false) {}

BBP::std::word BBP::std::ELF::SymbolBuilder::name() { return readWord(this, ELF_SYMBOL_NAMEOFFSET); }
void BBP::std::ELF::SymbolBuilder::name(BBP::std::word value) { writeWord(this, value, ELF_SYMBOL_NAMEOFFSET); }

BBP::std::word BBP::std::ELF::SymbolBuilder::value() { return readWord(this, ELF_SYMBOL_VALUEOFFSET); }
void BBP::std::ELF::SymbolBuilder::value(BBP::std::word value) { writeWord(this, value, ELF_SYMBOL_VALUEOFFSET); }

BBP::std::word BBP::std::ELF::SymbolBuilder::size() { return readWord(this, ELF_SYMBOL_SIZEOFFSET); }
void BBP::std::ELF::SymbolBuilder::size(BBP::std::word value) { writeWord(this, value, ELF_SYMBOL_SIZEOFFSET); }

BBP::std::byte BBP::std::ELF::SymbolBuilder::info() { return readByte(this, ELF_SYMBOL_INFOOFFSET); }
void BBP::std::ELF::SymbolBuilder::info(BBP::std::byte value) { writeByte(this, value, ELF_SYMBOL_INFOOFFSET); }

BBP::std::byte BBP::std::ELF::SymbolBuilder::other() { return readByte(this, ELF_SYMBOL_OTHEROFFSET); }
void BBP::std::ELF::SymbolBuilder::other(BBP::std::byte value) { writeByte(this, value, ELF_SYMBOL_OTHEROFFSET); }

BBP::std::halfword BBP::std::ELF::SymbolBuilder::shndx() { return readHalfWord(this, ELF_SYMBOL_SHNDXOFFSET); }
void BBP::std::ELF::SymbolBuilder::shndx(BBP::std::halfword value) { writeHalfWord(this, value, ELF_SYMBOL_SHNDXOFFSET); }