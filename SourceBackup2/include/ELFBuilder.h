#include "Kernel.h"

#ifndef BBP_ELFBUILDER_H
#define BBP_ELFBUILDER_H

namespace BBP
{
	namespace Services
	{
		namespace Binaries
		{

			namespace ELF
			{

				typedef struct {
					ProgramHeader header;
					unsigned char *data;

					char *name;
					unsigned int nameLength;
					bool autoDealloc;
				} segmentBuilder;


				typedef struct {
					SectionHeader header;
					unsigned char *data;

					char *name;
					unsigned int nameLength;
					bool autoDealloc;
				} sectionBuilder;


				typedef struct {
					ElfHeader header;

					segmentBuilder **segments;
					sectionBuilder **sections;
					SymbolEntry **symbols;
					WORD symbolCount;

					bool is64bit;

					BBP::IO::Buffers::stringTable shstrndx;
					BBP::IO::Buffers::stringTable *genericStrings;

					int emittedAuxLibs;

				} ELFBuilder;

				ELFBuilder *createELF(HALFWORD, HALFWORD);
				void destroyELF(ELFBuilder *);

				void setELFSegmentCount(ELFBuilder *, HALFWORD);
				void setELFSectionCount(ELFBuilder *, HALFWORD);
				void setELFSymbolCount(ELFBuilder *, WORD);

				void addELFSymbol(ELFBuilder *, SymbolEntry *, char *);
				void addELFSegment(ELFBuilder *, segmentBuilder *);
				void addELFSection(ELFBuilder *, sectionBuilder *);


				segmentBuilder *createSegment(const char *name, unsigned char *data, WORD size, WORD mem_size, WORD flags, bool);
				sectionBuilder *createSection(const char *name, unsigned char *data, WORD type, WORD size, bool);

			}
		}
	}
}

#endif
