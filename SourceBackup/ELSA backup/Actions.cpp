#include "../include/Lex.h"
#include "Actions.h"
#include "../include/ELSA.h"

BBP::std::STATIC_PAGE<BBP::std::hash_t, 256> definePage;
BBP::std::Stack<BBP::std::hash_t> defines(&definePage, 256);

// You cannot just dump a bunch of elses after eachother
BBP::std::STATIC_PAGE<bool, 32> shouldEmitPage;
BBP::std::STATIC_PAGE<bool, 32> alreadyElsedPage;
BBP::std::Stack<bool> shouldEmit(&shouldEmitPage, shouldEmitPage.dataSize);
BBP::std::Stack<bool> alreadyElsed(&alreadyElsedPage, alreadyElsedPage.dataSize);

// ESA directives
PRAGMA(include)
{
	// Move back one position
	context->atPosition--;

	// Move to the start of the next word, keeping in mind the newline.
	context->moveToStartOfNextWord(true);

	// Recalculate position
	context->calculateDebugPosition();

	// Since file names can appear between '<>' or '""' (but not both), create the following handle
	std::Lexer::lex_keywordhandle handles[] = { OPENANGLEBRACKET_HANDLE, CLOSEANGLEBRACKET_HANDLE, DOUBLEQUOTES_HANDLE, DOUBLEQUOTES_HANDLE };

	// Read delimiter information, and update debug information.
	std::errno_t error = esa::ExpectDelimiters(context, 4, handles, true, 1, 1, BACKSLASH_HANDLE, context->keywordStack.atElement + 2);
	context->debugInformation.highlightLength = context->atPosition - index;

	// Switch based on error code
	switch (error)
	{
	case EFAULT: // Somehow a nullpointer got here. Abort immediately.
		context->resetInfo(context->debugInformation);
		return "AMemory instability detected.";
	case EDOM: // Somehow, an odd number ended up as argc. Either programmer fault, or stack error. Abort immediately.
		context->resetInfo(context->debugInformation);
		return "APossible stack smashing detected.";
	case EEOF: // Found End Of File before anything was found.
		context->resetInfo(context->debugInformation);
		return "EExpected filename before end of file.";
	case ENOMSG:// Found End of Line before anything was found.
		context->resetInfo(context->debugInformation);
		return "EExpected filename before end of line.";
	case ELINEEND: // End of Line found before second delimiter
		context->resetInfo(context->debugInformation);
		return "EExpected closing delimiter before end of line.";
	case ENOANO: // Attempted to close delimiter before starting it.
		context->debugInformation.highlightmsg = "NHere";
		context->debugInformation.highlight = index;
		if (BBP::esa::handle == CLOSEANGLEBRACKET_HANDLE)
			return "eExpected '<' before '>'.";
		return "EExpected '\"' before filename.";
	case EILSEQ: // Mismatched delimiters
		context->debugInformation.highlightmsg = "NHere";
		context->debugInformation.highlight = index;
		if (BBP::esa::handle == CLOSEANGLEBRACKET_HANDLE)
			return "eExpected '>' after '<'.";
		return "EExpected '\"' after '\"'.";
	case EUNFOCS: // Somehow unfocused?
		context->resetInfo(context->debugInformation);
		return "AEncountered bug 0x01.";
	case EOVERFLOW: // More than one delimiter, can only really happen with <
		context->debugInformation.highlightmsg = "NHere";
		context->debugInformation.highlight = index;
		return "EUnexpected '<' after '<'.";
	}

	//context->resetInfo(context->debugInformation);
	context->debugInformation.highlight = lastOpener;
	context->debugInformation.highlightLength = firstCloser - lastOpener;
	context->debugInformation.highlightmsg = "NHere";
	return frontend.includeFile(context, index);
}

PRAGMA(handler)
{
	return nullptr;
}

PRAGMA(define)
{
	// Hash next word.
	std::hash_t macroHash = 0;
	bool couldHash = context->moveToAndHashNextWord(macroHash, '\n');

	// Then move up one more.
	context->atPosition++;

	// If the word could not be hashed, return an error.
	if (!couldHash)
	{
		// Reset debug info
		context->resetInfo(context->debugInformation);

		// Set other pertinent info
		context->debugInformation.errName = "-Wempty-define";
		return "WEmpty #define directive ignored.";
	}

	// Otherwise print
	if (beVerbose)
		std::printf("Defining 0x%08x\n", macroHash);

	// Look for empty definition in define pages.
	std::index_t reserved = 0;
	for (std::index_t idx = 0; idx < 256; idx++)
	{
		// If we have not found a spot yet, look for an empty slot.
		if (defines[idx] == 0 && reserved == 0)
			reserved = idx;

		// We can continue checking for duplicates
		if (defines[idx] == macroHash)
		{
			if (beVerbose)
				std::printf("Found duplicate hash at index %u.\n", idx);

			// Reset debug info
			context->resetInfo(context->debugInformation);

			// Set other pertinent info
			context->debugInformation.errName = "-Wduplicate-define";
			context->debugInformation.highlight = index;
			context->debugInformation.highlightLength = context->atPosition - index + 1;
			context->debugInformation.highlightmsg = "N#define found here";
			return "WDuplicate #define directive ignored.";
		}
	}

	// If we have not reserved anything yet, return error.
	if (reserved == 0)
	{
		// Reset debug info
		context->resetInfo(context->debugInformation);

		// Set other pertinent info
		return "E#define directive failed: page is full.";
	}

	// Just write the hash in, and maybe be verbose
	defines[reserved] = macroHash;

	if (beVerbose)
		std::printf("Stored hash 0x%08x in %u.\n", macroHash, reserved);

	return nullptr;
}

