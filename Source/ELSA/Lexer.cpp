#include "../include/ELSA/Lexer.h"

void BBP::elsa::Lexer::open(LexerInputStream_t stream)
{
	// Check if it is possible to push stream
	if (activeInputStream + 1 >= maximumLexerDepth)
		throw std::exception("TODO: IMPLEMENT DEBUG MESSAGE", ENODATA);

	// Push
	inputStreams[activeInputStream++] = stream;
}

void BBP::elsa::Lexer::Fetch()
{

}