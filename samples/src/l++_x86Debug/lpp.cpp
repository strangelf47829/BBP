
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
	BBP::Services::Binaries::ELF::stringTable* argTable = BBP::Services::Binaries::ELF::createStringTable();

	// Copy over arguments into string table
	for (unsigned int arg = 1; arg < argc; arg++)
		BBP::Services::Binaries::ELF::addStringToTable(argTable, argv[arg]);

	// Run compiler
	int code = BBP::Services::Binaries::LPP::Run(argTable);

	// Destroy stuff
	BBP::Services::Binaries::ELF::setStringTableSize(argTable, 0);
	delete(argTable);

	return code;
}