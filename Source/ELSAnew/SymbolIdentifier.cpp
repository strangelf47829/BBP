#include "../include/ELSA/Symbol.h"

BBP::elsa::symbol_identifier_t::symbol_identifier_t()
{
	// IDK
}

BBP::elsa::symbol_identifier_t::symbol_identifier_t(Section *section, std::string _name)
{
	// If section is invalid, signal
	if (section == nullptr)
		std::raise(std::SIGSEGV);

	// Get length of string
	std::size_t stringLength = std::strlen(_name) + 1;

	// Create page where data will be stored
	std::PAGE<std::byte> data;

	// Allocate that many bytes in memory
	nameOffset = section->Allocate(data, stringLength);

	// Set page
	name = std::string(stringLength, (std::string_element *)data.data);

	// Now copy data
	for (std::index_t idx = 0; idx < stringLength; idx++)
		name[idx] = _name[idx];

	// Now precompute hash
	hash = std::strhsh(name);
}