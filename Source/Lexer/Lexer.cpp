#include "Lexer.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace BBP::Services::Lexer;

void BBP::Services::Lexer::L_createLexer(lex_context **context, const char *filePath)
{
	char *data = nullptr;
	
	std::ifstream my_file(filePath, std::ios::binary);
	my_file.seekg(0, std::ios_base::end);
	unsigned int length = my_file.tellg();
	my_file.seekg(0);
	
	data = (char *)calloc(length + 1, sizeof(char));

	my_file.read(data, length);
	my_file.close();

	data[length] = 0;

	// If data is not null, free
	if ((*context)->data)
		free((*context)->data);

	// Now set context data to argument
	(*context)->data = data;

	// Set context data size
	(*context)->dataSize = length;

	// Reset lexer position
	(*context)->atLocation = 0;
}

void BBP::Services::Lexer::L_destroyLexer(lex_context *context)
{
	if (context == nullptr)
		return; // Cannot destroy lexer that does not exist.

	// Free data if data is available
	if (context->data != nullptr)
		free(context->data);

	context->data = nullptr;

	// Free keyword words, since they were copied
	for (unsigned int kw = 0; kw < context->keywordCount; kw++)
		free(context->words[kw].word);

	// Delete the entire keyword stuff
	free(context->words);

	// Delete the context
	delete(context);	
}

void BBP::Services::Lexer::L_insert(lex_context *context, unsigned int at, char *str)
{
	unsigned int strLen = 0;
	for (;str[strLen];strLen++)
	{ }

	// Save old data
	char *old = (char *)calloc(context->dataSize, sizeof(char));
	for (unsigned int idx = 0; idx < context->dataSize; idx++)
		old[idx] = context->data[idx];

	// Free old stuff
	free(context->data);

	// Create new buffer
	context->data = (char *)calloc(context->dataSize + strLen, sizeof(char));

	// Copy old data into new
	unsigned int index = 0;
	for (; index < at; index++)
		context->data[index] = old[index];

	for (; index < at + strLen; index++)
		context->data[index] = str[index - at];

	for (; index < context->dataSize + strLen; index++)
		context->data[index] = old[index - strLen];

	free(old);
	context->dataSize += strLen;
}

void BBP::Services::Lexer::L_insertNewline(lex_context *context, unsigned int at, char *str)
{
	unsigned int nlIndex = at;
	for (; context->data[nlIndex - 1] != '\n'; nlIndex++);
	L_insert(context, nlIndex, str);
}

void BBP::Services::Lexer::L_setKeywords(lex_context *context, unsigned int count)
{
	// Check for valid context
	if (context == nullptr)
		return;

	// Check if keywords already exist
	if (context->keywordCount)
		return;

	// Create array
	context->words = (lex_keyword *)calloc(count, sizeof(lex_keyword));
	context->keywordCount = count;
}

void BBP::Services::Lexer::L_expandKeywords(lex_context *context, unsigned int amount)
{
	// Check for valid context
	if (context == nullptr)
		return;

	// Check if keywords already exist
	if (!context->keywordCount)
		return;

	// Create array
	lex_keyword *arr = (lex_keyword *)calloc(context->keywordCount + amount, sizeof(lex_keyword));

	// Copy data
	for (unsigned int kw = 0; kw < context->keywordCount; kw++)
		arr[kw] = context->words[kw];

	// Free old data
	free(context->words);

	// Overwrite old array
	context->words = arr;

	// Update count
	context->keywordCount += amount;
}

lex_keyword *BBP::Services::Lexer::L_setKeywordTo(lex_context *context, const char *_word, unsigned int index, const char *(*action)(lex_context *, unsigned int, unsigned int))
{
	// Check for valid context
	if (context == nullptr)
		return nullptr;

	// Check for valid word
	if (_word == nullptr)
		return nullptr;

	// Check for non-empty string
	if (_word[0] == 0)
		return nullptr;

	// Check for valid index
	if (index >= context->keywordCount)
		return nullptr;

	// Create buffer
	context->words[index] = lex_keyword({nullptr, 0, 0, action});

	// Calculate string size
	for (;_word[context->words[index].length];context->words[index].length++)
	{ }

	// Allocate data to word
	context->words[index].word = (char *)calloc(context->words[index].length, sizeof(char));

	// Copy data
	for (unsigned int i = 0; i < context->words[index].length; i++)
		context->words[index].word[i] = _word[i];

	return &context->words[index];
}

