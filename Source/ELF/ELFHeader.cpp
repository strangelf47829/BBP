#include "../include/ELF.h"
#include "../include/StressRelief.h"
#include "../include/stdio.h"
#include "../include/Strings.h"


BBP::std::ELF::ELFBuilder::ELFBuilder(BBP::std::ResourceManager *res, std::size_t segCount, std::size_t secCount, std::size_t symCount)
	: allocator(res),
	file(PAGE<string_element>(52 + segCount * ELF_SEGMENT_HEADERSIZE + secCount * ELF_SECTION_HEADERSIZE, (std::string_element *)res->calloc(52 + segCount * ELF_SEGMENT_HEADERSIZE + secCount * ELF_SECTION_HEADERSIZE, sizeof(std::string_element)))),
	Endian(false),
	segments(Stack<SegmentBuilder>(&segPages, segCount)),
	sections(Stack<SectionBuilder>(&secPages, secCount)),
	symbols(Stack<SymbolBuilder>(&symPages, symCount)),
	symbolCount(symCount),
	segPages(PAGE<SegmentBuilder>(segCount, (SegmentBuilder *)res->calloc(segCount, sizeof(SegmentBuilder)))),
	secPages(PAGE<SectionBuilder>(secCount, (SectionBuilder *)res->calloc(secCount, sizeof(SectionBuilder)))),
	symPages(PAGE<SymbolBuilder>(symCount, (SymbolBuilder *)res->calloc(symCount, sizeof(SymbolBuilder)))),
	isMemUnique(true),
	hashBucketCount(4),
	hashChainCount(symCount)
{
	for (index_t index = 0; index < segCount; index++)
	{
		segments[index] = ELF::SegmentBuilder(this, 0);
		segments[index].elf = this;
	}
	for (index_t index = 0; index < secCount; index++)
	{
		sections[index] = ELF::SectionBuilder(this, 0);
		sections[index].elf = this;
	}
	for (index_t index = 0; index < symCount; index++)
	{
		symbols[index] = ELF::SymbolBuilder(this, 0);
		symbols[index].elf = this;
	}

	phnum(segCount);
	shnum(secCount);
}

