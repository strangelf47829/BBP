#include "../include/ELSA/Symbol.h"

// Create a new symbol
BBP::elsa::symbol_t::symbol_t(std::ResourceManager *allocator)
{

}

// Default constructor (Needed for identifiable constraint)
BBP::elsa::symbol_t::symbol_t()
{

}

// Allocate space and set name
void BBP::elsa::symbol_t::setUniqueName(std::c_string)
{

}

// Set value to be constant, and also specifiy size
void BBP::elsa::symbol_t::setConstantValue(std::word, std::DYN_PAGE::Alignment)
{

}

// Hashing operator
BBP::elsa::symbol_t::operator std::hash_t() const
{

}

// Comparison function
bool BBP::elsa::symbol_t::operator ==(symbol_t &b)
{

}

bool BBP::elsa::symbol_t::operator !=(symbol_t &b)
{

}