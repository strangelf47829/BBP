#ifndef BBP_ELSA_TRANSLATIONUNIT_H
#define BBP_ELSA_TRANSLATIONUNIT_H

#include "Symbol.h"
#include "SectionDB.h"
#include "BinaryApplication.h"
#include "Keywords.h"
#include "Processor.h"
#include "DebugUtils.h"
#include "Lexer.h"

namespace BBP
{

	namespace elsa
	{

		class TranslationUnit
		{
		public:

			// Output
			BinaryApplication application;

			// Page allocator for all things needed
			std::pageAllocator keywordAllocator;

			// Keyword database
			keyword_db keywords;

			// Debug database
			Debug_db debug;

			// Symbols
			symbol_db symbols;

			// Lexer
			Lexer lexer;

			// Constructor
			TranslationUnit();

			// Destructor
			~TranslationUnit();

			// Reset everything
			void Reset();

			// Set interpreter
			void emitInterpreter(std::string);

			// create new keyword
			void createKeyword(std::string, std::string);

			// create a new action
			void createAction(std::string, elsa::processor_action_t);

			// create the binary file with a preset amount of sections and such
			void createBinaryFile();

			// Include a file
			void lexFile(std::PATH path);

		};

	}
}

#endif