PRAGMA(undef)
{
	// Hash next word.
	std::hash_t macroHash = 0;
	bool couldHash = context->moveToAndHashNextWord(macroHash, '\n');

	// If the word could not be hashed, return an error.
	if (!couldHash)
	{
		// Reset debug info
		context->resetInfo(context->debugInformation);

		// Set other pertinent info
		context->debugInformation.errName = "-Wempty-undefine";
		return "WEmpty #undefine directive ignored.";
	}

	// Otherwise print
	if (beVerbose)
		std::printf("Undefining 0x%08x\n", macroHash);

	// Look for that definition on the page
	for (std::index_t idx = 0; idx < 256; idx++)
	{
		// We don't care about anything that isn't our hash
		if (defines[idx] != macroHash)
			continue;

		// Entry found, now delete it and be verbose
		if (beVerbose)
			std::printf("Removed hash 0x%08x from %u.\n", macroHash, idx);

		defines[idx] = 0;
		return nullptr;
	}

	// Macro does not exist
	context->resetInfo(context->debugInformation);
	context->debugInformation.errName = "-Wundef";
	context->debugInformation.highlight = index;
	context->debugInformation.highlightLength = context->atPosition - index + 1;
	context->debugInformation.highlightmsg = "N#undef found here";
	return "W#undef directive failed: macro does not exist.";
}

PRAGMA(ifndef)
{
	// Hash next word.
	std::hash_t macroHash = 0;
	bool couldHash = context->moveToAndHashNextWord(macroHash, '\n');

	// Then move up one more.
	context->atPosition++;

	// If the word could not be hashed, return an error.
	if (!couldHash)
	{
		// Reset debug info
		context->resetInfo(context->debugInformation);

		// Set other pertinent info
		context->debugInformation.errName = "-Wempty-ifndef";
		return "WEmpty #ifndef directive ignored.";
	}

	

	// Look for that definition on the page
	for (std::index_t idx = 0; idx < 256; idx++)
	{
		// We don't care about anything that isn't our hash
		if (defines[idx] != macroHash)
			continue;

		// Entry found, which means that ifndef turns off.
		context->commentDepth = 4;
		alreadyElsed << false;
		shouldEmit << false;
		if (beVerbose)
			std::printf("Processing #ifndef, hash 0x%08x. Nesting at %u.\n#ifndef: \e[0;31mStop\e[0;37m.\n\n", macroHash, shouldEmit.atElement);
		return nullptr;
	}

	// Macro does not exist, so ifndef stays on, so do nothing
	alreadyElsed << false;
	shouldEmit << true;
	if (beVerbose)
		std::printf("Processing #ifndef, hash 0x%08x. Nesting at %u.\n#ifndef: \e[0;32mGo\e[0;37m.\n\n", macroHash, shouldEmit.atElement);
	return nullptr;
}

PRAGMA(ifdef)
{
	// Hash next word.
	std::hash_t macroHash = 0;
	bool couldHash = context->moveToAndHashNextWord(macroHash, '\n');

	// If the word could not be hashed, return an error.
	if (!couldHash)
	{
		// Reset debug info
		context->resetInfo(context->debugInformation);

		// Set other pertinent info
		context->debugInformation.errName = "-Wempty-ifdef";
		return "WEmpty #ifdef directive ignored.";
	}

	// Look for that definition on the page
	for (std::index_t idx = 0; idx < 256; idx++)
	{
		// We don't care about anything that isn't our hash
		if (defines[idx] != macroHash)
			continue;

		// Entry found, which means that ifdef turns on.
		alreadyElsed << true;
		shouldEmit << false;
		return nullptr;
	}

	// Macro does not exist, so ifdef goes off.
	context->commentDepth = 4;
	alreadyElsed << false;
	shouldEmit << false;
	return nullptr;
}

