#include "../include/ELSA/Symbol.h"
#include "../include/ELSA/ELFSymbol.h"

void BBP::elsa::symbol_db::setDataSection(Section *sect) { dataSection = sect; }
void BBP::elsa::symbol_db::setStringSection(Section *sect) { stringSection = sect; }
void BBP::elsa::symbol_db::setSymbolSection(Section *sect) { symbolSection = sect; }
void BBP::elsa::symbol_db::setRelocationSection(Section *sect) { relocationSection = sect; }

BBP::elsa::symbol_db::symbol_db()
	: dataSection(nullptr), stringSection(nullptr), symbolSection(nullptr), relocationSection(nullptr), currentStream(0), activeEntry(nullptr), activeSymbol(nullptr),
	headEntry(nullptr), uniqueCount(0) {}

// Initialize a symbol
BBP::elsa::symbol_db::symhandle_t BBP::elsa::symbol_db::initSymbol()
{
	// Initialize symbols
	activeSymbol = allocator.Register(new symbol_t());
	
	// Set sections
	activeSymbol->setDataTable(dataSection);
	activeSymbol->setStringTable(stringSection);

	return uniqueCount++;
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
	symbol_db_entry *newActiveEntry = &dictionary.add(activeSymbol->identifier.name, value);

	// If activeEntry,
	if (activeEntry)
		activeEntry->next = newActiveEntry;

	// Then set active entry
	activeEntry = newActiveEntry;

	// If no head entry, set it here
	if (headEntry == nullptr)
		headEntry = activeEntry;

	// Reset active symbol
	activeSymbol = nullptr;
}

BBP::elsa::symbol_db::symhandle_t BBP::elsa::symbol_db::createSymbol(std::string ident)
{
	// Create symbol
	symhandle_t handle = initSymbol();

	// Set its name
	setSymbolName(ident);

	// Set index to 0
	activeSymbol->sectionInfo.sectionIndex = 0;

	// Then upload
	UploadSymbol();

	return handle;	
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

	// If symbol section, clear symbol section
	if (symbolSection)
	{
		symbolSection->Reset();
		symbolSection = nullptr;
	}

	// If relocation section, clear relocation section
	if (relocationSection)
	{
		relocationSection->Reset();
		relocationSection = nullptr;
	}

	// Reset allocators
	allocator.deleteAll();

	// Reset dictionary
	dictionary.Reset();

	// Reset count
	currentStream = 0;

	// Reset active symbols, etc...
	activeSymbol = nullptr;
	activeEntry = nullptr;
	headEntry = nullptr;

	// Set unique count back to 0
	uniqueCount = 0;

}

// Upload each symbol to binary
void BBP::elsa::symbol_db::UploadToBinary(bool endian)
{
	// Must have symbol section defined
	if (symbolSection == nullptr)
		std::raise(std::SIGSEGV);

	// Symbol data -- This is where all data is stored
	std::PAGE<std::byte> globalSymbolData;

	// Size of symbol entry
	std::size_t symbolEntrySize = 16;

	// Symbol object
	ELF::Symbol sym;

	// Get active symbol
	symbol_db_entry *activeSymbol = headEntry;

	// Get object count
	std::size_t symCount = 0;

	// Count symbols
	while (activeSymbol)
	{
		activeSymbol = activeSymbol->next;
		symCount++;
	}

	// Then reset active symbol
	activeSymbol = headEntry;

	// If no symbols are set, do nothing
	if (symCount == 0)
		return;

	// Allocate bytes to global symbol data
	symbolSection->Allocate(globalSymbolData, symbolEntrySize * symCount);

	// Then, update
	for (std::index_t symIndex = 0; symIndex < symCount; symIndex++)
	{
		// Set page
		sym.data = std::PAGE<std::byte>(symbolEntrySize, globalSymbolData.data + symbolEntrySize * symIndex);

		// If active symbol has a symbol, set data
		if (activeSymbol->sym)
		{
			// Set name
			sym.name = activeSymbol->sym->identifier.nameOffset;

			// Set size
			sym.size = activeSymbol->sym->sectionInfo.dataSize;

			// Set value
			sym.value = activeSymbol->sym->sectionInfo.sectionOffset;

			// Set index
			sym.shndx = activeSymbol->sym->sectionInfo.sectionIndex;

			// Set info and other
			sym.info = activeSymbol->sym->identifier.info;
			sym.other = activeSymbol->sym->identifier.other;
		}

		// Otherwise do nothing
		else
		{
			sym.Nullify();
		}

		// Then write data
		sym.writeData(endian);

		// Then update activeSymbol
		activeSymbol = activeSymbol->next;
	}
}
