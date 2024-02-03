
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
		printf("No input file\n");
		return -1;
	}

	// Aux table to store the arg data in.
	BBP::IO::Buffers::stringTable* argTable = BBP::IO::Buffers::createStringTable();

	// Copy over arguments into string table
	for (unsigned int arg = 1; arg < argc; arg++)
		BBP::IO::Buffers::addStringToTable(argTable, argv[arg]);

	// Load ELF
	BBP::Services::Binaries::ELF::Runtime::Executable *executable = BBP::Services::Binaries::ELF::Runtime::E_LoadExecutable(argTable->strings[0]);

	// Get function
	int (*sub5)(int) = (int (*)(int))BBP::Services::Binaries::ELF::Runtime::E_getFunction(executable, "sub5");
	int (*add5)(int) = (int (*)(int))BBP::Services::Binaries::ELF::Runtime::E_getFunction(executable, "add5");
	int (*add10)(int) = (int (*)(int))BBP::Services::Binaries::ELF::Runtime::E_getFunction(executable, "add10");

	int test = add5(10);

	printf("10 + 5 = %d\n", test);


	// Save for the lels
	BBP::IO::SDMMC::writeFile("linked.o", executable->data, executable->fileSize);
	free(executable->data);

	printf("Output ELF file\n");


	BBP::Services::Binaries::ELF::Runtime::E_DestroyExecutable(executable);

	printf("Executable destroyed.\n");

	// Destroy stuff
	BBP::IO::Buffers::setStringTableSize(argTable, 0);
	delete(argTable);

	return 0;
}