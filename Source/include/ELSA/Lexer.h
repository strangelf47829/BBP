#ifndef BBP_ELSA_LEXER_H
#define BBP_ELSA_LEXER_H

#include "../FileSys.h"

namespace BBP
{
	namespace elsa
	{
		// set maximum input depth
		constexpr std::size_t maximumLexerDepth = 10;


		// Input stream for a lexer
		class LexerInputStream_t
		{
			// Stack
			std::Stack<std::string_element> *stk;

		public:

			// Create a stream from a file
			LexerInputStream_t(std::FILE &);

			// Retrieve a character and advace the lexer
			std::string_element Consume();

			// Check if still empty
			bool hasData();
		};

		class Lexer
		{
			// Static page
			std::STATIC_PAGE<LexerInputStream_t, maximumLexerDepth> inputStreams;

			// What page are we on?
			std::index_t activeInputStream;

		public:

			// Open an input stream
			void open(LexerInputStream_t);

			// Perform a single fetch action (without processing anything)
			void Fetch();

			// Perform a single processing action (Update state basically)
			void Process();

			// Perform a single dequeue action (Perform queued actions)
			void Dequeue();

			// Perform a single backlog action (Check for debug emissions - Errors - Etc...)
			void Backlog();

		};

	}
}

#endif