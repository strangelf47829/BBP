#include "../include/ELSA/Lexer.h"
#include "../include/FileSys.h"

// Open a file
void BBP::elsa::LexerStream::open(std::PATH path)
{
	// If is open, close other file first
	if (isOpen)
		data.close();

	// Open file
	data = std::FILE(path);

	// Reset
	coloumn = 0;
	line = 0;
	position = 0;

	// Mark opened
	isOpen = true;
}

// Close a file
void BBP::elsa::LexerStream::close()
{
	// Do nothing if already closed
	if (isOpen == false)
		return;

	// Close
	data.close();

	// Mark closed
	isOpen = false;
}


bool BBP::elsa::LexerStream::consume(std::string_element &character)
{
	// If file closed, return false
	if (isOpen == false)
		return false;

	// Read
	data.b() >> character;

	// If reached max, close file
	if (data.b().max_elements <= data.b().atElement)
		close();

	// Return whether the file is open or not
	return isOpen;
}