PRAGMA(endif)
{
	// If endif is found in isolation, emit warning
	if (shouldEmit.atElement == 0 || alreadyElsed.atElement == 0)
	{
		context->resetInfo(context->debugInformation);
		context->debugInformation.errName = "-Wstray-endif";
		context->debugInformation.highlight = index;
		context->debugInformation.highlightLength = context->atPosition - index + 1;
		context->debugInformation.highlightmsg = "N#endif found here";
		return "WConsider removing stray #endif.";
	}

	{
		// A place where variables go to die.
		bool thebin;

		//Bye-Bye!
		shouldEmit >> thebin;
		alreadyElsed >> thebin;
	}
	
	// If there is nothing in the stack, just go back to normal.
	if (shouldEmit.atElement == 0 || alreadyElsed.atElement == 0)
	{
		context->commentDepth = 0;
		if (beVerbose)
			std::printf("#endif resulted in comment depth %u (nesting at %u).\n", context->commentDepth, shouldEmit.atElement);
		return nullptr;
	}

	// Restore last settings.
	bool shouldContinue = shouldEmit[shouldEmit.atElement - 1];
	context->commentDepth = shouldContinue ? 0 : 4;

	if (beVerbose)
	{
		std::printf("#endif resulted in comment depth %u (nesting at %u).\n#endif: Restored ", context->commentDepth, shouldEmit.atElement);
		if (shouldContinue)
			std::printf("\e[0;32mGo\e[0;37m.\n\n");
		else
			std::printf("\e[0;31mStop\e[0;37m.\n\n");
	}


	return nullptr;
}

PRAGMA(else)
{
	// If else is found in isolation, emit warning
	if (shouldEmit.atElement == 0 || alreadyElsed.atElement == 0)
	{
		context->resetInfo(context->debugInformation);
		context->debugInformation.errName = "-Wstray-else";
		context->debugInformation.highlight = index;
		context->debugInformation.highlightLength = context->atPosition - index + 1;
		context->debugInformation.highlightmsg = "N#else found here";
		return "WConsider removing stray #else.";
	}

	if (alreadyElsed[alreadyElsed.atElement - 1])
	{
		// If already elsed, emit error.
		context->debugInformation.highlight = index;
		context->debugInformation.highlightLength = context->atPosition - index + 1;
		context->debugInformation.highlightmsg = "NHere";
		return "eMultiple #else directives used, only one is allowed.";
	}

	// Now flip it
	alreadyElsed[alreadyElsed.atElement - 1] = true;
	shouldEmit[shouldEmit.atElement - 1] = !shouldEmit[shouldEmit.atElement - 1];
	context->commentDepth = shouldEmit[shouldEmit.atElement - 1] ? 0 : 4;

	return nullptr;
}

PRAGMA(atomic)
{
	return nullptr;
}

PRAGMA(handover)
{
	return nullptr;
}

PRAGMA(continue)
{
	return nullptr;
}


// The keywords used
ACTION(static)
{
	return frontend.markStatic();
}

ACTION(const)
{
	return frontend.markConst();
}

ACTION(virtual)
{
	return frontend.markVirtual();
}

ACTION(extern)
{
	return frontend.markExtern();
}

ACTION(procedure)
{
	return nullptr;
}

ACTION(end)
{
	return nullptr;
}

ACTION(label)
{

	return nullptr;
}


// Instructions
INSTRUCTION(mov)
{
	return nullptr;
}

INSTRUCTION(add)
{
	return nullptr;
}

INSTRUCTION(sub)
{
	return nullptr;
}


// Tokens used
TOKEN(newline)
{
	// If inside a comment, exit out and continue as normals
	if (context->commentDepth == 1)
	{
		context->commentDepth = 0;
		return nullptr;
	}
	
	// If this line was for a symbol definition, reset it here.
	if (lineIsDefiningSymbolDefinition)
	{
		switch (backend.buildSymbol())
		{
		default:
			return nullptr;
		}
	}


	return nullptr;
}

TOKEN(semicolon)
{
	// Set comment depth to 1
	context->commentDepth = 1;
	return nullptr;
}

TOKEN(openDelimiter)
{
	if (handle == OPENBRACKET_HANDLE)
	{
		// When in isolation, this means symbol definition. Get a hash, and move on.
		std::hash_t hash = 0;
		bool couldHash = context->moveToAndHashThisWord(hash, ')');

		// If the hasher could not hash, return error.
		if (!couldHash)
			return "EInvalid symbol identifier.";

		// If the hasher reads 0, it means empty object.
		if (hash == 0 || hash == 0x1194d79c) // Magic number UwU
			return "ESymbol identifier cannot be empty.";
		
		// Move up and recalculate position
		context->atPosition++;
		context->calculateDebugPosition();

		return frontend.setHash(hash);
	}

	return nullptr;
}

TOKEN(closeDelimiter)
{
	return nullptr;
}

TOKEN(comma)
{
	return nullptr;
}

TOKEN(percent)
{
	return nullptr;
}

TOKEN(doubleQuote)
{
	if (context->literalDepth)
	{
		context->literalDepth = 0;
		return "NEnd";
	}

	context->literalDepth = context->keywordStack[handle].considerLiteralDepth;
	return "NStart";
}

TOKEN(singleQuote)
{
	return nullptr;
}

TOKEN(colon)
{
	return nullptr;
}

TOKEN(backslash)
{
	return nullptr;
}

TOKEN(size)
{
	switch (handle)
	{
	case DN_HANDLE:
		return frontend.setSize(4);
	case DB_HANDLE:
		return frontend.setSize(8);
	case DH_HANDLE:
		return frontend.setSize(16);
	case DI_HANDLE:
		return frontend.setSize(20);
	case DA_HANDLE:
		return frontend.setSize(24);
	case DW_HANDLE:
		return frontend.setSize(32);
	}

	// Will select default
	return frontend.setSize(1);
}