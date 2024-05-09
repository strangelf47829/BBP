#ifndef BBP_ELSA_SYMBOL_H
#define BBP_ELSA_SYMBOL_H

// Create a structure that defines a symbol
#include "../Strings.h"
#include "../Dictionary.h"
#include "../State.h"
#include "SectionBuilder.h"
#include "Numerical.h"

namespace BBP
{

	namespace elsa
	{

		// Type definition for symbol attributes
		using symbol_attrib_t = std::word;


		// This struct holds the information pertaining to a symbols relation to an ELF section. 
		struct symbol_section_data_t
		{
			// The index of the section
			std::index_t sectionIndex;

			// The offset within the section data
			std::offset_t sectionOffset;

			// The size of this symbol within the section
			std::size_t dataSize;

		};

		class symbol_data_t
		{
			// The allocator used to allocate this data. If not allocated, this is nullptr.
			Section *allocator;

			// The symbol data
			std::PAGE<std::byte> allocated_data;

			// Dynamic page (this is what is used to read and write to allocated data
			std::DYN_PAGE dynamic_data;

		public:

			// Empty constructor
			symbol_data_t();

			// Constructor (Empty data)
			symbol_data_t(symbol_section_data_t &, Section *);

			// Constructor (Allocated)
			symbol_data_t(symbol_section_data_t &, Section *, std::size_t);

			// Constructor (Constant value)
			symbol_data_t(symbol_section_data_t &, Section *, std::DYN_PAGE::Alignment, std::word);

			// Read and write data
			std::word read(std::index_t);
			void write(std::index_t, std::word);

			// Write byte or string stream
			void write(std::index_t, std::string);
			void write(std::index_t, std::PAGE<std::byte>);
			void write(std::index_t, std::size_t, std::string);
			void write(std::index_t, std::size_t, std::PAGE<std::byte>);

		};

		// This is used to store debug information about a symbol
		struct symbol_debug_t
		{
			// Index to the file this symbol was defined in
			std::index_t fileIndex;

			// Line and Row information about where it was defined
			std::word coloumn;
			std::word row;

		};

		// This struct holds the information needed to differentiate symbols
		struct symbol_identifier_t
		{
			// The name of this symbol
			std::string name;

			// Precomputed hash
			std::hash_t hash;

			// Offset of identifier in string table
			std::offset_t nameOffset;

			// Info - implementation defined
			std::word info;

			// Other - implementation defined
			std::word other;

			// String table section
			Section *stringTable;

			// Constructors
			symbol_identifier_t(); // Default
			symbol_identifier_t(Section *, std::string);
		};

		// This class holds all the entities needed to store a single symbol
		class symbol_t
		{
			// String table and data section
			Section *stringTable;
			Section *dataTable;

		public:

			symbol_section_data_t sectionInfo;
			symbol_data_t data;
			symbol_debug_t debugInfo;
			symbol_identifier_t identifier;

			// Empty symbol
			symbol_t();

			// Set sections
			void setStringTable(Section *);
			void setDataTable(Section *);

			// Initialization
			bool initializeConstant(std::word, std::DYN_PAGE::Alignment);
			bool initializeDynamic(std::size_t);

			// Allocate space and set name
			void setUniqueName(std::string);

			// Read
			std::word operator[] (std::index_t);

			// Write stuff
			void write(std::index_t, std::word);
			void write(std::index_t, std::string);
			void write(std::index_t, std::PAGE<std::byte>);
			void write(std::index_t, std::size_t, std::string);
			void write(std::index_t, std::size_t, std::PAGE<std::byte>);

			// Hashing operator
			explicit operator std::hash_t() const;

			// Comparison function
			bool operator ==(symbol_t &b);
			bool operator !=(symbol_t &b);

		};

		// Structure to hold symbol pointer stuff
		struct symbol_db_entry
		{
			// Pointer
			symbol_t *sym;

			// Hashing function
			explicit operator std::hash_t() const;

			// Constructor
			symbol_db_entry();
			symbol_db_entry(symbol_t *);

			// Comparisons
			bool operator== (symbol_db_entry &);
			bool operator!= (symbol_db_entry &);
		};

		// Database for symbols
		class symbol_db
		{
			
			// Dictionary for symbols
			std::HashedDictionary<std::string, symbol_db_entry> dictionary;

			// Own Object allocator for symbols etc...
			std::objectAllocator allocator;

			// Currently active symbol
			symbol_t *activeSymbol;

			// Section information for data section and string section
			Section *dataSection;
			Section *stringSection;

			// Running count for current stream
			std::size_t currentStream;

		public:

			// Type for symbol handle. A handle is used externally to refer to different symbols.
			using symhandle_t = std::index_t;

			// Set data section, set string section
			void setDataSection(Section *);
			void setStringSection(Section *);

			// Initialize a new symbol with a unique handle.
			symhandle_t initSymbol();

			// Set name
			void setSymbolName(std::string);

			// Allocate dynamic memory
			void dynamic(std::size_t);

			// Set constant value
			void constant(std::word, std::DYN_PAGE::Alignment);

			// Write constant
			void setValue(std::word);

			// Stream data at an index
			void stream(std::index_t, std::byte);
			void stream(std::index_t, std::string);
			void stream(std::index_t, std::PAGE<std::byte> &);
			void stream(std::index_t, std::size_t, std::string);
			void stream(std::index_t, std::size_t, std::PAGE<std::byte> &);

			// Save current value in dictionary
			void UploadSymbol();

			// Quickly create a dynamic symbol and copy data
			symhandle_t createSymbol(std::string, std::string);
			symhandle_t createSymbol(std::string, std::PAGE<std::byte> &);
			symhandle_t createSymbol(std::string, std::size_t, std::PAGE<std::byte> &);

			// Lookup a symbol
			symbol_t *operator[] (std::string);

			// Reset everything
			void Reset();

		};


	}
}

#endif
