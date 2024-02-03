
#include "Kernel.h"
#undef printf
#include "ELFUtils.h"
#include "Lexer.h"
#include "LuaLexer.h"
#include "LuaELF.h"
#include "Lpp.h"

#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv)
{
	// If there are no arguments, throw error
	if (argc == 1)
	{
		printf("L++: No input files\n");
		return -1;
	}

	// Aux table to store the arg data in.
	BBP::IO::Buffers::stringTable* argTable = BBP::IO::Buffers::createStringTable();

	// Copy over arguments into string table
	for (unsigned int arg = 1; arg < argc; arg++)
		BBP::IO::Buffers::addStringToTable(argTable, argv[arg]);

	// Run compiler
	int code = BBP::Services::Binaries::LPP::Run(argTable);

	// Destroy stuff
	BBP::IO::Buffers::setStringTableSize(argTable, 0);
	delete(argTable);

	return code;
}