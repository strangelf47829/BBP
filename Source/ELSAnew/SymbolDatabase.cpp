#include "../include/ELSA/Symbol.h"

void BBP::elsa::symbol_db::setDataSection(Section *sect) { dataSection = sect; }
void BBP::elsa::symbol_db::setStringSection(Section *sect) { stringSection = sect; }

// Initialize a symbol
BBP::elsa::symbol_db::symhandle_t BBP::elsa::symbol_db::initSymbol()
{
	// Initialize symbols
	activeSymbol = allocator.Register(new symbol_t());
	
	// Set sections
	activeSymbol->setDataTable(dataSection);
	activeSymbol->setStringTable(stringSection);
}

// Set symbol name
void BBP::elsa::symbol_db::setSymbolName(std::string str)
{
	// If no active symbol, do nothing
	if (activeSymbol == nullptr)
		std::raise(std::SIGSEGV);

	// Set name
	activeSymbol->setUniqueName(str);
}

// Allocate data
void BBP::elsa::symbol_db::dynamic(std::size_t bytes)
{
	// If no active symbol, do nothing
	if (activeSymbol == nullptr)
		std::raise(std::SIGSEGV);

	// Allocate data
	activeSymbol->initializeDynamic(bytes);
}

// Stream
void BBP::elsa::symbol_db::stream(std::index_t idx, std::byte val) { if (activeSymbol == nullptr) std::raise(std::SIGSEGV); activeSymbol->write(idx, val); }
void BBP::elsa::symbol_db::stream(std::index_t idx, std::string val) { if (activeSymbol == nullptr) std::raise(std::SIGSEGV); activeSymbol->write(idx, val); }
void BBP::elsa::symbol_db::stream(std::index_t idx, std::PAGE<std::byte> &val) { if (activeSymbol == nullptr) std::raise(std::SIGSEGV); activeSymbol->write(idx, val); }

void BBP::elsa::symbol_db::stream(std::index_t idx, std::size_t amount, std::string val) { if (activeSymbol == nullptr) std::raise(std::SIGSEGV); activeSymbol->write(idx, amount, val); }
void BBP::elsa::symbol_db::stream(std::index_t idx, std::size_t amount, std::PAGE<std::byte> &val) { if (activeSymbol == nullptr) std::raise(std::SIGSEGV); activeSymbol->write(idx, amount, val); }


// Set constant value
void BBP::elsa::symbol_db::constant(std::word val, std::DYN_PAGE::Alignment size)
{
	// If active symbol is nullptr, do nothing
	if (activeSymbol == nullptr) 
		std::raise(std::SIGSEGV);
	
	// Prepare constant
	activeSymbol->initializeConstant(val, size);
}

void BBP::elsa::symbol_db::setValue(std::word val)
{
	// If active symbol is nullptr, do nothing
	if (activeSymbol == nullptr)
		std::raise(std::SIGSEGV);

	// Write
	activeSymbol->write(0, val);
}

// Save symbol
void BBP::elsa::symbol_db::UploadSymbol()
{
	// Create symboldb entry
	symbol_db_entry value(activeSymbol);

	// Save symbol
	dictionary.add(activeSymbol->identifier.name, value);

	// Reset active symbol
	activeSymbol = nullptr;
}

BBP::elsa::symbol_db::symhandle_t BBP::elsa::symbol_db::createSymbol(std::string ident, std::string data)
{
	// Create symbol
	symhandle_t handle = initSymbol();

	// Set its name
	setSymbolName(ident);

	// Get string length
	std::size_t stringLength = std::strlen(data) + 1;

	// Allocate that data, then write
	dynamic(stringLength);
	stream(0, data);

	// Finally upload symbol
	UploadSymbol();

	// Return handle
	return handle;
}

BBP::elsa::symbol_db::symhandle_t BBP::elsa::symbol_db::createSymbol(std::string ident, std::PAGE<std::byte> &data)
{
	// Create symbol
	symhandle_t handle = initSymbol();

	// Set its name
	setSymbolName(ident);

	// Get data length
	std::size_t stringLength = std::seqlen(data);

	// Allocate that data, then write
	dynamic(stringLength);
	stream(0, data);

	// Finally upload symbol
	UploadSymbol();

	// Return handle
	return handle;
}

BBP::elsa::symbol_db::symhandle_t BBP::elsa::symbol_db::createSymbol(std::string ident, std::size_t count, std::PAGE<std::byte> &data)
{
	// Create symbol
	symhandle_t handle = initSymbol();

	// Set its name
	setSymbolName(ident);

	// Allocate that data, then write
	dynamic(count);
	stream(0, count, data);

	// Finally upload symbol
	UploadSymbol();

	// Return handle
	return handle;
}

BBP::elsa::symbol_t *BBP::elsa::symbol_db::operator[] (std::string str)
{
	return dictionary[str].sym;
}

// Reset everything
void BBP::elsa::symbol_db::Reset()
{
	// If data section, clear data section
	if (dataSection)
	{
		dataSection->Reset();
		dataSection = nullptr;
	}

	// If string section, clear string section
	if (stringSection)
	{
		stringSection->Reset();
		stringSection = nullptr;
	}

	// Reset allocators
	allocator.deleteAll();
}