void BBP::Services::Lexer::L_pushString(lex_context *context, unsigned int index, unsigned int length)
{
	// Check valid context
	if (context == nullptr)
		return;

	// Push string termination character
	if (L_pushCharacter(context, index + length, '\0'))
	{
		// Error
		L_Log(context, index, "String stack", "ECould not push string.");
		return;
	}

	// Set string register to 'index'
	context->strRegister = context->data + index;

	// Update strRegisterIndex
	context->strRegisterIndex = context->charStackIndex - 1;
	
}

void BBP::Services::Lexer::L_popString(lex_context *context)
{
	// Check valid context
	if (context == nullptr)
		return;

	// Attempt to pop string
	if (context->strRegisterIndex + 1 == context->charStackIndex)
		L_popCharacter(context); // The current character is this strings null terminator
	else
		L_Log(context, context->atLocation, "L_popString", "NCould not pop string: Invalid stack index.");

	// Invalidate data
	context->strRegister = 0;
	
}

unsigned int BBP::Services::Lexer::L_findNextKeyword(unsigned int *index, lex_context *context)
{
	unsigned int kwIndex = 0;
	char character = 0;
	bool isEscapeKWWithinEnforcedKWs = context->escapeIndex >= context->enforceIndicies;
switch_whilecontinue:
	while (context->atLocation < context->dataSize)
	{
		// Get character
		character = context->data[context->atLocation++];
	
		// Since this increases once per character, we know exactly that there is a parity
		context->atCol++;

		// for each word, do the thing
		for (kwIndex = 0; kwIndex < context->keywordCount; kwIndex++)
		{

			unsigned int switchCond = (isEscapeKWWithinEnforcedKWs) |
				(kwIndex < context->enforceIndicies) << 1 |
				(kwIndex == context->escapeIndex + 1) << 2;

			if (context->isInLiteralContext)
				switch(switchCond)
				{
					case 0b010: // Below enforced indicies, and escape is before enforced indicies start, so switch to escape sequence
						kwIndex = context->escapeIndex;
						break;

					case 0b110: // After escape index, but before enforced indicies, so switch to enforced indicies
					case 0b011: // Below enforced indicies, and escape is after enforced indicies start, so switch to enforced indicies
						kwIndex = context->enforceIndicies;
						break;

					case 0b101: // Escape index is after enforced indicies, and we are past escape index, so we stop.
						goto switch_whilecontinue;
						break;
					default:
					break;
				}

			// If character is white space, no point in checking for keywords.
			if ((character == ' ' || character == '\n' || character == '\t' || character == '\r' || character == 1) && !context->isInLiteralContext)
			{
				// Newline is preserved if it is the first newline, otherwise character becomes 1. Spaces are always preserved.
				if (character != ' ')
					context->data[context->atLocation - 1] = 
					((character == '\n' || character == '\r') && !context->wasLastCharWhitespace) ? '\n' : 1;


				// DEBUG
				// If character is newline, increase row count, and reset Column
				if (character == '\n')
				{
					context->atLine++;
					context->atCol = 0;
				}


				// This character was whitespace, so set 'waslast...' to true
				context->wasLastCharWhitespace = true;
				
				// Set last white space to here
				context->lastWhiteSpace = context->atLocation;
				
				goto switch_whilecontinue;
			}
			else
			{
				// Character was not whitespace, so reset this flag
				context->wasLastCharWhitespace = false;
			}
				

			// Just for infixation, We also want to do the following:
			//TODO: This should be based on token, not hardcoded.
			if ((character == '.' || character == ':' || character == '(') && !context->isInLiteralContext && false)
			{
				// Set last white space to here
				context->lastWhiteSpace = context->atLocation;
			}
				
			// If character is the same, add one to mask. Otherwise, set to 0.
			context->words[kwIndex].mask += 
				(character == (context->words[kwIndex].word[context->words[kwIndex].mask])) ? 
					1 :
					-context->words[kwIndex].mask;
					
			// If the mask is not the same as the length of the keyword, keyword was not found
			if (context->words[kwIndex].mask != context->words[kwIndex].length)
				continue; // Continue searching

			// If this evaluates to true, it means that the keyword is infixed.
			// This will cause problems, as 'main' or 'print' contains an infixed 'in'.
			bool isToken = !(context->tokenIndex <= kwIndex && context->tokenIndex + context->tokenCount > kwIndex);
			bool isInfixed = context->atLocation - context->lastWhiteSpace - context->words[kwIndex].length;
			if (!isToken && isInfixed)
				continue;

			// Index is just where we are now, minus string length
			*index = context->atLocation - context->words[kwIndex].length + 1 /*Pesky off-by-one!*/;

			// Reset mask, since keywords may appear after eachother
			context->words[kwIndex].mask = 0;
			
			return kwIndex; // The position is already stored in 'index'.
		}
	}

	// Nothing was found
	return context->keywordCount;
}

