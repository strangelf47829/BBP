#include "Kernel.h"
#include "ELFUtils.h"

#ifndef BBP_ELFREADER_H
#define BBP_ELFREADER_H

namespace BBP
{
	namespace Services
	{
		namespace Binaries
		{

			namespace ELF
			{

				enum ELFVerification : unsigned int
				{
					VALID = 0,
					INVALID_MAGIC = 1,
					INVALID_MACHINE = 2,
					INVALID_VERSION = 4
				};

				uint64_t readEightBytesMSB(unsigned char *file, uint32_t offset);
				uint64_t readEightBytesLSB(unsigned char *file, uint32_t offset);
				uint32_t readFourBytesMSB(unsigned char *file, uint32_t offset);
				uint32_t readFourBytesLSB(unsigned char *file, uint32_t offset);
				uint16_t readTwoBytesMSB(unsigned char *file, uint32_t offset);
				uint16_t readTwoBytesLSB(unsigned char *file, uint32_t offset);

				ELFBuilder *readELF(unsigned char *file, uint32_t fileSize);
				ELFBuilder *readELF(unsigned char **, const char *at);

				ELFVerification verifyELF(ELFBuilder *);
				bool isELF(mem_t *data);

				int readELFHeader(unsigned char *file, ELFBuilder *);
				int readELFProgramHeader(unsigned char *file, ELFBuilder *, uint32_t offset, uint32_t index);
				int readELFSectionHeader(unsigned char *file, ELFBuilder *, uint32_t offset, uint32_t index);
				int readELFSymbols(unsigned char *file, ELFBuilder *, uint32_t symtableindex);
				int readELFStringTable(unsigned char *file, uint32_t offset, uint32_t length, BBP::IO::Buffers::stringTable *);
			}
		}
	}
}

#endif
