#include "ELFUtils.h"
#include "Lexer.h"

#ifndef LUA_PROC_H
#define LUA_PROC_H

const char* replace_Symbol(BBP::Services::Lexer::lex_context* context, unsigned int index, unsigned int kw);

namespace BBP
{
	namespace Services
	{
		namespace Lexer
		{


			struct LuaProcessor
			{
				lex_context *context;
				BBP::Services::Binaries::ELF::ELFBuilder *elf;

				bool doLinking = true;
				bool strip;

				bool emitCallsToSTDLib;

				BBP::IO::Buffers::stringTable *extVarTable = nullptr;
				BBP::IO::Buffers::stringTable *intVarTable = nullptr;
				BBP::IO::Buffers::stringTable *varTypeTable = nullptr;
				BBP::IO::Buffers::stringTable *funcSigTables = nullptr;
				BBP::IO::Buffers::stringTable *funcBodyTables = nullptr;
				BBP::IO::Buffers::stringTable *globalsTable = nullptr;
				BBP::IO::Buffers::stringTable *fileTable = nullptr;

				bool addedKeywords = false;

				char *activeFile = nullptr;

				char *interp;
			};

			void P_Link(LuaProcessor *, char *_ext, char *_int, char *type);
			void P_fetchKeywords(LuaProcessor *);

			void P_doFile(LuaProcessor *, const char *at, bool link);
			void P_destroyLua(LuaProcessor *proc);

		}
	}
}


#endif
