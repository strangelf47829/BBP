#ifndef BBP_ELSA_LEXER_H
#define BBP_ELSA_LEXER_H

#include "../FileSys.h"

namespace BBP
{
	namespace elsa
	{

		// Forward declaration of a debug database
		class Debug_db;

		// Forward declaration of keyword database
		class keyword_db;

		// The amount of input streams a lexer can hold
		constexpr std::size_t maxLexerDepth = 10;

		// The lexer input streams
		struct LexerStream
		{
			// The file from which this lexer is reading
			std::FILE data;

			// The position (in characters) of this stream
			std::index_t position;

			// The position (in characters) of the current line
			std::index_t coloumn;

			// The position (in characters) of the current row
			std::index_t line;

			// The current 'delimiter' thingy of the stream
			std::byte delimiterContext;

			// Consume a character
			bool consume(std::string_element &);

			// Open a stream
			void open(std::PATH);

			// close a stream
			void close();

			// Is open?
			bool isOpen;

		};

		class Lexer
		{
			// Static page with lexer data
			std::STATIC_PAGE<LexerStream, maxLexerDepth> lexerStreams;

			// Index of lexer stream
			std::index_t streamIndex;

			// Consume a character from the stream
			std::string_element consume();

			// Consume a character, and perform an action
			bool consume(Debug_db &, keyword_db &);

			// Open a new stream
			void open(std::PATH);


		};

	}
}

#endif