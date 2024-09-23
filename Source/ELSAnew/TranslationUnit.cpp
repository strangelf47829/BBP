#include "../include/ELSA/TranslationUnit.h"

BBP::elsa::TranslationUnit::TranslationUnit()
	: keywords(&keywordAllocator, 16, 16)
{
	application.emitFile("/home/new2.out");
}