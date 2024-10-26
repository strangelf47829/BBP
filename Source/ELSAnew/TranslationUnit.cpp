#include "../include/ELSA/TranslationUnit.h"

BBP::elsa::TranslationUnit::TranslationUnit()
	: keywords(&keywordAllocator, 16, 16)
{
	//application.emitFile("/home/new2.out");
}

BBP::elsa::TranslationUnit::~TranslationUnit()
{
	// Clean up the application
	keywordAllocator.freeAll();
}

void BBP::elsa::TranslationUnit::Reset()
{
	// Clean up keyword allocator
	keywordAllocator.freeAll();

	// Then reset application
	application.Reset();

	// Then reset symbol_db
	symbols.Reset();

}