#include "../include/ELSA/Lexer.h"
#include "../include/ELSA/DebugUtils.h"
#include "../include/ELSA/Keywords.h"
#include "../include/ELSA/Processor.h"

void BBP::elsa::Lexer::open(std::PATH path)
{
	// Increment streamIndex
	streamIndex++;

	// Then open
	lexerStreams[streamIndex].open(path);
}

BBP::std::string_element BBP::elsa::Lexer::consume()
{
	// Declare element
	std::string_element character = 0;

	// Consume
	bool couldConsume = lexerStreams[streamIndex].consume(character);

	// If true, return character
	if (couldConsume)
		return character;

	// Otherwise, stream has closed. If stream index is not 0, decrement it, then return character
	if (streamIndex != 0)
	{
		streamIndex--;
		return character;
	}

	// If this point is reached, the lexer has finished everything, so return '0'
	return 0;
}

bool BBP::elsa::Lexer::consume(Debug_db &debug, keyword_db &keyword)
{
	// Consume a character
	std::string_element consumed = consume();

	// If consumed is null, do nothing
	if (consumed == 0)
		return false;

	// Then consume over \
	keyword.getCount();

	return true;
}
