#include "../include/ELSA/Keywords.h"

// Keyword identifier constructor
BBP::elsa::keyword_identifier_t::keyword_identifier_t(std::conststring str)
	: identifierLength(std::strlen(str)), mask(0), identifier(str), handle(0)
{}

BBP::elsa::keyword_identifier_t::keyword_identifier_t(std::string str)
	: identifierLength(std::strlen(str)), mask(0), identifier(str), handle(0)
{}

// Keyword constructor
BBP::elsa::keyword_t::keyword_t(keyword_identifier_t ident, keyword_functor_t *func)
	: identifier(ident), functor(func), hashedIdentifier(0)
{
	// Hash identifier
	hashedIdentifier = std::strhsh(ident.identifier);
}

// Advance the state of a keyword by one
bool BBP::elsa::keyword_t::Advance(std::string_element element)
{
	// If element is equal to character at mask, increase mask
	if (element == identifier.identifier[identifier.mask])
		identifier.mask++;
	else
		identifier.mask = 0; // Otherwise reset mask

	// Return true if identifier mask is equal to identifier size
	return identifier.mask >= identifier.identifierLength;
}

// Reset a keyword
void BBP::elsa::keyword_t::Reset()
{
	// Reset mask
	identifier.mask = 0;
}

// Operate on virtual object
BBP::elsa::keyword_return_t BBP::elsa::keyword_t::operator() (TranslationUnit &unit, std::index_t idx, keyword_t &word)
{
	// If no functor is available, throw error
	if (functor == nullptr)
		throw std::exception("No functor defined for keyword", ENODATA);

	// Otherwise return functor
	return (*functor)(unit, idx, word);
}
