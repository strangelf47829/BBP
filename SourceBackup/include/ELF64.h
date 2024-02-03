#ifndef CLELF_H
#define CLELF_H

#include <stdint.h>

#define HALFWORD 	uint16_t
#define WORD		uint32_t
#define ADDRESS		uint32_t
#define OFFSET		uint32_t
#define DWORD		uint64_t

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
	unsigned char	e_ident[16]; 	///< [7f 45 4c 46] [ 00=invalid 01=32bit 02=64bit ] [ 00=invalid 01=LSB first 02=MSB first ] [ 01 00 00 00 00 00 00 00 00 00]
	HALFWORD		e_type; 		///< See E_TYPE enum
	HALFWORD		e_machine; 		///< See E_MACHINE enum
	WORD			e_version; 		///< Always 1
	DWORD			e_entry; 		///< Entry point address for executables, Constructor address for shared libraries, otherwise 0.
	DWORD			e_phoff; 		///< Program Header Offset (Relative to File start)
	DWORD			e_shoff; 		///< Section Header Offset (Relative to File start)
	WORD			e_flags; 		///< Flags
	HALFWORD		e_ehsize; 		///< Elf header size
	HALFWORD		e_phentsize; 		///< Program header entry size
	HALFWORD		e_phnum; 		///< Program header entry count
	HALFWORD		e_shentsize; 		///< Section header entry size
	HALFWORD		e_shnum; 		///< Section header entry count
	HALFWORD		e_shstrndx; 		///< Section header string table index
} ElfHeader;

typedef struct {
	WORD			type=0; 			///< The type of this program header. See P_TYPE
	WORD 			flags = 0; 			///< Flags. PF_X - Executable, PF_W - Writeable, PF_R - Readable
	OFFSET			offset=0; 		///< Specifies where the contents of this header is in the file (relative to File start)
	ADDRESS			vaddr=0; 			///< Where in virtual memory this segment should be loaded
	ADDRESS			paddr=0; 			///< Where in physical memory this segment should be loaded
	WORD 			filesz=0; 		///< Size of the segment in the file. If 0, the segment is defined exclusively by the header
	WORD 			memsz=0; 			///< Size of the segment in memory. If greater than the size in file, the extra bytes will be initialized as 0
	WORD			align=0; 			///< 0x1000
} ProgramHeader;

typedef struct {
	WORD			sh_name = 0; 		///< Name of section (as offset in member e_shstrndx of ELFHEADER)
	WORD			sh_type = 0; 		///< Type of this section
	DWORD			sh_flags = 0; 		///< Flags. 0x1 - Writeable, 0x2 - part of the sections that are loaded into memory, 0x4 - executable.
	DWORD			sh_addr = 0; 		///< ?
	DWORD 			sh_offset = 0; 		///< The offset relative to the beginning of the file where this segment is.
	DWORD 			sh_size = 0;  		///< How large this section is in the file
	WORD 			sh_link = 0; 		///< Allows for linking? (If sh_type is 'SHT_SYMTAB', this is where you would store the index of the string table pointing to the strings)
	WORD			sh_info = 0; 		///< Info?
	DWORD 			sh_addralign = 0; 		///< ?
	DWORD 			sh_entsize = 0; 		///< Size of each entry?
} SectionHeader;

typedef struct {
	WORD			st_name = 0; 		///< Name of the symbol (relative to string table (string table is a section!)
	ADDRESS			st_value = 0; 		///< Value of symbol (So for example where 'main' starts
	DWORD			st_size = 0; 		///< The size of whatever this symbol is (for example the size of 'main')
	unsigned char 		st_info = 0;		///< 0x.0 - Local, 0x.1 - Global, 0x.2 - Weak, 0x0. - no type, 0x1. - object, 0x2. - function
	unsigned char		st_other = 0;
	HALFWORD		st_shndx = 0; 		///< the section to which the symbol is relative
} SymbolEntry;

typedef struct {
	ADDRESS			r_offset;
	WORD			r_info;
} RelEntry;

typedef struct {
	DWORD			r_offset;
	DWORD			r_info;
	DWORD		r_addend;
} RelaEntry;

#endif
