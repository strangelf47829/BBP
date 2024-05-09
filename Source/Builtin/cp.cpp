#include "../include/Shell.h"
#include "../Daemons/initd.h"
#include "../include/TerminalController.h"
#include "../include/stdio.h"
#include "../include/Kernel.h"
#include "../include/DriverCommands.h"
#include "../include/ELSA.h"
#include "../include/BuiltinShell.h"
#include "../include/ELSA/Symbol.h"
#include "../include/Hashmap.h"
#include "../include/Concepts.h"
#include "../include/Dictionary.hxx"
#include "../include/ELSA/Numerical.h"
#include "../include/SequentialMemory.h"
#include "../include/ELSA/SectionDB.h"

// Symbol database
BBP::elsa::symbol_db database;
BBP::elsa::section_db database2;

BBP::elsa::Section *dataSection;
BBP::elsa::Section *stringTableSection;

BBP::std::errno_t BBP::system::cp_builtin(std::size_t argc, std::c_string *argv)
{
	// Set stuff in database
	database2.createSection(".text");
	database2.createSection(".strtab");

	// Create new section
	dataSection = &database2[".text"];
	stringTableSection = &database2[".strtab"];

	// Initialize symbol database
	database.setDataSection(dataSection);
	database.setStringSection(stringTableSection);

	database.createSymbol("_ZN3fd", "Hello, ");
	database.createSymbol("_ZN3bar", "world!");
	
	std::printf("Found: %s\n\n", database["_ZN3fd"]->identifier.name.data);


	// Print out data
	std::size_t stringTableLength = stringTableSection->size();
	std::size_t dataSectionLength = dataSection->size();

	// Print out info
	std::printf("String table size: %u:\n", stringTableLength);

	// Print stuff
	for (std::index_t idx = 0; idx < stringTableLength; idx++)
		std::printf("(%c)", (*stringTableSection)[idx]);
	kernelSS()->activeContext->STDOUT <<= '\n';

	// Print out info
	std::printf("\n\nData size: %u:\n", dataSectionLength);

	// Print stuff
	for (std::index_t idx = 0; idx < dataSectionLength; idx++)
		std::printf("(%c)", (*dataSection)[idx]);
	kernelSS()->activeContext->STDOUT <<= '\n';

	database.Reset();
	database2.Reset();

	return 0;
}
