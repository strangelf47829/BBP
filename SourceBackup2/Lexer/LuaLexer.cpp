#include "LuaLexer.h"
#include "ELFUtils.h"
#include "Lexer.h"
#include "LuaFlags.h"
#include "Lpp.h"


using namespace BBP::Services::Lexer;


static unsigned int keyword_count = 15;
static unsigned int token_count = 9;
static const char *const keywords[] = { "symbol", "function", "include", "end", "define", "while", "for", "auto", "file", "do", "then", "unsafe", "ifndef", "ifdef", "stop" };
static const char *const tokens[] = { "\n", ".", ":", "(", "--", ";", "=", "\\", "\"" };


#define KW_SYMBOL 0
#define KW_FUNCTION 1
#define KW_IMPORT 2
#define KW_END 3
#define KW_DEFINE 4
#define KW_WHILE 5
#define KW_FOR 6
#define KW_AUTO 7
#define KW_FILE 8
#define KW_DO 9
#define KW_THEN 10
#define KW_UNSAFE 11
#define KW_IFNDEF 12
#define KW_IFDEF 13
#define KW_STOP 14

#define TK_NEWLINE 15
#define TK_DOT 16
#define TK_COLON 17
#define TK_PARENTHESES 18
#define TK_COMMENT 19
#define TK_SEMICOLON 20
#define TK_ASSIGNMENT 21
#define TK_BACKLASH 22
#define TK_QUOT 23

#define KW_NONE 24



LuaProcessor *singleton;

void debugSymb(lex_context *context, char *identifier, char *exportedName)
{
	printf("(<%s>): Exporting ", singleton->activeFile);
	if (context->flags[FLAG_SYMBOLINCOMPLETE])
		printf("Incomplete ");
	else if (context->flags[FLAG_SYMBOLHASVALUE] || context->flags[FLAG_SYMBOLFUNCTION])
		printf("Complete ");
	else
		printf("Unkown ");

	printf("symbol \"%s\" which is of type ", identifier);

	if (context->flags[FLAG_SYMBOLFUNCTION])
		printf("<function> ");
	else
		printf("<symbol> ");

	if (context->flags[FLAG_SYMBOLFUNCTION] && context->flags[FLAG_FUNCPARAMSAVAIL])
	{
		printf("(%s) ", context->strRegister);
	}

	if (context->flags[FLAG_EXTDEFINEDNAME])
		printf("as \"%s\".\n", exportedName);
	else if (context->flags[FLAG_SYMBOLFILEMANGLE])
		printf("as <file>.\n");
	else
		printf("as <auto>.\n");
}


const char *tk_quote(lex_context *context, unsigned int index, unsigned int kw);

const char *replace_Symbol(lex_context *context, unsigned int index, unsigned int kw)
{
	unsigned int wordIndex = kw - 10 - 6;

	char *externalWord = singleton->extVarTable->strings[wordIndex];
	char *internalWord = singleton->intVarTable->strings[wordIndex];

	unsigned int externalLength = singleton->extVarTable->lengths[wordIndex];
	unsigned int internalLength = singleton->intVarTable->lengths[wordIndex];

	unsigned int pad = --index + (internalLength - externalLength);

	for (unsigned int idx = index; idx < index + internalLength - 1; idx++)
	{
		context->data[idx] = (idx < pad) ? IGNORE : externalWord[idx - pad];
	}

	return nullptr;
}

