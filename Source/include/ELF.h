#ifndef BBP_LIB_ELF
#define BBP_LIB_ELF

#include "Memory.h"
#include "Strings.h"
#include "Resources.h"

#include "ELF32.h"

namespace BBP
{
	namespace std
	{
		namespace ELF
		{
			class ELFBuilder;
			class SegmentBuilder;
			class SectionBuilder;
			class SymbolBuilder;


			class ELFBuilder
			{
			public:

				ELFBuilder(ResourceManager *res, std::size_t segCount, std::size_t secCount, std::size_t symCount);
				ELFBuilder(PAGE<string_element> *_file, ResourceManager *res);

				// The resource manager in use by the ELF
				ResourceManager *allocator;

				// The file in question
				PAGE<string_element> file;

				// Keep track of endiann-ness
				bool Endian;

				// Segment stack (pointers to save memory)
				Stack<SegmentBuilder> segments;

				// Section stack (pointers to save memory)
				Stack<SectionBuilder> sections;

				// Symbol stack (pointers to save memory)
				Stack<SymbolBuilder> symbols;

				std::byte ident(std::byte);
				void ident(std::byte, std::byte);

				std::halfword type(void);
				void type(std::halfword);

				std::halfword machine(void);
				void machine(std::halfword);

				std::word version(void);
				void version(std::word);

				std::address_t entry(void);
				void entry(std::address_t);

				std::offset_t phoff(void);
				void phoff(std::offset_t);

				std::offset_t shoff(void);
				void shoff(std::offset_t);

				std::word flags(void);
				void flags(std::word);

				std::halfword ehsize(void);
				void ehsize(std::halfword);

				std::halfword phentsize(void);
				void phentsize(std::halfword);

				std::halfword phnum(void);
				void phnum(std::halfword);

				std::halfword shentsize(void);
				void shentsize(std::halfword);

				std::halfword shnum(void);
				void shnum(std::halfword);

				std::halfword shstrndx(void);
				void shstrndx(std::halfword);

				// Functions to move around various components
				void emitHeader();
				std::size_t calculateHeaderSize();
				void emitELFHeaders();

				// Variables to keep track of various standard sections
				std::index_t shstrtab;
				std::index_t strtab;
				std::index_t symtab;
				std::index_t hashtab;

				std::index_t bss;
				std::index_t data;
				std::index_t data1;
				std::index_t handlers;
				std::index_t dynstr;
				std::index_t rodata;
				std::index_t got;
				std::index_t text;
				std::index_t rel_text;

				// Variables to keep track of various standard segments
				std::index_t textSegment;
				std::index_t handlerSegment;
				std::index_t gotSegment;
				std::index_t dataSegment;
				std::index_t bssSegment;
				std::index_t rodataSegment;

				SectionBuilder &_shstrtab();
				SectionBuilder &_strtab();
				SectionBuilder &_symtab();
				SectionBuilder &_hashtab();

				SectionBuilder &_bss();
				SectionBuilder &_data();
				SectionBuilder &_data1();
				SectionBuilder &_handlers();
				SectionBuilder &_dynstr();
				SectionBuilder &_rodata();
				SectionBuilder &_got();
				SectionBuilder &_text();
				SectionBuilder &_rel_text();

				// Recalculates the various offsets used
				void recalculateOffsets(std::offset_t after, std::size_t byHowMuch);

				// Creates a standard application
				void formStandard(std::size_t textSize, std::size_t dataSize, std::size_t rodataSize, std::size_t bssSize);

				// Renames a symbol
				void renameSymbol(std::index_t, conststring);

				void close();

				std::size_t symbolCount;

				// Hashing stuff
				std::word hashBucketCount;
				std::word hashChainCount;

			private:
				PAGE<SegmentBuilder> segPages;
				PAGE<SectionBuilder> secPages;
				PAGE<SymbolBuilder> symPages;

				bool isMemUnique;

			};


			class SymbolBuilder
			{
			public:

				std::word name(void);
				void name(std::word);

				std::address_t value(void);
				void value(std::address_t);

				std::word size(void);
				void size(std::word);

				std::byte info(void);
				void info(std::byte);

				std::byte other(void);
				void other(std::byte);

				std::halfword shndx(void);
				void shndx(std::halfword);

				PAGE<string_element> header; // This is the actual 'header' of the segment. Stored as binary representation as would be on file.
				Stack<string_element> stack; // This is a stack representation of the data
				PAGE<string_element> data; // The data this segment contains. 

				void close();

				// The owner of this segment
				ELFBuilder *elf;

				bool isMemUnique;

				SymbolBuilder(ELFBuilder *, std::offset_t); // Segment is already found within elf
				SymbolBuilder(ELFBuilder *, string_element *, std::size_t); // Segment is not within elf

				/*
				* If true, 'data' and 'header' are alloc'ed externally and is not contained within 'elf'.
				* If false, 'data' and 'header' point to an internal section of 'elf'.
				*
				* This is used to read and write data to and from the elf.
				*/


			};


			class SectionBuilder
			{
			public:

				std::word name(void);
				void name(std::word);

				std::word type(void);
				void type(std::word);

				std::word flags(void);
				void flags(std::word);

				std::address_t addr(void);
				void addr(std::address_t);

				std::offset_t offset(void);
				void offset(std::offset_t);

				std::word size(void);
				void size(std::word);

				std::word link(void);
				void link(std::word);

				std::word info(void);
				void info(std::word);

				std::word addralign(void);
				void addralign(std::word);

				std::word entsize(void);
				void entsize(std::word);

				PAGE<string_element> header; // This is the actual 'header' of the segment. Stored as binary representation as would be on file.
				Stack<string_element> stack; // This is a stack representation of the data
				PAGE<string_element> data; // The data this segment contains. 

				void Append(std::size_t);
				void Extend(std::size_t);
				void close();

				// The owner of this segment
				ELFBuilder *elf;
				bool isMemUnique;

				SectionBuilder(ELFBuilder *, std::offset_t); // Segment is already found within elf
				SectionBuilder(ELFBuilder *, string_element *, std::size_t); // Segment is not within elf

				/*
				* If true, 'data' and 'header' are alloc'ed externally and is not contained within 'elf'.
				* If false, 'data' and 'header' point to an internal section of 'elf'.
				*
				* This is used to read and write data to and from the elf.
				*/

			};


			class SegmentBuilder
			{
			public:
				
				void Append(std::size_t dataSize);

				std::word type(void);
				void type(std::word);

				std::offset_t offset(void);
				void offset(std::offset_t);

				std::address_t vaddr(void);
				void vaddr(std::address_t);

				std::address_t paddr(void);
				void paddr(std::address_t);

				std::word filesz(void);
				void filesz(std::word);

				std::word memsz(void);
				void memsz(std::word);

				std::word flags(void);
				void flags(std::word);

				std::word align(void);
				void align(std::word);

				PAGE<string_element> header; // This is the actual 'header' of the segment. Stored as binary representation as would be on file.
				Stack<string_element> stack; // This is a stack representation of the data
				PAGE<string_element> data; // The data this segment contains. 

				void close();

				// The owner of this segment
				ELFBuilder *elf;

				bool isMemUnique;

				SegmentBuilder(ELFBuilder *, std::offset_t); // Segment is already found within elf
				SegmentBuilder(ELFBuilder *, string_element *, std::size_t); // Segment is not within elf

				/*
				* If true, 'data' and 'header' are alloc'ed externally and is not contained within 'elf'.
				* If false, 'data' and 'header' point to an internal section of 'elf'.
				* 
				* This is used to read and write data to and from the elf.
				*/

			};

		}
	}
}

#endif
