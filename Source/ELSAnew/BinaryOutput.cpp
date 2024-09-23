#include "../include/ELSA/BinaryApplication.h"
#include "../include/FileSys.h"
#include "../include/ELSA/ELFRelocation.h"
#include "../include/ELSA/ELFSegment.h"
#include "../include/ELSA/ELFSymbol.h"

BBP::elsa::BinaryApplication::BinaryApplication()
	: elf(&sectionPointers)
{
	// Link each section together
	elf.sectionTable.Link(elf.segmentTable).Link(handlers)
		.Link(got)
		.Link(text)
		.Link(rodata)
		.Link(data)
		.Link(bss)
		.Link(reltext)
		.Link(relgot)
		.Link(symtab)
		.Link(strtab);

	// Define sections
	defineSections();

	// Create page for symdata
	std::PAGE<std::byte> symdata;

	// Then allocate four bytes from strtab
	strtab.Allocate(symdata, 4);

	// Then, 0, h, i, 0
	symdata[0] = 0;
	symdata[1] = 'h';
	symdata[2] = 'i';
	symdata[3] = 0; 

	// Create symbol
	ELF::Symbol symb;

	// Allocate 32 bytes from symtab
	symtab.Allocate(symb.data, 16);
	symb.Nullify();
	symb.writeData(elf.header.ident.littleEndian);

	symtab.Allocate(symb.data, 16);
	symb.Nullify();

	symb.name = 1;
	symb.shndx = elf.find(".data") + 1;

	symb.writeData(elf.header.ident.littleEndian);

	// Create new relocation
	ELF::Relocation reloc;

	// Then allocate 8 bytes from rel.text 
	reltext.Allocate(reloc.data, 8);

	// Now write
	reloc.Nullify();
	reloc.offset = 0;
	reloc.type = 1;
	reloc.value = 1;

	reloc.writeData(elf.header.ident.littleEndian);

	// Then allocate 8 bytes from rel.got 
	relgot.Allocate(reloc.data, 8);

	// Now write
	reloc.Nullify();
	reloc.offset = 0;
	reloc.type = 0;
	reloc.value = 1;

	reloc.writeData(elf.header.ident.littleEndian);

	// Then allocate 8 bytes from strtab 
	std::PAGE<std::byte> dat;
	strtab.Allocate(dat, 32);

	std::string str = "Hello-max,-it-works-:3";
	std::strcpy((std::c_string)dat.data, str.data);

	elf.header.e_machine = elf.header.EM_386;

}

// Define each section as defined by ELF standards
void BBP::elsa::BinaryApplication::defineSections()
{
	// Create thingy to edit information
	std::STATIC_PAGE<std::byte, 40> sectionInfoPage;
	ELF::Section sectionInfo;
	sectionInfo.data = sectionInfoPage;

	// Register .handlers
	elf.registerSection(handlers, sectionInfo, ".handlers");

	// .handlers needs to be aligned to 4 bytes, and flagged as 'allocate'
	sectionInfo.addralign = 4;
	sectionInfo.type = ELF::Section::SHT_PROGBITS;
	sectionInfo.flags = ELF::Section::SHF_ALLOC;

	// Then allocate 256 bytes to .handlers
	{
		std::PAGE<std::byte> data;
		handlers.Allocate(data, 256);
	}

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// Nullify
	sectionInfo.nullify();

	// Now register .got
	std::index_t gotIdx = elf.registerSection(got, sectionInfo, ".got") + 1;

	// .got also needs alignment to 4 bytes, and with appropriate flags set
	sectionInfo.addralign = 4;
	sectionInfo.type = ELF::Section::SHT_PROGBITS;
	sectionInfo.flags = ELF::Section::SHF_ALLOC;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// Then nullify
	sectionInfo.nullify();

	// Now register .text
	std::index_t textIdx = elf.registerSection(text, sectionInfo, ".text") + 1;

	// .text also needs alignment to 4 bytes, and with appropriate flags set
	sectionInfo.addralign = 4;
	sectionInfo.type = ELF::Section::SHT_PROGBITS;
	sectionInfo.flags = ELF::Section::SHF_ALLOC | ELF::Section::SHF_EXECINSTR;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// Nullify
	sectionInfo.nullify();

	// Now register .rodata
	elf.registerSection(rodata, sectionInfo, ".rodata");

	// Set flags
	sectionInfo.type = ELF::Section::SHT_PROGBITS;
	sectionInfo.flags = ELF::Section::SHF_ALLOC;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// Nullify
	sectionInfo.nullify();

	// Now register .data
	elf.registerSection(data, sectionInfo, ".data");

	// Set flags
	sectionInfo.type = ELF::Section::SHT_PROGBITS;
	sectionInfo.flags = ELF::Section::SHF_ALLOC | ELF::Section::SHF_WRITE;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// Nullify
	sectionInfo.nullify();

	// Now register .bss
	elf.registerSection(bss, sectionInfo, ".bss");

	// Set flags
	sectionInfo.type = ELF::Section::SHT_NOBITS;
	sectionInfo.flags = ELF::Section::SHF_ALLOC | ELF::Section::SHF_WRITE;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);


	// Nullify
	sectionInfo.nullify();

	// Now register .strtab
	std::index_t strtabIdx = elf.registerSection(strtab, sectionInfo, ".strtab") + 1;

	// Set flags
	sectionInfo.type = ELF::Section::SHT_STRTAB;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// Nullify
	sectionInfo.nullify();

	// Now register .symtab
	std::index_t symtabIdx = elf.registerSection(symtab, sectionInfo, ".symtab") + 1;

	// Set flags
	sectionInfo.type = ELF::Section::SHT_SYMTAB;
	sectionInfo.entsize = 16;
	sectionInfo.link = strtabIdx;
	sectionInfo.info = symtabIdx;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// Nullify
	sectionInfo.nullify();

	// Now register .rel.text
	elf.registerSection(reltext, sectionInfo, ".rel.text");

	// Set flags
	sectionInfo.entsize = 8;
	sectionInfo.type = ELF::Section::SHT_REL;
	sectionInfo.link = symtabIdx;
	sectionInfo.info = textIdx;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);

	// And finally register .rel.got
	elf.registerSection(relgot, sectionInfo, ".rel.got");

	// Set flags
	sectionInfo.entsize = 8;
	sectionInfo.type = ELF::Section::SHT_REL;
	sectionInfo.link = symtabIdx;
	sectionInfo.info = gotIdx;

	// Then save
	sectionInfo.writeData(elf.header.ident.littleEndian);
}

// Create some segments
void BBP::elsa::BinaryApplication::defineSegments()
{
	// Create a segment
	BBP::ELF::Segment segment;

	// Allocate 3 tables
	elf.segmentTable.Allocate(segment.data, elf.header.e_phentsize);
	segment.nullify();

	segment.type = segment.PT_INTERP;
	segment.offset = 0x344;
	segment.paddr = 0x344;
	segment.vaddr = 0x344;
	segment.filesz = 0x10;
	segment.memsz = 0x10;

	segment.writeData(elf.header.ident.littleEndian);

	elf.header.e_phnum = 1;
}

// Get byte
BBP::std::byte &BBP::elsa::BinaryApplication::operator[] (std::index_t idx)
{
	// If index is within header, show that
	if (idx < 52)
		return elf.header[idx];

	// Otherwise, return sections data
	return elf.sectionTable[idx - 52];
}

// Emit a file
void BBP::elsa::BinaryApplication::emitFile(std::conststring path)
{
	// Then define segments
	defineSegments();

	elf.saveFile(path);
}