const char *kw_symbol(lex_context *context, unsigned int index, unsigned int kw) 
{
	// Reset flags
	for (unsigned int k = FLAG_SYMBOLDEFINITION; k < FLAG_EXTDEFINEDNAME + 1; k++)
		context->flags[k] = false;


	// Start defining symbol
	context->flags[FLAG_SYMBOLDEFINITION] = true;
	context->careAboutNewLine = true;

	// Keep track of symbol name
	unsigned int exportedLength;
	char *exportedName;

	// Keep track of identifier
	unsigned int identifierLength = 0;
	char *identifier = nullptr;

	// Move to end of word, then look for symbol
	context->careAboutNewLine = true;
	L_moveToEndOfWord(context);
	int nextKeyword = L_findNextKeyword(&context->atLocation, context);

	// End of 'auto' or '".."'.
	unsigned int endOfMangle = 0;

	switch (nextKeyword)
	{
	case KW_AUTO: // 'Auto' -> set auto name mangling to true
		context->flags[FLAG_SYMBOLAUTOMANGLE] = true;
		L_moveToEndOfWord(context);
		endOfMangle = context->atLocation;
		break;
	case KW_FILE: // 'File' -> set file name mangling to true
		context->flags[FLAG_SYMBOLFILEMANGLE] = true;
		L_moveToEndOfWord(context);
		endOfMangle = context->atLocation;
		break;
	case TK_QUOT: // " -> Do " action, then search for pair.
	{
		// Remember start of quote
		unsigned int quotStart = context->atLocation;
		
		// Do " action
		context->flags[FLAG_STOPQUOTEATNEWLINE] = true;
		tk_quote(context, context->atLocation, TK_QUOT);

		// Increase atLocation by one
		context->atLocation++;

		// Find next KW
		unsigned int pairedKW = L_findNextKeyword(&context->atLocation, context);

		// Do " action again (even if not actually quote, to prevent issues with literal context nonesense.
		tk_quote(context, context->atLocation, TK_QUOT);

		// Exception handling
		if (pairedKW == TK_NEWLINE)
			return "EExpected matching \" before end-of-line in symbol declaration.";
		else if (pairedKW == KW_NONE)
			return "EExpected matching \" before end-of-file in symbol declaration.";
		else if (pairedKW != TK_QUOT)
			return "EExpected matching \" before token.";

		// Set flag
		context->flags[FLAG_EXTDEFINEDNAME] = true;

		// Push symbol name
		L_pushString(context, quotStart + 1, context->atLocation - quotStart - 1);
		exportedLength = context->atLocation - quotStart - 1;

		// Since context is now on '"', atLocation needs to be incremented again
		L_moveToEndOfWord(context);
		endOfMangle = context->atLocation;
		break;
	}
	case TK_NEWLINE:
		return "EExpected symbol declaration before end-of-line.";
	case KW_NONE:
		return "EExpected symbol declaration before end-of-file.";
	default:
		return "EExpected symbol export mode before token.";
	}
	
	// If externally defined, throw error if length is 0
	if (context->flags[FLAG_EXTDEFINEDNAME] && exportedLength == 0)
	{
		L_popString(context);
		return "EExported symbol name must have a length greater than 0.";
	}

	// If externally defined, check for white spaces
	if (context->flags[FLAG_EXTDEFINEDNAME])
	{
		for (unsigned int k = 0; k < exportedLength; k++)
		{
			if (!L_isWhitespace(context->strRegister[k]))
				continue;
			L_popString(context);
			return "EExported symbol name cannot have whitespaces.";
		}
	}

	// Copy over symbol data
	if (context->flags[FLAG_EXTDEFINEDNAME])
	{
		exportedName = (char *)calloc(exportedLength + 1, sizeof(char));
		for (unsigned int k = 0; k < exportedLength; k++)
			exportedName[k] = context->strRegister[k];
		exportedName[exportedLength] = 0;
		L_popString(context);
	}

search_again_for_identifier:

	// Find next keyword
	context->careAboutNewLine = true;
	nextKeyword = L_findNextKeyword(&context->atLocation, context);

	switch (nextKeyword)
	{
	case KW_NONE:
	case TK_NEWLINE: // Throw error based on if qualified ID exists (and also based on if thingy is function
	{
		if (context->flags[FLAG_EXTDEFINEDNAME])
			free(exportedName);
		unsigned int identStart = endOfMangle;
		for (; identStart < context->dataSize; identStart++)
		{
			// If \n is encountered, return with error
			if (context->data[identStart] == '\n' || identStart + 1 >= context->dataSize)
			{
				if (context->flags[FLAG_EXTDEFINEDNAME])
					free(exportedName);
				if (nextKeyword == TK_NEWLINE)
					return "EExpected unqualified-id before end-of-line.";
				return "EExpected unqualified-id before end-of-file.";
			}

			// Break out of for loop if character is not '\n' or whitespace
			if (!L_isWhitespace(context->data[identStart]))
				break;
		}

		// If symbol is not a function, throw an error
		if (!context->flags[FLAG_SYMBOLFUNCTION])
			return "ENon-function symbol does not name complete nor incomplete id. Did you forget a ';'?";
		return "EFunction symbol does not name complete nor incomplete id. Did you forget a ';' or '('?";
	}
		

	case TK_SEMICOLON: // Check if something was even defined
	{
		unsigned int identStart = endOfMangle;
		for (; identStart < context->dataSize; identStart++)
		{
			// If ; is encountered, return with error
			if (context->data[identStart] == ';')
			{
				if (context->flags[FLAG_EXTDEFINEDNAME])
					free(exportedName);
				return "EExpected unqualified-id before ';'.";
			}

			// Break out of for loop if character is not ';' or whitespace
			if (!L_isWhitespace(context->data[identStart]))
				break;
		}

		// Push identifier
		L_pushNextWord(&identStart, context);

		// Calculate identifier size
		for (identifierLength = 0;;)
		{
			// Get character
			char c = context->strRegister[identifierLength];
			
			// Break if characters are 0 or ;
			if (c == ';' || c == 0 || L_isWhitespace(c))
				break;

			// Increment
			identifierLength++;
		}

		// Calloc data and copy data
		identifier = (char *)calloc(identifierLength + 1, sizeof(char));
		for (unsigned int k = 0; k < identifierLength; k++)
			identifier[k] = context->strRegister[k];
		identifier[identifierLength] = 0;

		// Pop string
		L_popString(context);

		// Set flags
		context->flags[FLAG_SYMBOLINCOMPLETE] = true;
		context->atLocation++;
		break;
	}
	case TK_ASSIGNMENT: // Check if something was even defined
	{
		unsigned int identStart = endOfMangle;
		for (; identStart < context->dataSize; identStart++)
		{
			// If = is encountered, return with error
			if (context->data[identStart] == '=')
			{
				if (context->flags[FLAG_EXTDEFINEDNAME])
					free(exportedName);
				return "EExpected unqualified-id before '='.";
			}

			// Break out of for loop if character is not '=' or whitespace
			if (!L_isWhitespace(context->data[identStart]))
				break;
		}

		// Push identifier
		L_pushNextWord(&identStart, context);

		// Calculate identifier size
		for (identifierLength = 0;;)
		{
			// Get character
			char c = context->strRegister[identifierLength];

			// Break if characters are 0 or ;
			if (c == '=' || c == 0 || L_isWhitespace(c))
				break;
			
			// Increment
			identifierLength++;
		}

		// Calloc data and copy data
		identifier = (char *)calloc(identifierLength + 1, sizeof(char));
		for (unsigned int k = 0; k < identifierLength; k++)
			identifier[k] = context->strRegister[k];
		identifier[identifierLength] = 0;

		// Pop string
		L_popString(context);

		// Set flags
		context->flags[FLAG_SYMBOLHASVALUE] = true;
		context->atLocation++;
		break;
	}
	case TK_PARENTHESES: // Check if something was even defined
	{
		// If we are not declaring a function, throw error
		if (!context->flags[FLAG_SYMBOLFUNCTION])
		{
			if (context->flags[FLAG_EXTDEFINEDNAME])
				free(exportedName);
			return "EInitializing variable like a function.";
		}

		unsigned int identStart = endOfMangle;
		for (; identStart < context->dataSize; identStart++)
		{
			// If ; is encountered, return with error
			if (context->data[identStart] == '(')
			{
				if (context->flags[FLAG_EXTDEFINEDNAME])
					free(exportedName);
				return "EExpected unqualified-id before '('.";
			}

			// Break out of for loop if character is not ';' or whitespace
			if (!L_isWhitespace(context->data[identStart]))
				break;
		}

		// Push identifier
		L_pushNextWord(&identStart, context);

		// Calculate identifier size
		for (identifierLength = 0;;)
		{
			// Get character
			char c = context->strRegister[identifierLength];

			// Break if characters are 0 or ;
			if (c == '(' || c == 0 || L_isWhitespace(c))
				break;
			// Increment
			identifierLength++;
		}

		// Calloc data and copy data
		identifier = (char *)calloc(identifierLength + 1, sizeof(char));
		for (unsigned int k = 0; k < identifierLength; k++)
			identifier[k] = context->strRegister[k];
		identifier[identifierLength] = 0;

		// Pop string
		L_popString(context);

		// Move to ')', while keeping track where the '(' and ')' is.
		unsigned int opensat = context->atLocation;
		for (; context->atLocation < context->dataSize && context->data[context->atLocation] != ')';) 
		{
			if (context->data[context->atLocation] == '\n')
			{
				if (context->flags[FLAG_EXTDEFINEDNAME])
					free(exportedName);
				if (identifier != nullptr)
					free(identifier);
				return "EMissing ')' before end-of-line in function symbol declaration.";
			}
			if (context->atLocation + 1 >= context->dataSize)
			{
				if (context->flags[FLAG_EXTDEFINEDNAME])
					free(exportedName);
				if (identifier != nullptr)
					free(identifier);
				return "EMissing ')' before end-of-file in function symbol declaration.";
			}
			context->atLocation++;
		}

		// Push string onto string stack, then function signature stack
		L_pushString(context, opensat - identifierLength, context->atLocation - opensat + identifierLength);
		
		// Check if there is a semicolon before newline
		for (; context->atLocation < context->dataSize && context->data[context->atLocation] != '\n'; context->atLocation++)
		{
			// If character is ';', set flag then break
			if (context->data[context->atLocation] != ';')
				continue;

			context->flags[FLAG_SYMBOLINCOMPLETE] = true;
			context->atLocation++;
			break;
		}
		
		// set flag
		context->flags[FLAG_FUNCPARAMSAVAIL] = true;
		break;
	}
	case KW_FUNCTION: // Set 'function' to true
		context->flags[FLAG_SYMBOLFUNCTION] = true;
		L_moveToEndOfWord(context);
		endOfMangle = context->atLocation;

		// Search again for thingy
		goto search_again_for_identifier;
	default:
		printf("KW is %u\n", nextKeyword);
		break;
	}


	// A function with value makes no sense
	if (context->flags[FLAG_SYMBOLFUNCTION] && context->flags[FLAG_SYMBOLHASVALUE])
	{
		if (context->flags[FLAG_EXTDEFINEDNAME])
			free(exportedName);
		if (identifier != nullptr)
			free(identifier);
		if (context->flags[FLAG_FUNCPARAMSAVAIL])
			L_popString(context);

		context->highlightStart = index;
		context->highlightEnd = context->atLocation;

		return "eFunction initialized like a variable.";
	}

	// Figure out what type this symbol is
	char *type = (char *)BBP::Services::c_alloc(2, sizeof(char));
	if (context->flags[FLAG_SYMBOLFUNCTION])
		type[0] = (context->flags[FLAG_SYMBOLINCOMPLETE] ? 'f' : 'F');
	else
		type[0] = (context->flags[FLAG_SYMBOLINCOMPLETE] ? 'v' : 'V');

	// Add type to type table and free buffer
	BBP::IO::Buffers::addStringToTable(singleton->varTypeTable, type);
	BBP::Services::Free(type);

	// Add the internal identifier to the internal name table
	BBP::IO::Buffers::addStringToTable(singleton->intVarTable, identifier);

	// If the symbol is an undefined function, and has available parameters, add the signature and pop the string
	if (context->flags[FLAG_SYMBOLINCOMPLETE] && context->flags[FLAG_SYMBOLFUNCTION] && context->flags[FLAG_FUNCPARAMSAVAIL])
	{
		BBP::IO::Buffers::addStringToTable(singleton->funcSigTables, context->strRegister);
		//BBP::IO::Buffers::addStringToTable(singleton->funcBodyTables, "<body if>");
	}
	else if (context->flags[FLAG_SYMBOLINCOMPLETE] && context->flags[FLAG_SYMBOLFUNCTION])
	{
		BBP::IO::Buffers::addStringToTable(singleton->funcSigTables, identifier);
	}
	else if (context->flags[FLAG_SYMBOLFUNCTION])
	{
		// THIS IS JUST ADDED TO PREVENT SEGFAULT.. THIS SHOULD ACTUALLY BE ADDED BY THE FUNCTION DECLARATION FUNCTION
		BBP::IO::Buffers::addStringToTable(singleton->funcSigTables, identifier);
		BBP::IO::Buffers::addStringToTable(singleton->funcBodyTables, "<Defined function>");
	}

	// Pop string if params available
	if (context->flags[FLAG_FUNCPARAMSAVAIL])
		L_popString(context);

	// Now export the function with the mangled name.
	BBP::Services::Binaries::LPP::exportName(singleton);

	// If the symbol is incomplete, erase the entire line
	if (context->flags[FLAG_SYMBOLINCOMPLETE])
	{
		for (unsigned int k = index; k < context->dataSize && context->data[k] != '\n'; k++)
			context->data[k] = IGNORE;
	}
	else
	{
		// Otherwise, just erase the 'symbol' and name mangling if not function.
		if (context->flags[FLAG_SYMBOLFUNCTION])
		{
			// If function, move back one word
			for (; L_isWhitespace(context->data[--endOfMangle]);); // Stops at first non-whitespace char (end of 'function')
			endOfMangle -= context->words[KW_FUNCTION].length; // Moves back the length of 'function'.

			// Since we want to actually parse the function def, move position here
			context->atLocation = endOfMangle;
		}
		for (unsigned int k = index; k < endOfMangle + 1; k++)
			context->data[k] = IGNORE;
	}



	if (context->flags[FLAG_EXTDEFINEDNAME])
		free(exportedName);
	if (identifier != nullptr)
		free(identifier);

	return nullptr;
}
const char *kw_function(lex_context *context, unsigned int index, unsigned int kw)
{
	
	return "NFunction declaration";
}

