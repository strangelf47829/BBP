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

				BBP::Services::Binaries::ELF::stringTable *extVarTable = nullptr;
				BBP::Services::Binaries::ELF::stringTable *intVarTable = nullptr;
				BBP::Services::Binaries::ELF::stringTable *varTypeTable = nullptr;
				BBP::Services::Binaries::ELF::stringTable *funcSigTables = nullptr;
				BBP::Services::Binaries::ELF::stringTable *funcBodyTables = nullptr;
				BBP::Services::Binaries::ELF::stringTable *globalsTable = nullptr;
				BBP::Services::Binaries::ELF::stringTable *fileTable = nullptr;

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
