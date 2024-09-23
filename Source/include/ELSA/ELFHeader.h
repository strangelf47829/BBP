#ifndef BBP_ELF_HEADER_H
#define BBP_ELF_HEADER_H

#include "../Memory.h"

namespace BBP
{
	namespace ELF
	{

		// Structure for ELF idents
		class ELFIdent
		{

			// Where to store data?
			std::PAGE<std::byte> data;

		public:

			// Is 32 bit?
			bool thirtyTwoBit;

			// Is little endian?
			bool littleEndian;

			// OSAbi?
			enum ELFABI : std::byte
			{
				NONE,
				SYSV,
				HPUX,
				NETBSD,
				LINUX,
				SOLARIS,
				IRIX,
				FREEBSD,
				TRU64,
				ARM,
				STANDALONE
			} ABI;

			// The ABI version
			std::byte ABIVersion;

			// Read/Write this data
			bool readData();
			void writeData();

			// Constructor
			ELFIdent(std::PAGE<std::byte> *);
		};

		// Header class
		class ELFHeader
		{

			// The data of the header
			std::STATIC_PAGE<std::byte, 52> headerData;

		public:

			// Ident header
			ELFIdent ident;

			// Constructor
			ELFHeader();

			// Verify Ident
			bool verifyMagic();

			// Read/Write data
			void readHeaderFields();
			void writeHeaderFields();

			// Type of file
			enum ELFHeaderType : std::halfword
			{
				ET_NONE,
				ET_REL,
				ET_EXEC,
				ET_DYN,
				ET_CORE
			};

			// Type of machine
			enum ELFHeaderMachine : std::halfword
			{
				EM_NONE,
				EM_M32,
				EM_SPARC,
				EM_386,
				EM_68K,
				EM_88K,
				EM_860 = 7,
				EM_MIPS,
				EM_PARISC = 15,
				EM_VPP500 = 17,
				EM_SPARC32PLUS,
				EM_960,
				EM_PPC,
				EM_PPC64,
				EM_S390,
				EM_ARM = 0x28,
				EM_IA_64 = 0x32,
				EM_X86_64 = 0x3E,
				EM_VAX = 0x4B
			};

			// 'Special' fields
			ELFHeaderType e_type;
			ELFHeaderMachine e_machine;

			// Rest of the fields
			std::word e_version;
			std::address_t e_entry;
			std::offset_t e_phoff;
			std::offset_t e_shoff;
			std::word e_flags;
			std::halfword e_ehsize;
			std::halfword e_phentsize;
			std::halfword e_phnum;
			std::halfword e_shentsize;
			std::halfword e_shnum;
			std::halfword e_shstrndx;

			// Get raw data
			std::byte &operator[] (std::index_t);

		};

	}
}

#endif