const char *kw_import(lex_context *context, unsigned int index, unsigned int kw) 
{
	// Find out if it is in the style of <> or ""
	L_moveToEndOfWord(context);

	for (; context->atLocation < context->dataSize; context->atLocation++)
	{
		// Get character
		char c = context->data[context->atLocation];

		// If new line, throw error
		if (c == '\n' || c == '\r')
			return "EExpected '\"' or '<' in include statement before end-of-line.";

		// If end-of-file throw error
		if (context->atLocation + 1 >= context->dataSize)
			return "EExpected '\"' or '<' in include statement before end-of-file.";

		// If whitespace, continue
		if (L_isWhitespace(c))
			continue;

		// If not equal to < or ", throw error
		if (c == '<')
			context->flags[FLAG_INCLUDERELFILE] = false;
		else if (c == '"')
			context->flags[FLAG_INCLUDERELFILE] = true;
		else
			return "EExpected '\"' or '<' in include statement before unrecognized character.";
		break;
	}
	unsigned int startOfFile = ++context->atLocation;
	unsigned int endOfFile = context->atLocation;

	// Find end of > or " statement
	for (; context->atLocation < context->dataSize; context->atLocation++)
	{
		// Get character
		char c = context->data[context->atLocation];

		// If new line, throw error
		if (c == '\n' || c == '\r')
		{
			if (context->flags[FLAG_INCLUDERELFILE])
				return "EExpected matching '\"' in include statement before end-of-line.";
			return "EExpected matching '>' in include statement before end-of-line.";
		}
			

		// If end-of-file throw error
		if (context->atLocation + 1 >= context->dataSize)
		{
			if (context->flags[FLAG_INCLUDERELFILE])
				return "EExpected matching '\"' in include statement before end-of-file.";
			return "EExpected matching '>' in include statement before end-of-file.";
		}

		// If whitespace or not > or ", continue
		if (L_isWhitespace(c) || (c != '>' && c != '"'))
			continue;

		// Only match to corresponding start
		if (c == '>')
		{
			if (context->flags[FLAG_INCLUDERELFILE])
				return "EExpected matching '\"' in include statement instead of '>'.";
			endOfFile = context->atLocation;
			break;
		}

		if (!context->flags[FLAG_INCLUDERELFILE])
			return "EExpected matching '>' in include statement instead of '\"'.";
		endOfFile = context->atLocation;
		break;
	}

	// Check if include statement is false
	if (endOfFile - startOfFile == 0)
		return "EInclude statement cannot be empty.";

	// Push the name of the file
	L_pushString(context, startOfFile, endOfFile - startOfFile);

	if (context->flags[FLAG_INCLUDERELFILE])
		printf("Including file \" %s \"\n",context->strRegister);
	else
		printf("Including file < %s >\n", context->strRegister);

	// Read the file
	BBP::IO::File::FILE_HANDLE handle = BBP::IO::SDMMC::readFile(context->strRegister);
	L_popString(context);

	// Check if file is ELF or not
	bool isELF = BBP::Services::Binaries::ELF::isELF(handle.fileVector);

	if (isELF && context->flags[FLAG_INCLUDERELFILE])
	{
		free(handle.fileVector);
		return "ECannot import file because file is in ELF format. Please use <> instead of \"\".";
	}
	else if (!isELF && !context->flags[FLAG_INCLUDERELFILE])
	{
		free(handle.fileVector);
		return "ECannot import file because file is not in ELF format. Please use \"\" instead of <>.";
	}

	// Remove include statement
	for (unsigned int k = index; k < context->dataSize; k++)
	{
		char c = context->data[k];

		if (c == '\n')
			break;

		context->data[k] = IGNORE;
	}
	
	BBP::Services::Lexer::L_insert(context, index, (char *)handle.fileVector);
	
	free(handle.fileVector);

	// Who knows where we ended up, so set position to index.
	context->atLocation = index;

	return nullptr;
}
const char *kw_end(lex_context *context, unsigned int index, unsigned int kw) {}
const char *kw_define(lex_context *context, unsigned int index, unsigned int kw) {}
const char *kw_while(lex_context *context, unsigned int index, unsigned int kw) {}
const char *kw_for(lex_context *context, unsigned int index, unsigned int kw) {}
const char *kw_auto(lex_context *context, unsigned int index, unsigned int kw)
{
	return "EIsolated Auto.";
}
const char *kw_file(lex_context *context, unsigned int index, unsigned int kw)
{
	return "EIsolated File.";
}
const char *kw_do(lex_context *context, unsigned int index, unsigned int kw) {}
const char *kw_then(lex_context *context, unsigned int index, unsigned int kw) {}
const char *kw_unsafe(lex_context *context, unsigned int index, unsigned int kw) 
{
	return "WBlock marked unsafe";
}
const char *kw_ifndef(lex_context *context, unsigned int index, unsigned int kw) 
{
	return "NStarting IFNDEF";
}
const char *kw_ifdef(lex_context *context, unsigned int index, unsigned int kw)
{
	return "NStarting IFDEF";
}
const char *kw_stop(lex_context *context, unsigned int index, unsigned int kw)
{
	return "Nendif IFNDEF/IFDEF";
}


