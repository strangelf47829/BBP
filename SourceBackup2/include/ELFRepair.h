#include "ELFUtils.h"

#ifndef BBP_ELFREPAIR_H
#define BBP_ELFREPAIR_H

#define ELFREPAIR_MAXATTEMPTS 10

namespace BBP
{
	namespace Services
	{
		namespace Binaries
		{
			
			namespace ELF
			{
				struct ELFDamageReport
				{
					WORD leadingNullStrings;
					bool DisorganizedSymbolNames;
				};


				ELFDamageReport fixAllDamage(ELFBuilder *, unsigned char *);
				ELFDamageReport checkDamage(ELFBuilder *);

				bool checkForLeadingNullStrings(ELFBuilder *, ELFDamageReport *);
				bool fixLeadingNullStrings(sectionBuilder *, BBP::IO::Buffers::stringTable *, ELFDamageReport *);
				bool fixLeadingNullStrings(ELFBuilder *, sectionBuilder *, BBP::IO::Buffers::stringTable *, ELFDamageReport *);

				bool checkForDisorganizedSymbolNames(ELFBuilder *, ELFDamageReport *);
				bool fixDisorganizedSymbolNames(unsigned char *, ELFBuilder *, WORD, ELFDamageReport *);
			}
		}
	}
}

#endif