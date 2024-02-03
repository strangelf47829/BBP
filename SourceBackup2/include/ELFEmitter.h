#include "Kernel.h"
#include "ELFBuilder.h"

#ifndef BBP_ELFEMITTER_H
#define BBP_ELFEMITTER_H

namespace BBP
{
	namespace Services
	{
		namespace Binaries
		{

			namespace ELF
			{

				WORD calculateFileSize(ELFBuilder *);
				WORD calculateHeaderSize(ELFBuilder *);
				OFFSET calculateProgramHeaderOffsets(ELFBuilder *);
				WORD calculateStringTableLength(BBP::IO::Buffers::stringTable *);

				void emitELFAuxHeaders(ELFBuilder *);

				unsigned char *emitHeader(ELFBuilder *);
				unsigned char *emitProgramHeader(ELFBuilder *elf, segmentBuilder *);
				unsigned char *emitSectionHeader(ELFBuilder *elf, sectionBuilder *);
				unsigned char *emitStringTable(BBP::IO::Buffers::stringTable *);
				unsigned char *emitStringTableWithNull(BBP::IO::Buffers::stringTable *);
				unsigned char *emitSymbol(SymbolEntry *);
				void normalizeELF(ELFBuilder *elf);
				unsigned char *emitELF(ELFBuilder *);

				void saveELFFile(ELFBuilder *, const char *);
			}
		}
	}
}

#endif