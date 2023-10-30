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

					BBP::Services::Binaries::ELF::stringTable *inputFiles;

					bool emitExecutable = false;
					bool verbose = false;

				};

				void setOutputFile(CompilationContext *, BBP::Services::Binaries::ELF::stringTable *argTable, unsigned int arg);
				void setOutputFile(CompilationContext*, const char *);

				void setInterp(CompilationContext *, BBP::Services::Binaries::ELF::stringTable *argTable, unsigned int arg);

				void showHelp();
				void showVersion();

				int Run(Binaries::ELF::stringTable *args);

				void DoCompiling(CompilationContext *context, Binaries::ELF::stringTable *args);
				void DoLinking(CompilationContext *context, Binaries::ELF::stringTable *args);

				int ParseArguments(CompilationContext *, Binaries::ELF::stringTable *args, unsigned int argc);

				void EmitExecutable(CompilationContext *);

				CompilationContext *createCompiler();
				void destroyCompiler(CompilationContext *);
			}
		}
	}
}


#endif