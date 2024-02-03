#include "Actions.h"
#include "../include/ELSA.h"

BBP::std::Lexer::lex_keywordhandle BBP::esa::handle = 0;

BBP::std::Lexer::lex_keywordhandle BBP::esa::lastOpener;
BBP::std::Lexer::lex_keywordhandle BBP::esa::firstCloser;
BBP::std::STATIC_PAGE<BBP::std::Lexer::lex_keywordhandle, 32> BBP::esa::delimiterPage;
BBP::std::Stack<BBP::std::Lexer::lex_keywordhandle> BBP::esa::delimiterStack(&BBP::esa::delimiterPage, 32);


BBP::std::errno_t BBP::esa::ExpectDelimiters(std::Lexer::lex_context *context, std::index_t argc, std::Lexer::lex_keywordhandle const argv[], bool singleLine, std::index_t min, std::index_t maxDepth, std::Lexer::lex_keywordhandle escape, std::Lexer::lex_keywordhandle stop)
{
	// Check if context is valid
	if (context == nullptr)
		return EFAULT; // Bad context address.

	// If argc is not 0, check if argv is not null
	if (argc != 0 && argv == nullptr)
		return EFAULT; // Bad argv address.

	// If argc is not a multiple of 2, throw error
	if (argc % 2 != 0)
		return EDOM; // EDOM because odd numbers are not within the domain of this function

	// Reset delimiter stack
	delimiterStack.atElement = 0;

	// Save index of here
	std::index_t start = context->atPosition;
	context->debugInformation.highlight = start;

	// This variable stores the 'depth' of the delimiter
	std::word depth = 0;

	// Although possible through the stack alone, allocating a new variable optimizes the read time and thus the speed of this function.
	std::Lexer::lex_keywordhandle lastIndex = argc; // Initializing this to 'argc' tells the algorithm to ignore this value.

	// If true, the depth is increasing. Otherwise, decreasing.
	bool isAscending = true;

	// If false, the minimum amount of things hasn't been reached yet.
	bool hasReachedMinimum = false;

	do
	{
		// Get handle to next keyword
		handle = context->moveToNextKeyword();

		// If handle is EOF, return error
		if (handle == context->keywordStack.atElement)
			return EEOF;

		// Get keyword
		std::Lexer::lex_keyword *keyword = &context->keywordStack[handle];

		// If handle is new line or stop, and delimiter stack is not balanced, and expected within one line, return error
		if ((context->isNewLine(keyword->word[0]) || handle == stop) && depth && (singleLine || handle == stop))
			return ELINEEND;

		// Otherwise, if we have reached 'stop' normally, just break out.
		if (handle == stop && depth == 0)
			break;

		// Aux variables
		bool isIncluded = false;
		std::index_t index = 0;

		// Check if delimiter is included in argv
		for (; index < argc; index++)
		{
			if (argv[index] != handle)
				continue;
			isIncluded = true;
			break;
		}

		// If handle is escape, and also less than EOF, advance one position and then skip.
		if (handle == escape && handle != context->keywordStack.atElement)
		{
			context->atPosition++;
			continue;
		}

		// If not included, just skip
		if (!isIncluded)
			continue;

		// The first check checks to see if the handle matches the closing delimiter of the last found delimiter
		// Check to see if lastIndex is in bounds
		if (lastIndex < argc - 1)
		{
			if (argv[lastIndex + 1] == handle)
			{
				// This delimiter is the closing delimiter to the last found delimiter.
				// Decrease the depth by 1, and retrieve the last index from stack.
				depth--;
				lastIndex = delimiterStack[depth];

				// Since this must be first closer, set index here
				firstCloser = context->atPosition - keyword->length;

				// Set flag to descending
				isAscending = false;

				// Then add the handle to the stack
				delimiterStack << handle;

				// Then continue
				continue;
			}
		}
		
		// If true, this handle is for an opening keyword.
		bool isOpening = (handle % 2 == 0);

		// If is opening and ascending, overwrite previous last opener
		if (isOpening && isAscending)
			lastOpener = context->atPosition;

		// If is not opening but still ascending, its about to close. Write once to firstCloser
		if (!isOpening && isAscending)
			firstCloser = context->atPosition - keyword->length;

		// If not opening, the sequence must be descending.
		isAscending &= isOpening;

		// If the handle is closing, and the depth is 0, return error..
		if (!isOpening && depth == 0)
			return ENOANO; // No anode (because closing delimiters are cathodes).

		// If the handle is closing, and is not the expected type, return error..
		if (!isOpening && lastIndex < argc - 1)
			return EILSEQ; // Illegal sequence of delimiters.

		// If the handle is opening, and the sequence is descending, return error.
		if (isOpening && !isAscending)
			return EUNFOCS; // Try again, but with the other function.

		// Increase depth by one.
		depth++;
		hasReachedMinimum |= depth >= min;

		// If opening, check for max depth. If would cause overflow, return error.
		if (isOpening && depth > maxDepth)
			return EOVERFLOW;

		// Move handle to stack
		delimiterStack << handle;

		// Then set lastIndex to handle
		lastIndex = index;

	} while (depth);

	// If minimum has not been reached, return error
	if (!hasReachedMinimum)
		return ENOMSG; // Nothing was found, so no message.

	// If done successfully, congrats! My algorithm worked! Good job me :)
	return ENONE;
}


BBP::std::conststring BBP::esa::Frontend::setHash(std::hash_t hash)
{
	// Cannot hash if: Hash already exists
	// Symbol has already been hashed

	// Check if the symbol doesn't already have a hash
	if (symbol.hash)
		return "EIgnoring misplaced identifier.";

	symbol.hash = hash;

	return nullptr;
}

BBP::std::conststring BBP::esa::Frontend::setSize(std::size_t size)
{
	// Return error if size already defined
	if (symbol.subtype & 0b111111100000)
	{
		esaParser.debugInformation.errName = "-Wsize-redefinition-unsupported";
		return "WSize already defined: ignored size directive.";
	}
	
	switch (size)
	{
	case 0: // No size
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_NIL;
		break;
	case 4: // Nibble
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_DN;
		break;
	case 8: // Byte
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_DB;
		break;
	case 16: // Half word
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_DH;
		break;
	case 20: // Index
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_DI;
		break;
	case 24: // Address
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_DA;
		break;
	case 32: // Word
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_DW;
		break;
	default: // Unkown
		symbol.subtype |= SYMBOL_TYPE::SYMTSZ_DW;
		esaParser.debugInformation.errName = "-Wunkown-size";
		return "WUnkown size, defaulting to 32 bits.";
	}

	// If this comes at a time where a symbol isn't defined, return error
	if (!lineIsDefiningSymbolDefinition)
	{
		esaParser.debugInformation.errName = "-Wanonymous-size-declaration";
		return "WSize declaration for anonymous symbols will be ignored.";
	}

}