const char *tk_dot(lex_context *context, unsigned int index, unsigned int kw) {}
const char *tk_colon(lex_context *context, unsigned int index, unsigned int kw) {}
const char *tk_parentheses(lex_context *context, unsigned int index, unsigned int kw) 
{
	return nullptr;
}
const char *tk_comment(lex_context *context, unsigned int index, unsigned int kw) {}

const char *tk_newline(lex_context *context, unsigned int index, unsigned int kw) 
{
	context->careAboutNewLine = false;
	if (context->isInLiteralContext)
		return "EExpected '\"' before new line.";
	return nullptr;
}

const char *tk_semicolon(lex_context *context, unsigned int index, unsigned int kw) 
{
	return "WSemi colon serves no purpose here. Please remove.";
}
const char *tk_assignment(lex_context *context, unsigned int index, unsigned int kw)
{
	return nullptr;
}
const char *tk_backslash(lex_context *context, unsigned int index, unsigned int kw) {}
const char *tk_quote(lex_context *context, unsigned int index, unsigned int kw) 
{
	if (context->isInLiteralContext)
	{
		// Reset flag
		context->isInLiteralContext = false;
		
		// Reset escape indicies
		context->escapeIndex = 0;
		context->enforceIndicies = 0;


		return nullptr;
	}
	else
	{
		// Set 'in literal context' to true
		context->isInLiteralContext = true;

		// Set escape indicies
		context->escapeIndex = kw;
		context->enforceIndicies = kw;
		
		// If we do not want to encounter a newline before reaching ", set flag to true
		if (context->flags[FLAG_STOPQUOTEATNEWLINE])
			context->careAboutNewLine = true;

		
		return nullptr;
	}
}

