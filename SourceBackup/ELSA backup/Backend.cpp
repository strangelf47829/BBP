#include "../include/ELSA.h"

BBP::esa::Backend BBP::esa::backend;

BBP::esa::Backend::Backend(std::conststring str)
	: application(str, 0, 0, 5)
{

}

BBP::std::errno_t BBP::esa::Backend::buildSymbol()
{
	// TODO: error handling
	std::printf("Building symbol (0x%08x), attributes: 0x%04x\n", Frontend::symbol.hash, Frontend::symbol.subtype);

	Frontend::symbol.hash = 0;
	Frontend::symbol.subtype = 0;
	lineIsDefiningSymbolDefinition = false;

	return 0;
}