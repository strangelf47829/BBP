#ifndef BBP_STDLIB_ELF64_H
#define BBP_STDLIB_ELF64_H

#error "64 Bit ELF header not supported anymore"

#define ELF_HEADER_IDENTOFFSET 0
#define ELF_HEADER_IDENTSIZE 16
#define ELF_HEADER_TYPEOFFSET 16
#define ELF_HEADER_TYPESIZE 2
#define ELF_HEADER_MACHINEOFFSET 18
#define ELF_HEADER_MACHINESIZE 2
#define ELF_HEADER_VERSIONOFFSET 20
#define ELF_HEADER_VERSIONSIZE 4
#define ELF_HEADER_ENTRYOFFSET 24
#define ELF_HEADER_ENTRYSIZE 8
#define ELF_HEADER_PHOFFOFFSET 32
#define ELF_HEADER_PHOFFSIZE 8
#define ELF_HEADER_SHOFFOFFSET 40
#define ELF_HEADER_SHOFFSIZE 8
#define ELF_HEADER_FLAGSOFFSET 48
#define ELF_HEADER_FLAGSSIZE 4
#define ELF_HEADER_EHSIZEOFFSET 52
#define ELF_HEADER_EHSIZESIZE 2
#define ELF_HEADER_PHENTSIZEOFFSET 54
#define ELF_HEADER_PHENTSIZESIZE 2
#define ELF_HEADER_PHNUMOFFSET 56
#define ELF_HEADER_PHNUMSIZE 2
#define ELF_HEADER_SHENTSIZEOFFSET 58
#define ELF_HEADER_SHENTSIZESIZE 2
#define ELF_HEADER_SHNUMOFFSET 60
#define ELF_HEADER_SHNUMSIZE 2
#define ELF_HEADER_SHSTRNDXOFFSET 62
#define ELF_HEADER_SHSTRNDXSIZE 2

#define ELF_SEGMENT_HEADERSIZE 32
#define ELF_SEGMENT_TYPEOFFSET 0
#define ELF_SEGMENT_TYPESIZE 4
#define ELF_SEGMENT_FLAGSOFFSET 4
#define ELF_SEGMENT_FLAGSIZE 4
#define ELF_SEGMENT_OFFSETOFFSET 8
#define ELF_SEGMENT_OFFSETSIZE 4
#define ELF_SEGMENT_VADDROFFSET 12
#define ELF_SEGMENT_VADDRSIZE 4
#define ELF_SEGMENT_PADDROFFSET 16
#define ELF_SEGMENT_PADDRSIZE 4
#define ELF_SEGMENT_FILESZOFFSET 20
#define ELF_SEGMENT_FILESZSIZE 4
#define ELF_SEGMENT_MEMSZOFFSET 24
#define ELF_SEGMENT_MEMSZSIZE 4
#define ELF_SEGMENT_ALIGNOFFSET 28
#define ELF_SEGMENT_ALIGNSIZE 4 

#define ELF_SECTION_HEADERSIZE 64
#define ELF_SECTION_NAMEOFFSET 0
#define ELF_SECTION_NAMESIZE 4
#define ELF_SECTION_TYPEOFFSET 4
#define ELF_SECTION_TYPESIZE 4
#define ELF_SECTION_FLAGSOFFSET 8
#define ELF_SECTION_FLAGSSIZE 8
#define ELF_SECTION_ADDROFFSET 16
#define ELF_SECTION_ADDRSIZE 8
#define ELF_SECTION_OFFSETOFFSET 24
#define ELF_SECTION_OFFSETSIZE 8
#define ELF_SECTION_SIZEOFFSET 32
#define ELF_SECTION_SIZESIZE 8
#define ELF_SECTION_LINKOFFSET 40
#define ELF_SECTION_LINKSIZE 4
#define ELF_SECTION_INFOOFFSET 44
#define ELF_SECTION_INFOSIZE 4
#define ELF_SECTION_ADDRALIGNOFFSET 48
#define ELF_SECTION_ADDRALIGNSIZE 8
#define ELF_SECTION_ENTSIZEOFFSET 56
#define ELF_SECTION_ENTSIZESIZE 8

#define ELF_SYMBOL_ENTRYSIZE 20
#define ELF_SYMBOL_NAMEOFFSET 0
#define ELF_SYMBOL_NAMESIZE 4
#define ELF_SYMBOL_VALUEOFFSET 4
#define ELF_SYMBOL_VALUESIZE 4
#define ELF_SYMBOL_SIZEOFFSET 8
#define ELF_SYMBOL_SIZESIZE 8
#define ELF_SYMBOL_INFOOFFSET 16
#define ELF_SYMBOL_INFOSIZE 1
#define ELF_SYMBOL_OTHEROFFSET 17
#define ELF_SYMBOL_OTHERSIZE 1
#define ELF_SYMBOL_SHNDXOFFSET 18
#define ELF_SYMBOL_SHNDXSIZE 2

#include "stddef.h"

namespace BBP
{
	namespace std
	{
		namespace ELF
		{
			typedef enum
			{
				ET_NONE = 0, 	///< No File Type
				ET_REL = 1, 	///< Relocatable file type
				ET_EXEC = 2, 	///< Executable file type
				ET_DYN = 3, 	///< Shared Object File
				ET_CORE = 4 	///< Core file type
			} E_TYPE;

			typedef enum
			{
				X86 = 0x03,
				MIPS = 0x08,
				ARM = 0x28,
				AMD64 = 0x3E,
				ARMV8 = 0xB7,
				RISCV = 0xF3
			} E_MACHINE;

