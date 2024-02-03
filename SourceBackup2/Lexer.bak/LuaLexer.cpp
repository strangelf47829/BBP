#include "LuaLexer.h"
#include "ELFUtils.h"
#include "Lexer.h"

#define IGNORE 1

using namespace BBP::Services::Lexer;

bool isUnsafe = false;

static const unsigned int keyword_count = 7;
static const unsigned int token_count = 5;

LuaProcessor *singleton;

const char *kw_escape(lex_context *context, unsigned int index, unsigned int kw);

const char *replace_Symbol(lex_context *context, unsigned int index, unsigned int kw)
{
	unsigned int wordIndex = kw - keyword_count - token_count;

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

bool variableIsMacro = false;

const char *export_Variable(lex_context *context, unsigned int index, unsigned int firstQuot, unsigned int secondQuot)
{

	unsigned int externalNameSize = secondQuot - firstQuot - 1;
	char *externalName = (char *)calloc(externalNameSize + 1, sizeof(char));
	externalName[externalNameSize] = 0;

	for (unsigned int k = 0; k < externalNameSize; k++)
		externalName[k] = context->strRegister[k];

	L_popString(context);

	unsigned int intNameStart = 0;
	char *internalName = nullptr;
	unsigned int internalNameSize = 0;

	unsigned int intIndex = secondQuot + 1;
	for (; intIndex < context->dataSize; intIndex++)
		if (!L_isWhitespace(context, intIndex) && context->data[intIndex] != '\n')
			break;

	intNameStart = intIndex;
	L_pushNextWord(&intIndex, context);

	if (context->strRegister == nullptr)
	{
		free(externalName);
		L_popString(context);

		context->highlightStart = index;
		for (context->highlightEnd = index; context->highlightEnd < context->dataSize && context->data[context->highlightEnd] != '\n'; context->highlightEnd++);

		if (variableIsMacro)
			return "wCould not create macro: Error reading macro definition.";
		return "wCould not export symbol: Error reading symbol name.";
	}

	internalNameSize = intIndex - intNameStart;
	internalName = (char *)calloc(internalNameSize + 1, sizeof(char));
	for (unsigned int k = 0; k < internalNameSize; k++)
		internalName[k] = context->strRegister[k];
	internalName[internalNameSize] = 0;

	L_popString(context);

	if (externalNameSize > internalNameSize)
	{
		free(externalName);
		free(internalName);

		context->highlightStart = index;
		for (context->highlightEnd = index; context->highlightEnd < context->dataSize && context->data[context->highlightEnd] != '\n'; context->highlightEnd++);


		if (variableIsMacro)
			return "wCould not create macro: Macro is longer than the text it replaces.";
		return "eCould not export: External name is longer than internal name.";
	}

	if (variableIsMacro)
		P_Link(singleton, externalName, internalName, "M");
	else
		P_Link(singleton, externalName, internalName, "V");

	free(externalName);
	free(internalName);

	if (singleton->strip)
		for (unsigned int idx = index - 1; idx <= (variableIsMacro ? (intNameStart + internalNameSize - 1) : secondQuot); idx++)
			context->data[idx] = IGNORE;

	return nullptr;
}

bool isFunctionBeingExported = false;

const char *export_function(lex_context *context, unsigned int index, unsigned int firstQuot, unsigned int secondQuot)
{
	unsigned int externalNameSize = secondQuot - firstQuot - 1;
	char *externalName = (char *)calloc(externalNameSize + 1, sizeof(char));

	for (unsigned int k = 0; k < externalNameSize; k++)
		externalName[k] = context->strRegister[k];

	externalName[externalNameSize] = 0;

	L_popString(context);

	unsigned int intIndex = secondQuot + 1;
	for (; intIndex < context->dataSize; intIndex++)
		if (!L_isWhitespace(context, intIndex))
			break;
	for (; intIndex < context->dataSize; intIndex++)
		if (L_isWhitespace(context, intIndex))
			break;
	for (; intIndex < context->dataSize; intIndex++)
		if (!L_isWhitespace(context, intIndex))
			break; 

	unsigned int intNameStart = intIndex;
	L_pushNextWord(&intIndex, context);

	unsigned int internalNameSize = intIndex - intNameStart;
	char *internalName = (char *)calloc(internalNameSize, sizeof(char));

	for (unsigned int k = 0; k < internalNameSize; k++)
	{
		char c = context->strRegister[k];
		internalName[k] = c;

		if (c != ' ' && c != '(')
			continue;

		internalName[k] = 0;
		internalNameSize = k;
		break;
	}
		

	L_popString(context);

	if (externalNameSize > internalNameSize)
	{
		free(externalName);
		free(internalName);

		return "ECould not export function: External name is longer than internal name";
	}
	isFunctionBeingExported = true;
	addStringToTable(singleton->extVarTable, externalName);
	addStringToTable(singleton->intVarTable, internalName);
	addStringToTable(singleton->varTypeTable, (char *)(singleton->doLinking ? "F" : "f"));

	if (singleton->doLinking)
	{
		L_expandKeywords(context, 1);
		L_setKeywordTo(context, internalName, context->keywordCount - 1, replace_Symbol);
	}

	free(externalName);
	free(internalName);

	if (singleton->strip)
		for (unsigned int idx = index - 1; idx <= secondQuot; idx++)
			context->data[idx] = IGNORE;

	return nullptr;
}

const char *kw_export(lex_context *context, unsigned int index, unsigned int kw)
{
	// These store the indicies of the quotes in this line
	unsigned int firstQuot = 0;
	unsigned int secondQuot = 0;
	unsigned int thirdQuot = 0;
	unsigned int fourthQuot = 0;

	// Checks if there is an open bracket in this line
	bool containsOpenBracket = false;

	for (unsigned int idx = index + context->words[kw].length; idx < context->dataSize; idx++)
	{
		char c = context->data[idx];
		
		switch (c)
		{
		case '"':
			if (firstQuot == 0)
				firstQuot = idx;
			else if (secondQuot == 0)
				secondQuot = idx;
			else if (thirdQuot == 0 && idx)
				thirdQuot = idx;
			else if (fourthQuot == 0 && idx)
				fourthQuot = idx;
			else
				return "EToo many Quotation marks.";
			break;
		case '(':
			if (firstQuot == 0)
			{
				context->highlightStart = index;
				for (context->highlightEnd = idx; !L_isWhitespace(context, context->highlightEnd); context->highlightEnd++);
				return "eExpected unqualified-id before function declaration";
			}
			containsOpenBracket = true;
			break;

		case '\n':
			if (firstQuot <= secondQuot)
				goto for_break;
			context->highlightStart = index;
			context->data[idx] = ' ';
			context->highlightEnd = idx;
			return "eExpected '\"' before '\\n'.";
		case '\r':
		case '\t':
			if (secondQuot >= firstQuot)
				break;
			context->highlightStart = index;
			context->data[idx] = ' ';
			context->highlightEnd = idx;
			return "eExpected '\"' before 'illegal token'.";
		case ' ':
			if (firstQuot <= secondQuot)
				break;
			context->highlightStart = index;
			context->highlightEnd = idx;
			return "eIllegal ' ' in export statement.";
		case '\\':
			if (kw)
			{
				const char *msg = kw_escape(context, idx + 1, keyword_count + token_count - 2);
				idx++;
				idx++;
				if (msg != nullptr)
					return msg;
			}
		case '\'':
		case '.':
			context->highlightStart = index;
			context->highlightEnd = idx + 1;
			if (firstQuot != 0 && !kw)
				return "eUnexpected token inside unqualified-id.";

			break;
		default:
			if (firstQuot == 0)
			{
				context->highlightStart = index;
				for (context->highlightEnd = idx; !L_isWhitespace(context, context->highlightEnd); context->highlightEnd++);
				return "eExpected unqualified-id.";
			}
			break;
		}
	}
for_break:

	if (fourthQuot)
		secondQuot = fourthQuot;
	
	if (secondQuot == firstQuot + 1)
	{
		context->highlightStart = index;
		context->highlightEnd = secondQuot + 1;
		return "wEmpty unqualified-id after export statement.";
	}

	L_pushString(context, firstQuot + 1, secondQuot - firstQuot - 1);
	
	if (kw == 0)
	{ // KW is 'export'
		variableIsMacro = false;
		if (containsOpenBracket)
			return export_function(context, index, firstQuot, secondQuot);
		else
			return export_Variable(context, index, firstQuot, secondQuot);
	}
	if (kw == 1)
	{
		// KW is 'macro'
		variableIsMacro = true;

		if (thirdQuot && fourthQuot == 0)
			return "EExpected '\"' before end of line";

		return export_Variable(context, index, firstQuot, secondQuot);
	}


	return nullptr;
}

bool functionDeclInStack = false;
unsigned int functionDeclIdx = 0;
unsigned int stack = 0;
unsigned int funcDeclAt = 0;

const char *kw_deepenStack(lex_context *context, unsigned int index, unsigned int kw)
{
	stack++;
	return nullptr;
}

const char *kw_shortenStack(lex_context *context, unsigned int index, unsigned int kw)
{
	if (stack == 0)
		return "EStack is empty";
	stack--;

	if ((stack == functionDeclIdx) && functionDeclInStack)
	{
		unsigned int len = index - funcDeclAt + 2;

		L_pushString(context, funcDeclAt, len);

		addStringToTable(singleton->funcBodyTables, context->strRegister, IGNORE);
		L_popString(context);

		unsigned int intIndex = funcDeclAt;
		for (; intIndex < context->dataSize; intIndex++)
			if (L_isWhitespace(context, intIndex))
				break;
		for (; intIndex < context->dataSize; intIndex++)
			if (!L_isWhitespace(context, intIndex))
				break;

		unsigned int funcSig = intIndex;
		L_pushNextWord(&intIndex, context);
		
		for (; funcSig < context->dataSize; funcSig++)
		{
			if (context->data[funcSig] != '(')
				continue;

			L_pushCharacter(context, funcSig, 0);
			break;
		}

		addStringToTable(singleton->funcSigTables, context->strRegister, IGNORE);
		L_popCharacter(context);
		L_popString(context);

		functionDeclInStack = false;

		
		for (unsigned int n = 0; n < len; n++)
			context->data[n + funcDeclAt] = IGNORE;

	}

	return nullptr;
}

const char *kw_funcDecl(lex_context *context, unsigned int index, unsigned int kw)
{
	if (functionDeclInStack)
		return "EA function is already being declared.";

	functionDeclInStack = true;
	functionDeclIdx = stack;
	funcDeclAt = index - 1;

	// We don't really care about the function signature, since it isn't really going anywhere. All we care about is the type
	if (!isFunctionBeingExported)
	{
		addStringToTable(singleton->extVarTable, (char *)"<func>");
		addStringToTable(singleton->intVarTable, (char *)"<func>");
		addStringToTable(singleton->varTypeTable, (char *)"F");
	}

	isFunctionBeingExported = false;

	kw_deepenStack(context, index, kw);
	return nullptr;
}


const char *kw_guard(lex_context *context, unsigned int index, unsigned int kw)
{
	kw_deepenStack(context, index, kw);

	if (!singleton->emitCallsToSTDLib && !isUnsafe)
		return "WGuard not implemented: calls to std lib disabled.";

	if (!isUnsafe)
		L_insertNewline(context, index, "app.watch()\n");
	isUnsafe = false;
	return nullptr;
}


const char *kw_unsafe(lex_context *context, unsigned int index, unsigned int kw)
{
	if (isUnsafe)
		return "EUnresolved 'unsafe'.";

	if (singleton->strip)
		for (unsigned int i = 0; i < context->words[kw].length; i++)
			context->data[i + index - 1] = IGNORE;

	isUnsafe = true;
	
	return "WBlock marked 'unsafe'.";
}

const char *tk_startString(lex_context *context, unsigned int index, unsigned int kw)
{
	if (!context->isInLiteralContext)
	{
		context->isInLiteralContext = true;
		context->escapeIndex = kw;
		context->enforceIndicies = kw - 1;
	}
	else
	{
		context->isInLiteralContext = false;
		context->enforceIndicies = kw - 1;
	}

	return nullptr;
}

const char *kw_escape(lex_context *context, unsigned int index, unsigned int kw)
{
	char type = context->data[index];

	switch (type)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '8':
	case '9':
	case 'x':
		// Increment by 2, since numbers follow after
		context->atLocation += 2;
	case 'a':
	case 'b':
	case 'f':
	case 'n':
	case 'r':
	case 't':
	case 'v':
	case '\\':
	case '"':
	case '\'':
		// Vanilla LUA escape, skip over character
		context->atLocation++;

		printf("%c\n", type);

		// If context is in a literal context, return with no error
		if (context->isInLiteralContext && type != '\\')
			return nullptr;

		// Highlighting
		context->highlightStart = index;
		context->data[index - 1] = IGNORE;
		for (context->highlightEnd = index; !L_isWhitespace(context, context->highlightEnd); context->highlightEnd++)
			context->data[context->highlightEnd] = IGNORE;

		return "wEscape sequence found outside of string, which is forbidden: Escape sequence removed.";


	case 'e':
		// Remove the escape sequence
		context->data[index - 1] = IGNORE;
		context->data[index] = IGNORE;

		// Toggle context
		context->atLocation++;
		return tk_startString(context, index, keyword_count + token_count - 1);

	case '\n':
	case '\r':
		// Just remove the 
		context->data[index - 1] = IGNORE;
		return nullptr;

	default:
		context->highlightStart = index;
		context->highlightEnd = index + 1;
		printf("0x%02x\n", type);
		return "nUnrecognized escape sequence:";

	}
	return nullptr;
}

