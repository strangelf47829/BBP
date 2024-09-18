#include "../include/ELSA/TranslationUnit.h"

void BBP::elsa::TranslationUnit::createKeyword(std::string word, std::string action)
{
	// Create keyword
	keywords.Register(word, action);
}

void BBP::elsa::TranslationUnit::createAction(std::string word, elsa::processor_action_t action)
{
	// Create keyword
	keywords.getProcessor().Register(word, action);
}