			typedef enum
			{
				PT_NILL = 0, 	///< Place holder. (Disabled)
				PT_LOAD = 1, 	///< Load segment type. This segment will be loaded into memory. Can also be used to create 0 initialized segments by 'specifying a larger size in memory than is loaded from the file'
				PT_DYNAMIC = 2, ///< Contains information required for dynamically linking binaries. Is interpreted by the OS.
				PT_INTERP = 3, 	///< Specifies the program interpreter required for this dynamic executable.
				PT_NOTE = 4, 	///< Aux data for binary.
				PT_SHLIB = 5,   ///< Undefined. Do not use.
				PT_PHDR = 6, 	///< Program header table data.
				PT_TLS = 7	///< Thread local storage.
			} P_TYPE;

			typedef enum {
				SHT_NILL = 0, 		///< Inactive section
				SHT_PROGBITS = 1, 	///< Contains data for the program.
				SHT_SYMTAB = 2, 	///< Symbol table. List of symbol tables.
				SHT_STRTAB = 3, 	///< String table. This is what e_shstrndx points to.
				SHT_RELA = 4, 		///< Relocation A. Contains information on how to modify sections.
				SHT_HASH = 5, 		///< Hash table
				SHT_DYNAMIC = 6, 	///< Information required for dynamic linking
				SHT_NOTE = 7, 		///< Contains auxiliary info
				SHT_NOBITS = 8, 	///< Section that takes no space in the file
				SHT_REL = 9, 		///< Relocation. Contains information on how to modify sections
				SHT_SHLIB = 10, 	///< Reserved, but unused.
				SHT_DYNSYM = 11 	///< Symbol table. Used to define external symbols and so.
			} SH_TYPE;


			typedef struct {
				std::byte		e_ident[16]; 	///< [7f 45 4c 46] [ 00=invalid 01=32bit 02=64bit ] [ 00=invalid 01=LSB first 02=MSB first ] [ 01 00 00 00 00 00 00 00 00 00]
				std::halfword	e_type; 		///< See E_TYPE enum
				std::halfword	e_machine; 		///< See E_MACHINE enum
				std::word		e_version; 		///< Always 1
				std::dword		e_entry; 		///< Entry point address for executables, Constructor address for shared libraries, otherwise 0.
				std::dword		e_phoff; 		///< Program Header Offset (Relative to File start)
				std::dword		e_shoff; 		///< Section Header Offset (Relative to File start)
				std::word		e_flags; 		///< Flags
				std::halfword	e_ehsize; 		///< Elf header size
				std::halfword	e_phentsize; 	///< Program header entry size
				std::halfword	e_phnum; 		///< Program header entry count
				std::halfword	e_shentsize; 	///< Section header entry size
				std::halfword	e_shnum; 		///< Section header entry count
				std::halfword	e_shstrndx; 	///< Section header string table index
			} ElfHeader;

			typedef struct {
				std::word		type = 0; 		///< The type of this program header. See P_TYPE
				std::word		flags = 0; 		///< Flags. PF_X - Executable, PF_W - Writeable, PF_R - Readable
				std::offset_t	offset = 0; 	///< Specifies where the contents of this header is in the file (relative to File start)
				std::address_t	vaddr = 0; 		///< Where in virtual memory this segment should be loaded
				std::address_t	paddr = 0; 		///< Where in physical memory this segment should be loaded
				std::word		filesz = 0; 	///< Size of the segment in the file. If 0, the segment is defined exclusively by the header
				std::word		memsz = 0; 		///< Size of the segment in memory. If greater than the size in file, the extra bytes will be initialized as 0
				std::word		align = 0; 		///< 0x1000
			} ProgramHeader;

			typedef struct {
				std::word		sh_name = 0; 		///< Name of section (as offset in member e_shstrndx of ELFHEADER)
				std::word		sh_type = 0; 		///< Type of this section
				std::dword		sh_flags = 0; 		///< Flags. 0x1 - Writeable, 0x2 - part of the sections that are loaded into memory, 0x4 - executable.
				std::dword		sh_addr = 0; 		///< ?
				std::dword		sh_offset = 0; 		///< The offset relative to the beginning of the file where this segment is.
				std::dword		sh_size = 0;  		///< How large this section is in the file
				std::word		sh_link = 0; 		///< Allows for linking? (If sh_type is 'SHT_SYMTAB', this is where you would store the index of the string table pointing to the strings)
				std::word		sh_info = 0; 		///< Info?
				std::dword		sh_addralign = 0; 		///< ?
				std::dword		sh_entsize = 0; 		///< Size of each entry?
			} SectionHeader;

			typedef struct {
				std::word		st_name = 0; 		///< Name of the symbol (relative to string table (string table is a section!)
				std::address_t	st_value = 0; 		///< Value of symbol (So for example where 'main' starts
				std::dword		st_size = 0; 		///< The size of whatever this symbol is (for example the size of 'main')
				std::byte 		st_info = 0;		///< 0x.0 - Local, 0x.1 - Global, 0x.2 - Weak, 0x0. - no type, 0x1. - object, 0x2. - function
				std::byte		st_other = 0;
				std::halfword	st_shndx = 0; 		///< the section to which the symbol is relative
			} SymbolEntry;

			typedef struct {
				std::address_t	r_offset;
				std::word		r_info;
			} RelEntry;

			typedef struct {
				std::dword		r_offset;
				std::dword		r_info;
				std::dword		r_addend;
			} RelaEntry;
		}
	}
}

#endif