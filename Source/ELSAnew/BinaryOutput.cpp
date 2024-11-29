#include "../include/ELSA/BinaryApplication.h"
#include "../include/FileSys.h"
#include "../include/ELSA/ELFRelocation.h"
#include "../include/ELSA/ELFSegment.h"
#include "../include/ELSA/ELFSymbol.h"

BBP::elsa::BinaryApplication::BinaryApplication()
	: elf(&sectionPointers)
{
	// Make big endian
	elf.header.ident.littleEndian = std::isLittleEndianEnvironment();

	// Link each section together
	elf.sectionTable.Link(elf.segmentTable).QLink(handlers)
		.Link(got)
		.Link(text)
		.Link(rodata)
		.Link(data)
		.Link(bss)
		.Link(reltext)
		.Link(relgot)
		.Link(symtab)
		.Link(strtab);

	// Set x86 header
	elf.header.e_machine = elf.header.EM_386;

	// Define sections
	defineSections();

	// Define null symbol
	emitEmptySymbol();
}

void BBP::elsa::BinaryApplication::emitEmptySymbol()
{
	// Create symbol
	ELF::Symbol symbol;

	// Allocate bytes for symbol
	symtab.Allocate(symbol.data, 16);

	// Set to 0
	symbol.Nullify();

	// Then write to data
	symbol.writeData(elf.header.ident.littleEndian);
}

	// Create page for symdata
	/*std::PAGE<std::byte> symdata;

	std::string symName = "hi_max!";
	std::size_t symNameLength = std::strlen(symName);

	// Then allocate four bytes from strtab
	strtab.Allocate(symdata, 2 + symNameLength);

	// Set 0 to 0
	symdata[0] = 0;

	// And terminator
	symdata[symNameLength + 1] = 0;

	// Copy string
	for (std::index_t idx = 0; idx < symNameLength; idx++)
		symdata[idx + 1] = symName[idx];

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

	reloc.writeData(elf.header.ident.littleEndian);*/

// Reset everything upon destruction
BBP::elsa::BinaryApplication::~BinaryApplication()
{
	Reset();
}

// Reset everything
void BBP::elsa::BinaryApplication::Reset()
{
	// Reset ELF sections
	elf.Reset();

	// Reset Own sections
	handlers.Reset();
	text.Reset();
	got.Reset();
	data.Reset();
	bss.Reset();
	rodata.Reset();
	reltext.Reset();
	relgot.Reset();
	symtab.Reset();
	strtab.Reset();
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
void BBP::elsa::BinaryApplication::defineSegments(std::offset_t interpreterOffset)
{
	// Create a segment
	BBP::ELF::Segment segment;

	// Create a section
	BBP::ELF::Section section;

	// Get 0th byte of segmentTable
	segment.data = std::PAGE<std::byte>(elf.header.e_phentsize, &elf.segmentTable[0]);
	segment.nullify();

	// Get index of string tab (to find interpreter string)
	std::index_t strtabIndex = elf.find(".strtab");

	// Then get section data
	section.data = std::PAGE<std::byte>(elf.header.e_shentsize, &elf.sectionTable[elf.header.e_shentsize * strtabIndex]);
	section.readData(elf.header.ident.littleEndian);
	
	// This function expects the interpreter segment to be the first segment of the binary file
	// These lines set the actual interpreter string
	segment.type = segment.PT_INTERP;

	// Set segment data to the section offset (interpreter string is expected to be the first string, right after the first '0')
	segment.offset = section.offset + interpreterOffset;
	segment.paddr = section.offset + interpreterOffset;
	segment.vaddr = section.offset + interpreterOffset;

	// Then get length of interpreter (first string in strtab)
	// Find section
	BBP::elsa::Section *strtabSection = elf.find(strtabIndex);

	// Check if nullptr
	if (strtabSection == nullptr)
	{
		// If nullptr, set everything to 0
		segment.filesz = 0;
		segment.memsz =  0;

		segment.align = 0;
		segment.flags = 0;
	}

	// Otherwise, get string size
	else
	{
		// Get string size
		std::size_t interpLength = 0;
		while((*strtabSection)[interpreterOffset + interpLength++]);

		// Then set sizes
		segment.filesz = interpLength;
		segment.memsz = interpLength;

		// Align to 1 byte, then set flags to Readable and allocatable
		segment.align = 1;
		segment.flags = 4;
	}

	// Now write data into segment header
	segment.writeData(elf.header.ident.littleEndian);
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

// Get section
BBP::elsa::Section *BBP::elsa::BinaryApplication::operator[] (std::string str)
{
	// Otherwise, return sections data
	return elf.get(str);
}

// Set up a symbol db page
void BBP::elsa::BinaryApplication::setup(symbol_db &db)
{
	// Reset the db
	db.Reset();

	// Then set pages
	db.setDataSection(&data);
	db.setRelocationSection(&reltext);
	db.setStringSection(&strtab);
	db.setSymbolSection(&symtab);
}

// Set up a symbol db page
void BBP::elsa::BinaryApplication::setDataSection(symbol_db &db, std::string &str)
{
	// Set
	db.setDataSection(elf.get(str));
}

// Check if little endian
bool BBP::elsa::BinaryApplication::isLittleEndian()
{
	return elf.header.ident.littleEndian;
}

// Emit a file
void BBP::elsa::BinaryApplication::emitFile(std::conststring path)
{
	// Then pack segments
	elf.packSegments(4);

	// Pack segments
	std::size_t fileSize = elf.packSections();

	// Then define segments
	defineSegments(1);

	// Then save, 
	elf.saveFile(path, fileSize);
}
