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

void BBP::elsa::TranslationUnit::emitInterpreter(std::string str)
{
	// Get string size
	std::size_t length = std::strlen(str);

	// Create page for str
	std::PAGE<std::byte> interpreterString;

	// Then allocate bytes
	application[".strtab"]->Allocate(interpreterString, length + 1);

	// Then copy over
	for (std::index_t idx = 0; idx < length; idx++)
		interpreterString[idx] = str[idx];

	// Set null bytes
	interpreterString[length] = 0;
}