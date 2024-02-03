#include "Kernel.h"
#include "ELFUtils.h"
#include "LuaElf.h"
#include "LuaLexer.h"

#define BBP_LPP_VERSION "1.0.0"

#ifndef BBP_LPP_H
#define BBP_LPP_H

namespace BBP
{
	namespace Services
	{
		namespace Binaries
		{
			namespace LPP
			{

				struct CompilationContext
				{
					char *outFile;
					unsigned int outFileLength;

					char *interp;
					unsigned int interpLength;

					BBP::Services::Lexer::LuaProcessor *proc;

					BBP::IO::Buffers::stringTable *inputFiles;

					bool emitExecutable = false;
					bool verbose = false;

				};

				void setOutputFile(CompilationContext *, BBP::IO::Buffers::stringTable *argTable, unsigned int arg);
				void setOutputFile(CompilationContext*, const char *);

				void setInterp(CompilationContext *, BBP::IO::Buffers::stringTable *argTable, unsigned int arg);

				void showHelp();
				void showVersion();

				int Run(BBP::IO::Buffers::stringTable *args);

				void DoCompiling(CompilationContext *context, BBP::IO::Buffers::stringTable *args);
				void DoLinking(CompilationContext *context, BBP::IO::Buffers::stringTable *args);

				int ParseArguments(CompilationContext *, BBP::IO::Buffers::stringTable *args, unsigned int argc);

				void EmitExecutable(CompilationContext *);

				CompilationContext *createCompiler();
				void destroyCompiler(CompilationContext *);

				void exportName(BBP::Services::Lexer::LuaProcessor *);

				char *mangleName(bool *, char *);
				char *mangleName(bool *, const char *);
			}
		}
	}
}


#endif