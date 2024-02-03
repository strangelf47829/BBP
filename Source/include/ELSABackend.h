#ifndef BBP_STDLIB_ELSABACKEND_H
#define BBP_STDLIB_ELSABACKEND_H

#include "ELF.h"
#include "FileSys.h"
#include "Lex.h"

namespace BBP
{
	namespace esa
	{

		struct dataEntry
		{
			// The data to be encoded into data
			std::conststring data;

			// The size of the data to be encoded into data
			std::size_t size;

			// The actual location of the data within rodata
			std::c_string elfData;

			// The offset within rodata of this data
			std::offset_t offset;

			// The corresponding symbol for this entry
			std::index_t symbol;

			// The type of symbol. Yeah yeah it is stored in builder.symbols[symbol].info() but I dont give a shit I want it here fuck you.
			std::byte type;

			// The data hash
			std::hash_t hash;

			dataEntry(std::conststring msg, std::size_t len)
				: data(msg),
				size(len),
				elfData(nullptr),
				offset(0),
				symbol(0),
				type(0),
				hash(0)
			{}

			dataEntry(std::conststring msg, std::size_t len, std::byte _type)
				: data(msg),
				size(len),
				elfData(nullptr),
				offset(0),
				symbol(0),
				type(_type),
				hash(0)
			{}

		};

		class BinaryApplication
		{
		public:

			BinaryApplication(std::conststring fileName, std::size_t extraSegments, std::size_t extraSections, std::size_t extraSymbols);

			// Adds some data into a section
			void addDataToSection(std::index_t, dataEntry &);

			// Adds a symbol (and does not emit symbol data! use addDataToSection for that!)
			void addSymbol(dataEntry &, std::conststring);
			void addSymbol(dataEntry &, std::conststring, std::index_t);

			// Emits a symbol, at the specified section, including its data. Also provides ability to override symbol.
			void emitSymbol(std::index_t, dataEntry &, std::conststring, std::hash_t, std::index_t);

			// Add symbol hash
			bool emitSymbolHash(std::hash_t hash, std::index_t symbol);

			// Shorthand for emitting source file name
			void setSource(std::conststring);

			// add N amount of symbols
			void appendSymbols(std::index_t);

			// Name mangling functions
			void mangle(std::conststring name, std::Lexer::lex_thing &thing, dataEntry &entry);

			// This is what builds the application
			std::ELF::ELFBuilder builder;
			std::Stack<std::string_element> builderStack;
			std::FILE file;

			// Static string to mangle names
			std::static_string<128> mangledPage;
			std::Stack<std::string_element> mangled;

			void save();
			void close();

		};

	}
}

#endif