static const lex_action keywordActions[] = { kw_symbol, kw_function, kw_import, kw_end, kw_define, kw_while, kw_for, kw_auto, kw_file, kw_do, kw_then, kw_unsafe, kw_ifndef, kw_ifdef, kw_stop };
static const lex_action tokenActions[] = { tk_newline, tk_dot, tk_colon, tk_parentheses, tk_comment, tk_semicolon, tk_assignment, tk_backslash, tk_quote };


void BBP::Services::Lexer::P_doFile(LuaProcessor *proc, const char *at, bool link)
{
	L_createLexer(&proc->context, at);

	singleton = proc;
	proc->doLinking = link;

	P_fetchKeywords(proc);
	proc->context->newline = TK_NEWLINE;
}

void BBP::Services::Lexer::P_fetchKeywords(LuaProcessor *proc)
{
	if (proc->addedKeywords)
		return;
	proc->addedKeywords = true;

	L_setKeywords(proc->context, keyword_count + token_count);

	for (unsigned int k = 0; k < keyword_count; k++)
		L_setKeywordTo(proc->context, keywords[k], k, keywordActions[k]);

	for (unsigned int k = 0; k < token_count; k++)
		L_setKeywordTo(proc->context, tokens[k], keyword_count + k, tokenActions[k]);

	proc->context->newline = TK_NEWLINE;

	proc->context->tokenIndex = keyword_count;
}

void BBP::Services::Lexer::P_destroyLua(LuaProcessor *proc)
{
	// Free interpreter data
	free(proc->interp);

	L_destroyLexer(proc->context);
	destroyELF(proc->elf);
}