void BBP::Services::Lexer::L_pushNextWord(unsigned int *index, lex_context *context)
{
	// Check valid context
	if (context == nullptr)
		return;

	// Check valid index
	if (index == nullptr)
		return;

	// Continue until not in white space
	for (;;)
	{
		// Check if within bounds
		if (*index >= context->dataSize)
			return;

		// Continue if in whitespace
		if (!L_isWhitespace(context, *index))
			break;
			
		*index++;
	}

	// Store index 
	unsigned int strStart = *index;

	// Find end of string
	for (unsigned int length = 0; *index < context->dataSize; (*index)++)
	{
		// Increment length
		length++;

		char c = context->data[*index];

		// Check for nullspace
		if (!L_isWhitespace(context, *index))
			continue; // Not whitespace

		// Whitespace found: Terminate search.
		L_pushString(context, strStart, length - 1);
		return;
	}
	// Something went wrong: string not found.
	L_Log(context, strStart, nullptr, "EExpected white space before 'EOF'.");
}

void BBP::Services::Lexer::L_findAllKeywords(lex_context *context)
{
	// Check context validity
	if (context == nullptr)
		return;

	// Create index
	unsigned int index = 0;

	while (context->atLocation < context->dataSize)
	{
	
		// Find the next keyword
		unsigned int kwIndex = L_findNextKeyword(&index, context);

		// If nothing found, continue
		if (kwIndex == context->keywordCount)
			continue;

		//printf("[[LEXER]]: Found keyword >'%s'< at index %u.\n", context->words[kwIndex].word, index);

		// Something was found, call action if it exists
		if (context->words[kwIndex].lex_foundAction != nullptr)
		{
			// Execute function and get error message
			const char *msg = context->words[kwIndex].lex_foundAction(context, index, kwIndex);

			// If msg is nullptr, no error. Continue.
			if (msg == nullptr)
				continue;

			L_Log(context, context->dataSize + 1, nullptr, msg);
		} //else
			//L_Log(context, index, context->words[kwIndex].word, "EUndefined action.");
	}
	
	if (context->isInComment)
		L_Log(context, context->commentStartedAt, "lexer", "EFound EOF before end of comment.");
	else if (context->isInLiteralContext)
		L_Log(context, index - 1, "lexer", "EExpected literal context termination before EOF.");

	context->isInComment = false;
	context->isInLiteralContext = false;

}

bool BBP::Services::Lexer::L_pushCharacter(lex_context *context, unsigned int index)
{
	return L_pushCharacter(context, index, 0);
}

bool BBP::Services::Lexer::L_pushCharacter(lex_context *context, unsigned int index, char replacement)
{
	// Check if context exists
	if (context == nullptr)
		return true;

	// Check if character stack is not full
	if (context->charStackIndex >= 255)
		return true;

	// Add item to stack
	context->charStack[context->charStackIndex] = context->data[index];
	context->charIndex[context->charStackIndex] = index;

	// Update data
	context->data[index] = replacement;

	// Increment stack index
	context->charStackIndex++;

	// Return success
	return false;
}

void BBP::Services::Lexer::L_popCharacter(lex_context *context)
{
	// Check if context exists
	if (context == nullptr)
		return;

	// Check if stack is empty
	if (context->charStackIndex == 0)
		return;

	// Decrement stack index
	context->charStackIndex--;

	// write character data to index
	context->data[context->charIndex[context->charStackIndex]] = context->charStack[context->charStackIndex];
}

void BBP::Services::Lexer::L_popCharacter(lex_context *context, unsigned char amount)
{
	// Check for invalid context
	if (context == nullptr)
		return;

	// If amount is zero, return.
	if (amount == 0)
		return;
	
	for (unsigned char k = 0; k < amount; k++)
		L_popCharacter(context);
}

void BBP::Services::Lexer::L_highlight(lex_context *context, unsigned int start, unsigned int end, const char *col)
{
	start--;
	char tmp = context->data[end];
	context->data[end] = 0;

	//printf("(%-4u~%4u)\t%s%s\n", start, end, col, strt);
	//printf("           \t");

	bool showLines = context->showLines;

	if (showLines)
		printf("%6u|\t%s",context->atLine + 1, col);
	else
		printf("~%5u|\t%s", start, col);

	// Loop over each character
	bool hasNewLines = false;
	unsigned int lines = 0;
	char lastC = 0;

	for (unsigned int i = start; i < end; i++)
	{
		// Fetch character
		char c = context->data[i];

		// If we are '\r' and last char is \n, just skip
		if (c == '\r')
		{
			lastC = c;
			continue;
		}

		// If we are at new line, we emit new line info and set flag to true
		if (c == '\n')
		{
			if (showLines)
				printf("\e[0;37m\n%6u|\t%s", context->atLine + (++lines) + 1, col);
			else
				printf("\e[0;37m\n~%5u|\t%s", i + 1, col);

			hasNewLines = true;
			lastC = c;
			continue;
		}

		if (context->data[i] != '\t')
			printf("%c", c);
		lastC = c;
	}

	printf("\e[0;37m\n      |\t");

	if (!hasNewLines)
	{
		printf("%s", col);
		for (unsigned int i = start; i < end; i++)
			printf("^");
	}


	context->data[end] = tmp;
	printf("\e[0;37m\n");
}

