#include "LuaLexer.h"
#include "ELFUtils.h"
#include "Lexer.h"

#ifndef BBP_LUAELF_H
#define BBP_LUAELF_H


namespace BBP
{
	namespace Services
	{
		namespace Lexer
		{
			LuaProcessor *P_Create(bool, bool, HALFWORD a, HALFWORD b);
			void P_Readfile(LuaProcessor *, const char *at);

			void P_EmitELFFunctions(LuaProcessor *);
			void P_EmitELFLinkInfo(LuaProcessor *);
			void P_deleteTables(LuaProcessor *);

			void P_BuildELF(LuaProcessor *);
			void P_EmitELF(LuaProcessor *, const char *at);


		}
	}
}

#endif