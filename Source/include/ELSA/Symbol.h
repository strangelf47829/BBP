#ifndef BBP_ELSA_SYMBOL_H
#define BBP_ELSA_SYMBOL_H

// Create a structure that defines a symbol
#include "../Strings.h"
#include "../Dictionary.hxx"
#include "Numerical.h"

namespace BBP
{

	namespace elsa
	{

		// Type definition for symbol attributes
		using symbol_attrib_t = std::word;

		class symbol_data_t
		{
			// The allocator used to allocate this data. If not allocated, this is nullptr.
			std::ResourceManager *allocator;

			// The symbol data
			std::PAGE<std::byte> allocated_data;

			// Dynamic page (this is what is used to read and write to allocated data
			std::DYN_PAGE dynamic_data;

		public:

			// Link
			// Create constant
			// etc...

		};

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

			// Info - implementation defined
			std::word info;

			// Other - implementation defined
			std::word other;

			// Allocator used to make name
			std::ResourceManager *allocator;
		};

		// This class holds all the entities needed to store a single symbol
		class symbol_t
		{
		public:

			symbol_data_t data;
			symbol_section_data_t sectionInfo;
			symbol_debug_t debugInfo;
			symbol_identifier_t identifier;

			// Create a new symbol
			symbol_t(std::ResourceManager *);

			// Allocate space and set name
			void setUniqueName(std::c_string);

			// Set value to be constant, and also specifiy size
			void setConstantValue(std::word, std::DYN_PAGE::Alignment);

			// Hashing operator
			explicit operator std::hash_t () const;

			// Comparison function
			bool operator ==(symbol_t &b);
			bool operator !=(symbol_t &b);

			// Default constructor
			symbol_t();

		};

		// Database for symbols
		class symbol_db
		{
			
			// Dictionary for symbols
			std::HashedDictionary<std::string, symbol_t> dictionary;

			// Own page allocator for strings etc...
			std::pageAllocator stringAllocator;

			// Currently active symbol
			symbol_t activeSymbol;

		public:

			// Type for symbol handle. A handle is used externally to refer to different symbols.
			using symhandle_t = std::index_t;

			// Initialize a new symbol.
			symhandle_t initSymbol();

			// Set active symbol to handle. Returns true on success, false on failure.
			bool setActiveSymbol(symhandle_t);

			// Set active symbol based on symbol name (may or may not be mangled)
			bool setActiveSymbol(std::string &);
			bool setActiveSymbol(std::string &&);

			// Set constant value
			void setConstantValue(Numerical_t);

			// Copy over data
			void copyData();

		};


	}
}

#endif