void BBP::Services::Lexer::L_Log(lex_context *context, unsigned int _where, const char *from, const char *message)
{
	if (context->showWhere)
	{
		if (_where < context->dataSize)
			if (context->activeFile == nullptr)
				printf("(<Binary>~%4u): ", _where);
			else
				printf("(%s~%4u): ", context->activeFile, _where);
		else
			if (context->activeFile == nullptr)
				printf("(<Binary>:%u:%u): ", context->atLine + 1, context->atCol);
			else
				printf("(%s:%u:%u): ", context->activeFile, context->atLine + 1, context->atCol);
	}

	switch(message[0])
	{
		case 'E': // *error*
			if (from == nullptr)
				printf("\e[0;31mError:\e[0;37m %s\n", message + 1);
			else
				printf("\e[0;31mError:\e[0;37m (%s): %s\n", from, message + 1);
		break;
		case 'W': // *Warning*
			if (from == nullptr)
				printf("\e[0;35mWarning:\e[0;37m %s\n", message + 1);
			else
				printf("\e[0;35mWarning:\e[0;37m (%s): %s\n", from, message + 1);
		break;
		case 'N': // *Note*
			if (from == nullptr)
				printf("\e[0;36mNote:\e[0;37m %s\n", message + 1);
			else
				printf("\e[0;36mNote:\e[0;37m (%s): %s\n", from, message + 1);
		break;
		case 'L': // *Literal*
			printf("\e[0;33m%s:\e[o;37m %s\n", from, message + 1);
		break;

		case 'e': // *error* with accompanying highlighting.
			if (from == nullptr)
				printf("\e[0;31mError:\e[0;37m %s\n", message + 1);
			else
				printf("\e[0;31mError:\e[0;37m (%s): %s\n", from, message + 1);
			L_highlight(context, context->highlightStart, context->highlightEnd, "\e[0;31m");
			break;

		case 'w': // *warning* with accompanying highlighting.
			if (from == nullptr)
				printf("\e[0;35mWarning:\e[0;37m %s\n", message + 1);
			else
				printf("\e[0;35mWarning:\e[0;37m (%s): %s\n", from, message + 1);
			L_highlight(context, context->highlightStart, context->highlightEnd, "\e[0;35m");
			break;

		case 'n': // *note* with accompanying highlighting.
			if (from == nullptr)
				printf("\e[0;36mNote:\e[0;37m %s\n", message + 1);
			else
				printf("\e[0;36mNote:\e[0;37m (%s): %s\n", from, message + 1);
			L_highlight(context, context->highlightStart, context->highlightEnd, "\e[0;36m");
			break;

		case 'l': // *literal* with accompanying highlighting.
			if (from == nullptr)
				printf(" %s\n", message + 1);
			else
				printf(" \e[0;33m%s:\e[o;37m %s\n", from, message + 1);

			L_highlight(context, context->highlightStart, context->highlightEnd, "\e[0;33m");
			break;


		}
}

void BBP::Services::Lexer::L_ignore(lex_context *context, char c)
{
	unsigned int mod = 0;
	for (unsigned int indx = 0; indx < context->dataSize; indx++)
	{
		char character = context->data[indx];

		if (character != c)
			context->data[indx - mod] = character;
		else
			mod++;
	}
	context->data[context->dataSize - mod] = 0;
	context->dataSize -= mod;
}

void BBP::Services::Lexer::L_emitFile(lex_context *context, const char *at)
{
	// Create file
	FILE *binaryFile = fopen(at, "wb");

	// Ignore tabs
	L_ignore(context, '\t');

	// Create buffer
	char *buffer = (char *)calloc(context->dataSize, sizeof(char));
	for (unsigned int idx = 0; idx < context->dataSize; idx++)
		buffer[idx] = context->data[idx];

	// Write data
	fwrite(buffer, context->dataSize, 1, binaryFile);

	// Close file
	fclose(binaryFile);

	// Free buffer
	free(buffer);

	printf("Data saved to \"%s\".\n", at);
}