BBP::std::ELF::ELFBuilder::ELFBuilder(BBP::std::PAGE<string_element> *_file, BBP::std::ResourceManager *res)
	: allocator(res),
	file(PAGE<string_element>(0, nullptr)),
	Endian(false),
	segments(Stack<SegmentBuilder>(&segPages, 0)),
	sections(Stack<SectionBuilder>(&secPages, 0)),
	symbols(Stack<SymbolBuilder>(&symPages, 0)),
	symbolCount(0),
	segPages(PAGE<SegmentBuilder>()),
	secPages(PAGE<SectionBuilder>()),
	symPages(PAGE<SymbolBuilder>()),
	isMemUnique(false),
	hashBucketCount(4),
	hashChainCount(0)
{

	// Check if file is not nullptr
	if (_file == nullptr)
		throw std::exception("Invalid file", ENOENT);

	// Check if resource allocator is nullptr
	if (res == nullptr)
		throw std::exception("Invalid resource allocator", ENOENT);

	// Check if file size is at least 52 bytes
	if (std::seqlen(*_file) < 52)
		throw std::exception("File is not ELF file: too small.", ENODATA);

	// Set file
	file = std::PAGE<std::string_element>(_file->dataSize, _file->data);

	// Get magic
	std::word magic = (ident(0) << 24) | (ident(1) << 16) | (ident(2) << 8) | ident(3);

	// Validate magic
	if (magic != 0x7f454c46)
		throw std::exception("File is not ELF file: wrong magic", ENODATA);

	// Get phnum and shnum count
	std::size_t sectionCount = shnum();
	std::size_t segmentCount = phnum();

	// Create resources
	secPages = std::PAGE<SectionBuilder>(sectionCount, (SectionBuilder *)res->calloc(sectionCount, sizeof(SectionBuilder)));
	segPages = std::PAGE<SegmentBuilder>(segmentCount, (SegmentBuilder *)res->calloc(segmentCount, sizeof(SegmentBuilder)));
	
	// And assign stacks
	sections = std::Stack<SectionBuilder>(&secPages, sectionCount);
	segments = std::Stack<SegmentBuilder>(&segPages, segmentCount);

	// Get offsets of program and section headers
	std::offset_t sectionOffsets = shoff();
	std::offset_t segmentOffsets = phoff();

	// Now also get size of program and section headers
	std::size_t sectionSize = shentsize();
	std::size_t segmentSize = phentsize();

	// Now initialize them
	for (index_t index = 0; index < segmentCount; index++)
	{
		segments[index] = ELF::SegmentBuilder(this, segmentOffsets + index * segmentSize);
		segments[index].elf = this;
	}
	for (index_t index = 0; index < sectionCount; index++)
	{
		sections[index] = ELF::SectionBuilder(this, sectionOffsets + index * sectionSize);
		sections[index].elf = this;
	}

	// Now, each section should now be initialized. Find the string tab.
	shstrtab = shstrndx();

	// If strings are invalid, return error
	if (shstrtab == 0)
		throw std::exception("No string table present", ENODATA);

	// If string table type is not string table, throw error
	if (_shstrtab().type() != std::ELF::SHT_STRTAB)
		throw std::exception("Invalid string table type", EINVAL);

	// Try to parse each string stuff
	for (std::index_t index = 0; index < sectionCount; index++)
	{
		// Get string
		std::c_string sectionName = file.data + sections[shstrtab].offset() + sections[index].name();

		// Hash string
		std::hash_t nameHash = std::strhsh(sectionName);

		// Switch based on name
		switch (nameHash)
		{
		case 0x12b9abd7: /*.text*/
			text = index;
			break;
		case 0x04079010: /*.rel.text*/
			rel_text = index;
			break;
		case 0x3a311d7d: /*.dynsym*/
			handlers = index;
			break;
		case 0x3a280aa7: /*.dynstr*/
			dynstr = index;
			break;
		case 0x11ac0ad4: /*.data*/
			data = index;
			break;
		case 0x2d1158c8: /*.rodata*/
			rodata = index;
			break;
		case 0x119dc248: /*.bss*/
			bss = index;
			break;
		case 0x0ea7e788: /*.data1*/
			data1 = index;
			break;
		case 0x119e283e: /*.got*/
			got = index;
			break;
		case 0x0700b1c6: /*.symtab*/
			symtab = index;
			break;
		case 0x0eb7addf: /*.shstrtab*/
			shstrtab = index;
			break;
		case 0x0702e4dc: /*.strtab*/
			strtab = index;
			break;
		case 0x0ed1cab6: /*.hashtab*/
			hashtab = index;
			break;
		}


	}

	// If symbols exist, check how many exist
	// Early return ignored since more conditions may be added later.
	if (symtab)
	{
		// Get count by dividing size by entsize
		symbolCount = _symtab().size() / _symtab().entsize();

		// Remove empty symbols
		for (std::index_t index = 0; index < symbolCount; index++)
		{
			// Create symbol builder
			SymbolBuilder sym(this, _symtab().offset() + _symtab().entsize() * index);

			// Get name
			std::c_string symName = file.data + sections[_symtab().link()].offset() + sym.name();

			// Symbol is wasted if index is 0 and name is 0.
			if (sym.info() == 0 && sym.name() == 0)
				symbolCount--;
		}

		// Create page and stack
		symPages = std::PAGE<SymbolBuilder>(symbolCount, (SymbolBuilder *)res->calloc(symbolCount, sizeof(SymbolBuilder)));
		symbols = std::Stack<SymbolBuilder>(&symPages, symbolCount);

		// Assign stuff now
		for (std::index_t index = 0; index < symbolCount; index++)
		{
			// Constructor
			symbols[index] = SymbolBuilder(this, _symtab().offset() + _symtab().entsize() * index);

			// Save data if it has any.
			if (!(symbols[index].info() == 0 && symbols[index].name() == 0) && index != 0)
				symbols[index].data = std::PAGE<std::string_element>(symbols[index].size(), file.data + sections[symbols[index].shndx()].offset() + symbols[index].value());

		}

		// and that's basically it.
	}
}


