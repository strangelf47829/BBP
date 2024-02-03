#include "ELFUtils.h"

#ifndef BBP_ELFLOADER_H
#define BBP_ELFLOADER_H

#define R_386_PC32 2


namespace BBP
{
	namespace Services
	{
		namespace Binaries
		{
			namespace ELF
			{
				namespace Runtime
				{

					struct Executable
					{
						// The ELF in question
						ELF::ELFBuilder *elf;

						unsigned char *data;
						unsigned int fileSize;

						WORD relocationTable;
						WORD symbolNameTable;
						WORD textSection;

						unsigned char *text = nullptr;

						void **funcs = nullptr;
						unsigned int funcCount = 0;
						BBP::IO::Buffers::stringTable *loadedFunctions = nullptr;
					};

					Executable *E_LoadExecutable(const char *at);
					Executable *E_LoadExecutable(unsigned char *, ELFBuilder *elf);

					void E_LoadSectionInfo(Executable *);

					void E_doRelocation(Executable *, RelEntry *);
					void E_doRelocation(Executable *, RelaEntry *);

					unsigned int E_getSymbolIndex(Executable *, const char *);
					unsigned int E_getFunctionIndex(Executable *, const char *);

					void E_readFunctions(Executable *);
					void *E_getFunction(Executable *, const char *);

					char *E_getSymbolName(Executable *, unsigned int);

					void E_DestroyExecutable(Executable *);


				}
			}
		}
	}
}


#endif