const char *tk_treatAsWhitespace(lex_context *context, unsigned int index, unsigned int kw)
{
	context->lastWhiteSpace = index - 1;
	return nullptr;
}

const char *tk_comment(lex_context *context, unsigned int index, unsigned int kw)
{
	// Skip over first '-'
	index++;

	bool isMultilineComment = false;
	bool isDefiningMultilineComment = false;
	bool isDefiningEndOfMultilineComment = false;
	unsigned int depth = 0;

	if (index + 1 < context->dataSize)
	{
		isDefiningMultilineComment = (context->data[index] == '[');
		isDefiningEndOfMultilineComment = (context->data[index] == ']');
	}

	isMultilineComment = isDefiningEndOfMultilineComment || isDefiningMultilineComment;

	// Since we only care if the character immediately after '-' is '[', checking it again is redundant and also complicates the logic.
	index++;

	if (!isMultilineComment || (!context->isInComment && isDefiningEndOfMultilineComment))
	{
		// We might as well just remove the comments here, instead of using the lexer (which would be complicated anyway)
		index -= 3;	// < Move to first '-'
		context->data[index++] = IGNORE; // < ignore the first --
		context->data[index++] = IGNORE; // ^^^^^^^^^^^^^^^^^^^^^^

		// Now just remove everything until new line or EOF
		for (; index < context->dataSize && context->data[index] != '\n'; index++)
			context->data[index] = IGNORE;

		// Set the parser location to index
		context->atLocation = index - 1;

		return nullptr;
	}

	// Loop over each character
	for (unsigned int idx = index;; idx++)
	{
		// If end of file reached, stop or throw error, depending on if comment is multiline or not.
		if (idx >= context->dataSize)
		{
			if (!isMultilineComment)
				return nullptr;

			// Multiline comment, throw error
			context->highlightStart = index;
			context->highlightEnd = idx;
			return "eExpected comment termination before 'EOF'.";
		}

		// Get current character
		char c = context->data[idx];

		switch (c)
		{
		case '=': // '=' defines the depth of the comment, so only pay it any attention if it is in a comment definition
			depth++;
			break;
		case '[': // '[' Indicates end of comment definition, so terminate comment search if this is found.
			if (isDefiningEndOfMultilineComment)
				goto default_case;
			
			if (context->isInComment) // If we are already in a comment, do not override 'depth', as that would be pointless.
			{
				if (depth != context->commentDepth)
					return nullptr;

				context->highlightStart = context->commentStartedAt + 1;
				context->highlightEnd = idx + 1;
				return "wFound two multiline comment definitions with the same depth, consider changing the depth of one of the comments.";
			}

			// We are defining a comment
			context->isInComment = true;
			context->commentDepth = depth;
			context->commentStartedAt = index - 3;

			// Set some flags for the lexer
			context->isInLiteralContext = true;
			context->escapeIndex = kw;
			context->enforceIndicies = kw;
			return nullptr;

		case ']': // ']' Indicates end of 'end of' comment definition, so terminate comment search if this is found.
			if (isDefiningMultilineComment)
				goto default_case;

			// If the comment depth equals to the stored comment depth, exit out of comment
			if (context->commentDepth != depth)
				return nullptr;

			// Remove everything since comment
			for (unsigned int indx2 = context->commentStartedAt; indx2 < idx + 1; indx2++)
				context->data[indx2] = IGNORE;
			
			// Reset flags
			context->isInComment = false;
			context->commentDepth = 0;

			// Reset lexer flags
			context->isInLiteralContext = false;
			context->escapeIndex = 0;
			context->enforceIndicies = 0;
			return nullptr;

		default:
		default_case:
			// Something has been found which was unexpected, throw error
			context->highlightStart = index - 2;
			context->highlightEnd = idx + 1;

			if (isDefiningEndOfMultilineComment)
				return "eExpected `]` Before unexpected token.";
			return "eExpected `[` Before unexpected token.";
		}

	}


}

static const char *const keywords[] = { "symbol", "macro", "function", "end", "unsafe", "do", "then"};
static const char *const tokens[] = { ".", ":", "(", "--", "\\", "\""};

static const lex_action keywordActions[] = { kw_export, kw_export, kw_funcDecl, kw_shortenStack, kw_unsafe, kw_guard, kw_guard };
static const lex_action tokenActions[] = { tk_treatAsWhitespace, tk_treatAsWhitespace, tk_treatAsWhitespace, tk_comment, kw_escape, tk_startString };

void BBP::Services::Lexer::P_doFile(LuaProcessor *proc, const char *at, bool link)
{
	L_createLexer(&proc->context, at);

	singleton = proc;
	proc->doLinking = link;

	P_fetchKeywords(proc);
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

	proc->context->tokenIndex = keyword_count;
}

void BBP::Services::Lexer::P_destroyLua(LuaProcessor *proc)
{
	// Free interpreter data
	free(proc->interp);

	L_destroyLexer(proc->context);
	destroyELF(proc->elf);
}