// Dispose of everything
void BBP::std::ELF::ELFBuilder::close(void)
{
	segments.atElement = segments.max_elements;
	sections.atElement = sections.max_elements;
	symbols.atElement = symbols.max_elements;

	segments.foreach([](SegmentBuilder &segment) { segment.close(); });
	sections.foreach([](SectionBuilder &section) { section.close(); });
	symbols.foreach([](SymbolBuilder &symbol) { symbol.close(); });

	// Loop over until no more
	std::PAGE<std::ELF::SegmentBuilder> *segmentptr = segments.page;
	std::PAGE<std::ELF::SectionBuilder> *sectionptr = sections.page;
	std::PAGE<std::ELF::SymbolBuilder> *symbolptr = symbols.page;

	do
	{
		std::PAGE<std::ELF::SegmentBuilder> *tmp = segmentptr->nextPage;
		allocator->free(segmentptr->data);
		segmentptr = tmp;
	} while (segmentptr);

	do
	{
		std::PAGE<std::ELF::SectionBuilder> *tmp = sectionptr->nextPage;
		allocator->free(sectionptr->data);
		sectionptr = tmp;
	} while (sectionptr);

	do
	{
		std::PAGE<std::ELF::SymbolBuilder> *tmp = symbolptr->nextPage;
		allocator->free(symbolptr->data);
		symbolptr = tmp;
	} while (symbolptr);


	//allocator->free(segments.page->data);
	//allocator->free(sections.page->data);
	//allocator->free(symbols.page->data);

	if (isMemUnique)
		allocator->free(file.data);

}

BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_shstrtab()
{
	return sections[shstrtab];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_strtab()
{
	return sections[strtab];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_symtab()
{
	return sections[symtab];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_bss()
{
	return sections[bss];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_data()
{
	return sections[data];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_data1()
{
	return sections[data1];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_dynstr()
{
	return sections[dynstr];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_handlers()
{
	return sections[handlers];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_rodata()
{
	return sections[rodata];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_got()
{
	return sections[got];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_text()
{
	return sections[text];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_rel_text()
{
	return sections[rel_text];
}
BBP::std::ELF::SectionBuilder &BBP::std::ELF::ELFBuilder::_hashtab()
{
	return sections[hashtab];
}

void BBP::std::ELF::ELFBuilder::formStandard(std::size_t textSize, std::size_t dataSize, std::size_t rodataSize, std::size_t bssSize)
{
	// Emit headers so we can actually modify them
	emitHeader();

	// Define the different sections indicies

	handlers = 1;
	text = 2;
	got = 3;
	dynstr = 4;
	data = 5;
	bss = 6;
	data1 = 7;
	rodata = 8;
	rel_text = 9;
	symtab = 10;
	shstrtab = 11;
	strtab = 12;
	hashtab = 13;

	// Now set types
	sections[text].type(SHT_PROGBITS);
	sections[rel_text].type(SHT_REL);
	sections[handlers].type(SHT_PROGBITS);
	sections[dynstr].type(SHT_STRTAB);
	sections[data].type(SHT_PROGBITS);
	sections[rodata].type(SHT_PROGBITS);
	sections[bss].type(SHT_NOBITS);
	sections[data1].type(SHT_PROGBITS);
	sections[got].type(SHT_PROGBITS);
	sections[symtab].type(SHT_SYMTAB);
	sections[strtab].type(SHT_STRTAB);
	sections[shstrtab].type(SHT_STRTAB);
	sections[hashtab].type(SHT_HASH);

	// Set ent sizes
	sections[symtab].entsize(ELF_SYMBOL_ENTRYSIZE);
	sections[rel_text].entsize(8);

	// Set names
	sections[shstrtab].Append(128);
	sections[shstrtab].stack++;
	sections[text].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".text";
	sections[rel_text].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".rel.text";
	sections[handlers].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".handlers";
	sections[dynstr].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".dynstr";
	sections[data].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".data";
	sections[rodata].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".rodata";
	sections[bss].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".bss";
	sections[data1].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".data1";
	sections[got].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".got";
	sections[symtab].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".symtab";
	sections[strtab].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".strtab";
	sections[shstrtab].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".shstrtab";
	sections[hashtab].name(sections[shstrtab].stack.atElement);
	sections[shstrtab].stack <<= ".hashtab";

	// Update header
	shstrndx(shstrtab);

	// Update link and info fields
	sections[rel_text].link(symtab);
	sections[rel_text].info(text);
	sections[symtab].link(strtab);
	sections[symtab].info(symbolCount);
	sections[hashtab].link(symtab);
	IHate::Myself();

	// Add symbols
	sections[symtab].Append(symbolCount * ELF_SYMBOL_ENTRYSIZE);
	sections[handlers].Append(255);
	sections[dynstr].Append(1024);
	sections[strtab].Append(1024);
	
	// Set symbol entry flags
	for (index_t i = 0; i < symbolCount; i++)
	{
		symbols[i].header.bytes = ELF_SYMBOL_ENTRYSIZE;
		symbols[i].header.data = sections[symtab].data.data + ELF_SYMBOL_ENTRYSIZE * i;
	}

	// Now Append data sizes and so
	sections[text].Append(textSize);
	sections[data].Append(dataSize);
	sections[rodata].Append(rodataSize);
	sections[bss].Append(bssSize);

	// Now set hashtab ready. Size has to be multiplied by sizeof(word) / sizeof(byte)
	sections[hashtab].Append((hashBucketCount + hashChainCount + 2) * (sizeof(std::word) / sizeof(std::byte)));
	sections[hashtab].stack << hashBucketCount;
	sections[hashtab].stack << hashChainCount;

	// We want .text sections to be executable,
	sections[handlers].flags(std::ELF::Executable);
	sections[text].flags(std::ELF::Executable);
	sections[got].flags(std::ELF::Executable);

	// We also want .data and .bss to be allocated and writeable
	sections[bss].flags(std::ELF::Allocator | std::ELF::Writeable);
	sections[data].flags(std::ELF::Allocator | std::ELF::Writeable);
	sections[data1].flags(std::ELF::Allocator | std::ELF::Writeable);

	// We want rodata to be allocateable, but not writeable
	sections[rodata].flags(std::ELF::Allocator);

	// Set segment indicies
	textSegment = 0;
	handlerSegment = 1;
	gotSegment = 2;
	dataSegment = 3;
	bssSegment = 4;
	rodataSegment = 5;

	// Set segment types
	segments[textSegment].type(std::ELF::PT_LOAD);
	segments[handlerSegment].type(std::ELF::PT_LOAD);
	segments[gotSegment].type(std::ELF::PT_LOAD);
	segments[dataSegment].type(std::ELF::PT_LOAD);
	segments[bssSegment].type(std::ELF::PT_LOAD);
	segments[rodataSegment].type(std::ELF::PT_LOAD);
}

void BBP::std::ELF::ELFBuilder::renameSymbol(std::index_t symb, std::conststring name)
{
	if (symb >= symbolCount)
		throw std::exception("Symbol out of range.", ERANGE);
	sections[strtab].stack++;
	symbols[symb].name(sections[strtab].stack.atElement);
	sections[strtab].stack <<= name;	
}