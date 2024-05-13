#ifndef BBP_ELSA_LEXERFRONTEND_H
#define BBP_ELSA_LEXERFRONTEND_H

#include "LexerState.h"
#include "Processor.h"
#include "Keywords.h"

namespace BBP
{
	namespace std
	{

		// A Lexer has two stages: The consumption stage, the action phase.
		// The Lexer consumes one character from an input stream. It then 

		class Lexer_db
		{


		public:

			// Keywords with actions
			BBP::elsa::keyword_db *kdb;

		};

	}
}

#endif