#include "../include/ELSA/Symbol.h"

// Empty constructor
BBP::elsa::symbol_t::symbol_t()
	: sectionInfo({ 0, 0, 0 }), data(), debugInfo({ 0, 0, 0 }), identifier()
{

}

// Setters
void BBP::elsa::symbol_t::setDataTable(Section *data) { dataTable = data; }
void BBP::elsa::symbol_t::setStringTable(Section *data) { stringTable = data; }

// Initialize
bool BBP::elsa::symbol_t::initializeConstant(std::word value, std::DYN_PAGE::Alignment alignment)
{
	// If either data table or string table is null, return false
	if (dataTable == nullptr || stringTable == nullptr)
		return false;

	// Construct items
	data = symbol_data_t(sectionInfo, dataTable, alignment, value);
}

// Initialize
bool BBP::elsa::symbol_t::initializeDynamic(std::size_t size)
{
	// If either data table or string table is null, return false
	if (dataTable == nullptr || stringTable == nullptr)
		return false;

	// Construct items
	data = symbol_data_t(sectionInfo, dataTable, size);
}


// Allocate space and set name
void BBP::elsa::symbol_t::setUniqueName(std::string str)
{
	// Create new identifier
	identifier = symbol_identifier_t(stringTable, str);
}

// Write
void BBP::elsa::symbol_t::write(std::index_t idx, std::word value)
{
	data.write(idx, value);
}

void BBP::elsa::symbol_t::write(std::index_t idx, std::string value)
{
	data.write(idx, value);
}

void BBP::elsa::symbol_t::write(std::index_t idx, std::PAGE<std::byte> value)
{
	data.write(idx, value);
}

void BBP::elsa::symbol_t::write(std::index_t idx, std::size_t amount, std::string value)
{
	data.write(idx, amount, value);
}

void BBP::elsa::symbol_t::write(std::index_t idx, std::size_t amount, std::PAGE<std::byte> value)
{
	data.write(idx, amount, value);
}

// Read
BBP::std::word BBP::elsa::symbol_t::operator[] (std::index_t idx)
{
	return data.read(idx);
}

// Hashing operator
BBP::elsa::symbol_t::operator std::hash_t() const
{
	return identifier.hash;
}

// Comparison function
bool BBP::elsa::symbol_t::operator ==(symbol_t &b)
{
	return false;
}

bool BBP::elsa::symbol_t::operator !=(symbol_t &b)
{
	return true;
}

BBP::elsa::symbol_db_entry::operator BBP::std::hash_t() const
{
	// If no symbol is present, signal
	if (sym == nullptr)
		std::raise(std::SIGSEGV);

	// Return hash
	return (std::hash_t)(*sym);
}

// Constructors
BBP::elsa::symbol_db_entry::symbol_db_entry() : sym(nullptr) {}
BBP::elsa::symbol_db_entry::symbol_db_entry(symbol_t *s) : sym(s) {}

// Comparators
bool BBP::elsa::symbol_db_entry::operator==(symbol_db_entry &b)
{
	if (b.sym == nullptr)
		std::raise(std::SIGSEGV);

	return (*sym) == (*b.sym);
}

bool BBP::elsa::symbol_db_entry::operator!=(symbol_db_entry &b)
{
	if (b.sym == nullptr)
		std::raise(std::SIGSEGV);

	return (*sym) != (*b.sym);
}