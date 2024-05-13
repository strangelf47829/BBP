#ifndef BBP_ELSA_LEXERINPUT_H
#define BBP_ELSA_LEXERINPUT_H

#include "../Strings.h"

namespace BBP
{
	namespace elsa
	{

		// This structure is used by the lexer input class to record which input stream is used where.
		struct Lexer_Input_Record_t
		{

		};

		// This class provides an interface the ability to move back and forth between different input streams seamlessly.
		// One example of this is through including files.
		// The lexer can queue

		class Lexer_Input_t
		{
			
		public:

			// Get a character from this stream.
			std::string_element fetchCharacter();

		};

	}
}

#endif