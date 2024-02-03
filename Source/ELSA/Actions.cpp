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
	std::errno_t error = processor.ExpectDelimiters(context, 4, handles, true, 1, 1, BACKSLASH_HANDLE, context->keywordStack.atElement + 2);
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
		if (processor.handle == CLOSEANGLEBRACKET_HANDLE)
			return "eExpected '<' before '>'.";
		return "EExpected '\"' before filename.";
	case EILSEQ: // Mismatched delimiters
		context->debugInformation.highlightmsg = "NHere";
		context->debugInformation.highlight = index;
		if (processor.handle == CLOSEANGLEBRACKET_HANDLE)
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
	context->debugInformation.highlight = processor.lastOpener;
	context->debugInformation.highlightLength = processor.firstCloser - processor.lastOpener;
	context->debugInformation.highlightmsg = "NHere";
	return processor.includeFile(context, index);
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
	if (processor.beVerbose)
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
			if (processor.beVerbose)
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

	if (processor.beVerbose)
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
	if (processor.beVerbose)
		std::printf("Undefining 0x%08x\n", macroHash);

	// Look for that definition on the page
	for (std::index_t idx = 0; idx < 256; idx++)
	{
		// We don't care about anything that isn't our hash
		if (defines[idx] != macroHash)
			continue;

		// Entry found, now delete it and be verbose
		if (processor.beVerbose)
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
		if (processor.beVerbose)
			std::printf("Processing #ifndef, hash 0x%08x. Nesting at %u.\n#ifndef: \e[0;31mStop\e[0;37m.\n\n", macroHash, shouldEmit.atElement);
		return nullptr;
	}

	// Macro does not exist, so ifndef stays on, so do nothing
	alreadyElsed << false;
	shouldEmit << true;
	if (processor.beVerbose)
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
		if (processor.beVerbose)
			std::printf("#endif resulted in comment depth %u (nesting at %u).\n", context->commentDepth, shouldEmit.atElement);
		return nullptr;
	}

	// Restore last settings.
	bool shouldContinue = shouldEmit[shouldEmit.atElement - 1];
	context->commentDepth = shouldContinue ? 0 : 4;

	if (processor.beVerbose)
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
	return processor.markStatic();
}

ACTION(const)
{
	return processor.markConst();
}

ACTION(virtual)
{
	return processor.markVirtual();
}

ACTION(extern)
{
	return processor.markExtern();
}

ACTION(procedure)
{
	// Check appropriate flags
	if (processor.lineIsDefiningInstruction || processor.lineIsDefiningSymbolDefinition)
		return "E'procedure' is a reserved keyword."; // If any of these flags are true, return error.

	// Set appropriate flags
	processor.lineIsDefiningProcedure = true;

	// Set index to here
	processor.setProcedureAddress();

	return nullptr;
}

ACTION(end)
{
	// Check if inside a procedure
	if (processor.procedureHash == 0)
		return "EInvalid 'end'; Must be matched with 'procedure'.";

	// Save end
	processor.endedAt = processor.application->builder._text().stack.atElement;

	// Push function
	std::conststring error = processor.emitProcedure();

	// Now reset procedure flags
	processor.procedureName.static_data[0] = 0;
	processor.procedureHash = 0;
	processor.startedAt = 0;
	processor.endedAt = 0;
	processor.procedureArgumentCount = 0;
	processor.doneDeclaringProcedure = false;
	processor.externProcedure = false;

	// Check for any subscribers.
	if (processor.routineStarts.atElement)
	{
		// Reset them
		processor.routineStarts.atElement = 0;

		// Return error
		return "EUnterminated ':' tokens before end of procedure.";
	}

	// Then return error
	return error;
}

ACTION(endroutine)
{
	// If no subscribers, emit warning
	if (processor.routineStarts.atElement == 0)
	{
		context->resetInfo(context->debugInformation);
		context->debugInformation.errName = "-Wignored-double-at";
		return "WIgnored stray '@@' because no ':' tokens precedes it.";
	}

	// Get current index in .text
	std::index_t currentIndex = processor.application->builder.sections[processor.application->builder.text].stack.atElement;

	// Get routine starts
	std::offset_t startOfRoutine = 0;
	std::index_t zero = 0;
	std::index_t subscriberCount = std::read(&processor.routineSubscriberCount, processor.routineStarts.atElement);

	std::write(&processor.routineSubscriberCount, zero, processor.routineStarts.atElement);
	processor.routineStarts >> startOfRoutine;

	// Loop over subscribers	
	for (std::index_t index = 0; index < subscriberCount; index++)
	{
		// Get target offset
		std::offset_t target = 0;
		processor.subscribers >> target;

		// Write into section
		processor.application->builder.sections[processor.application->builder.text].stack[target] = (currentIndex >> 24) & 0xFF;
		processor.application->builder.sections[processor.application->builder.text].stack[target+1] = (currentIndex >> 16) & 0xFF;
		processor.application->builder.sections[processor.application->builder.text].stack[target+2] = (currentIndex >> 8) & 0xFF;
		processor.application->builder.sections[processor.application->builder.text].stack[target+3] = (currentIndex >> 0) & 0xFF;

	}

	return nullptr;
}


// Tokens used
TOKEN(newline)
{

	// Check if procedure is being defined, and is also marked extern. Because if so, emit empty function
	if (processor.lineIsDefiningProcedure && processor.externProcedure)
	{
		// Emit procedure
		std::conststring error = processor.emitProcedure();

		// Now reset procedure flags
		processor.procedureName.static_data[0] = 0;
		processor.procedureHash = 0;
		processor.startedAt = 0;
		processor.endedAt = 0;
		processor.procedureArgumentCount = 0;
		processor.doneDeclaringProcedure = false;
		processor.externProcedure = false;

		// Also reset symbol
		processor.resetSymbol();

		// Just for logging
		return error;
	}

	// Reset this flag
	processor.lineIsDefiningProcedure = false;

	// If inside a comment, exit out and continue as normals
	if (context->commentDepth == 1)
	{
		context->commentDepth = 0;
		return nullptr;
	}

	// If this line was for a symbol definition, reset it here.
	// Also, if symbol is marked extern and this point is reached, also run it. Because this is definately not a procedure then.
	if (processor.lineIsDefiningSymbolDefinition || (processor.symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN))
	{
		// Also reset extern procedure flag
		processor.externProcedure = false;

		processor.lineIsDefiningSymbolDefinition = false;
		switch (processor.buildSymbol())
		{
		case 0:
			return nullptr;
		case ENOBUFS:
			return "EUnable to register quasi symbol: quasi symbol limit reached.";
		case EADDRINUSE:
			return "EIdentifier already used.";
		case EBADR:
			return "ECannot assign symbol to procedure marked extern.";
		case 2:
			return "NOverriding extern symbol.";
		default:
			return "ECould not export symbol.";
		}
	}

	// If this line was for a instruction definition, reset it here.
	if (processor.lineIsDefiningInstruction)
	{
		// Emit instruction
		processor.emitInstruction();

		// We want to clear remains
		processor.resetInstruction();
		
		// Stop defining instructions
		processor.lineIsDefiningInstruction = false;
		return nullptr;
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

	// Not interested if this keyword is not '(' or '['.
	if (handle != OPENBRACKET_HANDLE && handle != OPENSQBRACKET_HANDLE)
		return nullptr;

	// If square bracket and not inside procedure, check if it is an attribute
	// Neat thing is that this also supports variables :)
	if (handle == OPENSQBRACKET_HANDLE && !processor.lineIsDefiningProcedure && !processor.lineIsDefiningInstruction)
	{
		// Expect [ or ]
		context->atPosition--;
		std::Lexer::lex_keywordhandle handles[] = { OPENSQBRACKET_HANDLE, CLOSESQBRACKET_HANDLE };
		context->inhibitSpecialFunctions = true;
		std::errno_t error = processor.ExpectDelimiters(context, 2, handles, true, 2, 2, BACKSLASH_HANDLE, context->keywordStack.atElement + 2);
		context->inhibitSpecialFunctions = false;

		// Error handling
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
			return "EExpected attribute before end of file.";
		case ENOMSG:// Found End of Line before anything was found.
			context->resetInfo(context->debugInformation);
			return "EExpected attribute before end of line.";
		case ELINEEND: // End of Line found before second delimiter
			context->resetInfo(context->debugInformation);
			return "EExpected closing delimiter before end of line.";
		case ENOANO: // Attempted to close delimiter before starting it.
			context->debugInformation.highlightmsg = "NHere";
			context->debugInformation.highlight = index;
			return "eExpected '[' before ']'.";
		case EILSEQ: // Mismatched delimiters
			context->debugInformation.highlightmsg = "NHere";
			context->debugInformation.highlight = index;
			return "eExpected ']' after '['.";
		case EUNFOCS: // Somehow unfocused?
			context->resetInfo(context->debugInformation);
			return "AEncountered bug.";
		case EOVERFLOW: // More than one delimiter, can only really happen with <
			context->debugInformation.highlightmsg = "NHere";
			context->debugInformation.highlight = index;
			return "EUnexpected '[' after '['.";
		}

		// Check if empty
		if (processor.lastOpener == processor.firstCloser)
		{
			context->resetInfo(context->debugInformation);
			context->debugInformation.errName = "-Wempty-attribute";
			return "WEmpty attribute ignored.";
		}

		// Declare string for attribute
		std::string attribute = std::string();

		// Check if attribute is less than size
		if (processor.firstCloser - processor.lastOpener <= attribute.dataSize)
			return "EAttribute cannot be longer than 24 characters.";

		// Now encode attribute
		attribute.data = std::read_a(&context->data, processor.lastOpener);
		attribute.dataSize = processor.firstCloser - processor.lastOpener;

		// Swap out ']' with 0
		attribute.data[attribute.dataSize] = 0;

		// Calculate hash
		std::hash_t hash = std::strhsh(attribute);

		// Try doing number
		{
			float parsedFloat = 0;
			int parsedInt = 0;
			bool isFloat = 0;

			if (context->tryParseNumerical(processor.lastOpener, processor.firstCloser, isFloat, parsedFloat, parsedInt))
			{
				// Check if is float, because it is not possible to set floats
				if (isFloat)
				{
					// Restore ]
					attribute.data[attribute.dataSize] = ']';

					context->resetInfo(context->debugInformation);
					context->debugInformation.highlight = index;
					context->debugInformation.highlightLength = attribute.dataSize + 4;
					context->debugInformation.highlightmsg = "NAttribute defined here:";
					
					return "ECannot use floats";
				}

				if (parsedInt < 0)
				{
					// Restore ]
					attribute.data[attribute.dataSize] = ']';

					context->resetInfo(context->debugInformation);
					context->debugInformation.highlight = index;
					context->debugInformation.highlightLength = attribute.dataSize + 4;
					context->debugInformation.highlightmsg = "NAttribute defined here:";

					return "ENumerical attributes must be unsigned";
				}

				// Check if number is above threshold 
				if (parsedInt > 255)
				{
					// Restore ]
					attribute.data[attribute.dataSize] = ']';

					context->resetInfo(context->debugInformation);
					context->debugInformation.highlight = index;
					context->debugInformation.highlightLength = attribute.dataSize + 4;
					context->debugInformation.highlightmsg = "NAttribute defined here:";

					return "ENumerical attributes may not exceed 255";
				}

				return nullptr;
			}
		}

		// Get type and entry
		std::halfword type = processor.application->builder.type();
		std::address_t entry = processor.application->builder.entry();

		// Restore ]
		attribute.data[attribute.dataSize] = ']';

		// Do thing based on hash
		switch (hash)
		{
		case 0x2836b5a2: // [[sharedlib]]
			// Check if type isn't already sharedlib
			if (type == std::ELF::ET_DYN)
			{
				context->resetInfo(context->debugInformation);
				context->debugInformation.errName = "-Wrepeated-attr";
				return "WRepeated [[sharedlib]] attribute";
			}


			// If executable type
			if (type == std::ELF::ET_EXEC || entry)
				return "E[[sharedlib]] attribute incompatible with [[entry]].";

			// Set type to shared lib
			processor.application->builder.type(std::ELF::ET_DYN);
			return nullptr;
		case 0x0168c712: // [[entry]]
			// Check if type isn't already executable
			if (type == std::ELF::ET_EXEC || entry)
			{
				context->resetInfo(context->debugInformation);
				return "ERepeated [[entry]] attribute not allowed";
			}

			// If executable type
			if (type == std::ELF::ET_DYN)
				return "E[[entry]] attribute incompatible with [[sharedlib]].";

			// Set type to shared lib
			processor.application->builder.type(std::ELF::ET_EXEC);
			processor.application->builder.entry(processor.application->builder._text().stack.atElement);
			return nullptr;
		case 0x0e9327cf: // [[handler]]
			context->resetInfo(context->debugInformation);
			return "N[[handler]] is currently nop.";
		case 0x07220b7b: // [[shared]]. This is used to explicitly mark that this symbol is not in this file

			// Check if not extern
			if (!(processor.symbol.subtype & esa::SYMTMOD_EXTERN))
			{
				context->resetInfo(context->debugInformation);
				return "E[[shared]] only valid after 'extern'. Use [[export]] instead to export shared symbol.";
			}

		case 0x2325a40c: // [[export]]. Used to indicate explicitly that this is for shared library linking

			// Check if not already shared
			if (processor.symbol.subtype & esa::SYMTMOD_SHARED)
			{
				context->resetInfo(context->debugInformation);
				return "EDouble [[shared]] or [[export]] attribute.";
			}

			// Set shared
			processor.symbol.subtype |= esa::SYMTMOD_SHARED;

			return nullptr;
		case 0x00c89ae5: // [[align]]
			context->resetInfo(context->debugInformation);
			return "N[[align]] is currently nop.";
		}

		// Unrecognized attribute
		context->resetInfo(context->debugInformation);
		context->debugInformation.errName = "-Wunknown-attr";
		context->debugInformation.highlight = index;
		context->debugInformation.highlightLength = attribute.dataSize + 4;
		context->debugInformation.highlightmsg = "NAttribute defined here:";

		std::printf("Unkown attribute %s, 0x%08x\n", attribute.data, hash);
		
		return "WUnkown attribute ignored.";
	}

	// Otherwise, if handle is open square bracket, it can be ignored
	if (handle == OPENSQBRACKET_HANDLE)
		return nullptr;

	// Not doing anything with procedures, do this
	if (!processor.lineIsDefiningProcedure)
	{
		// Well, we might as well flag quasi symbol
		processor.lineIsDefiningSymbolDefinition = true;

		// Start of the name is here
		processor.symbol.start = context->atPosition;

		// When in isolation, this means symbol definition. Get a hash, and move on.
		std::hash_t hash = 0;
		bool couldHash = context->moveToAndHashThisWord(hash, ')');

		// End of the name is here
		processor.symbol.end = context->atPosition;

		// If the hasher could not hash, return error.
		if (!couldHash)
			return "EInvalid symbol identifier.";

		// If the hasher reads 0, it means empty object.
		if (hash == 0 || hash == 0x1194d79c) // Magic number UwU
			return "ESymbol identifier cannot be empty.";

		// Move up and recalculate position
		context->atPosition++;
		context->calculateDebugPosition();

		return processor.setHash(hash);
	}

	// This must be the procedure identifier if this is the first time this is found
	if (std::read(&processor.procedureName, 0) == 0)
	{
		// Get procedure hash
		std::word start = context->atPosition;

		// Hash procedure identifier
		std::hash_t hash = 0;
		bool couldHash = context->moveToAndHashThisWord(hash, ')');

		// End of the name is here
		std::word end = context->atPosition;

		// If hashing failed, return error
		if (!couldHash)
			return "Einvalid procedure Identifier.";

		// Otherwise, if hash is empty, return error
		if (hash == 0 || hash == 0x1194d79c) // OMG, stop it with the magic numbers daddy ~~
			return "EProcedure identifier cannot be empty.";

		// Move up one and calculate position again
		context->atPosition++;
		context->calculateDebugPosition();

		// Set procedure names
		processor.setProcedureIdentifier(start, end, hash);

		// Return success
		return nullptr;
	}
	else if (!processor.doneDeclaringProcedure)
	{
		// This must be the second argument, I.E., the size.

		// Then read the character, and do something based on it.
		std::string_element c = std::read(&context->data, context->atPosition);

		// Check if within '0' and '8' (48 and 56)
		if (c >= '0' && c <= '8')
			processor.setProcedureArgCount(c - '0');
		else if (c == ')')
			return "EExpected a numerical constant between '0' and '8' (inclusive) between '()'.";
		else
			return "EInvalid procedure argument count.";

		// Now check for ')' immediately after
		context->atPosition++;
		context->calculateDebugPosition();

		// Then read the character, and do something based on it.
		c = std::read(&context->data, context->atPosition);

		if (c == ')')
			return nullptr;
		return "EExpected ')' immediately after numerical constant.";
	}
	else
	{
		return "EUnexpected argument supplied during procedure declaration.";
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

TOKEN(caret)
{
	// Check if any routines are available
	if (processor.routineStarts.atElement == 0)
	{
		// Reset info and throw error
		context->resetInfo(context->debugInformation);
		return "ECannot find address of routine since '^' is not within routine.";
	}

	// Set argument
	processor.numericalArgument({ processor.routineStarts[processor.routineStarts.atElement - 1] }, false);

	// If instruction is part of a jmp, jmpr, or jmpc set, flag suffix absolute.
	if (processor.instruction.header.opcode == userspace::JMP || processor.instruction.header.opcode == userspace::JMPZ || processor.instruction.header.opcode == userspace::JMPC)
		processor.instruction.header.suffix |= userspace::absoluteJmp;
	return nullptr;
}


BBP::std::index_t stringStart = 0;
TOKEN(doubleQuote)
{
	

	// If inside a string, exit out of the string
	if (context->literalDepth)
	{
		// Exit the string
		context->literalDepth = 0;

		// Check if symbol is extern
		if (processor.symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN)
			return "ECannot add data to extern symbol";

		// If also defining symbol, add numericals.
		if (processor.lineIsDefiningSymbolDefinition)
		{
			for (std::index_t i = stringStart; i < context->atPosition - 1; i++)
				processor.addNumerical(std::Lexer::numerical_t{ (std::word)context->data.data[i] }, false);
		}

		return nullptr;
	}

	context->literalDepth = context->keywordStack[handle].considerLiteralDepth;
	stringStart = context->atPosition;
	return nullptr;
}

TOKEN(singleQuote)
{
	return nullptr;
}

TOKEN(colon)
{
	// If not defining a instruction, return error
	if (!processor.lineIsDefiningInstruction)
		return "E':' Can only be used when constructing an instruction.";

	// Get offset
	std::offset_t subscriber = processor.calculateAbsoluteArgumentOffset();

	// Check if instruction is routine. If so, Add routine start. Also flag absolute (Since this measures distance in bytes, not instructions)
	if (processor.instruction.header.opcode == userspace::CTRL && processor.instruction.header.suffix == userspace::routine)
	{
		// Add routine start
		processor.routineStarts << subscriber;

		// Flag absolute
		processor.instruction.header.suffix |= userspace::absolute;

	}

	// Check if allowed to add subscribers
	if (processor.subscribers.atElement >= processor.subscribers.max_elements)
		return "EReached the limit of consecutive ':' tokens allowed.";

	// Add subscriber to subscribers stack
	processor.subscribers << subscriber;

	// Increase subscriber count
	processor.routineSubscriberCount.data[processor.routineStarts.atElement]++;

	// Set argument
	processor.numericalArgument({'EMMA'}, false);
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
		return processor.setSize(4);
	case DB_HANDLE:
		return processor.setSize(8);
	case DH_HANDLE:
		return processor.setSize(16);
	case DI_HANDLE:
		return processor.setSize(20);
	case DA_HANDLE:
		return processor.setSize(24);
	case DW_HANDLE:
		return processor.setSize(32);
	}

	// Will select default
	return processor.setSize(1);
}










// Instructions

INSTRUCTION(nil)
{
	processor.setInstruction(userspace::NIL, 0, 0, 0);

	return nullptr;
}

INSTRUCTION(stack)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case PUSH_HANDLE:
		suffix = userspace::push;
		break;
	case POP_HANDLE:
		suffix = userspace::pop;
		break;
	case POPA_HANDLE:
		suffix = userspace::pop | userspace::argument;
		break;
	}

	processor.setInstruction(userspace::STACK, suffix, 0, 1);
	if (handle == PUSH_HANDLE)
		processor.expectAny(0);
	else
		processor.expectAddress(0);

	return nullptr;
}

INSTRUCTION(ctrl)
{
	std::byte suffix = 0;
	std::byte argCount = 0;

	switch (handle)
	{
	case RET_HANDLE:
		suffix = userspace::ret;
		argCount = 1;
		processor.surpressArgCountWarning(0);
		processor.setInstruction(userspace::CTRL, suffix, 0, argCount);
		break;

	case CALL_HANDLE:
		suffix = userspace::call;
		argCount = 8;
		processor.surpressArgCountWarning();
		processor.surpress[0] = false;
		processor.setInstruction(userspace::CTRL, suffix, 0, argCount);
		processor.expectAddress(0);
		for (std::byte n = 1; n < 8; n++)
			processor.expectAny(n);
		break;

	case CALLP_HANDLE:
		suffix = userspace::callp;
		argCount = 8;
		processor.surpressArgCountWarning();
		processor.surpress[0] = false;
		processor.setInstruction(userspace::CTRL, suffix, 0, argCount);
		processor.expectAddress(0);
		for (std::byte n = 1; n < 8; n++)
			processor.expectAny(n);
		break;

	case SYSCALL_HANDLE:
		suffix = userspace::syscall;
		argCount = 8;
		processor.surpressArgCountWarning();
		processor.surpress[0] = false;
		processor.setInstruction(userspace::CTRL, suffix, 0, argCount);
		processor.expectLiteral(0);
		for (std::byte n = 1; n < 8; n++)
			processor.expectAny(n);
		break;

	case ROUTINE_HANDLE:
		suffix = userspace::routine;
		argCount = 1;
		processor.setInstruction(userspace::CTRL, suffix, 0, argCount);
		processor.expectLiteral(0);
		break;
	}

	return nullptr;
}

INSTRUCTION(mem)
{
	
	std::byte firsttype = 0;
	std::byte secondtype = 0;

	// Decode first type
	switch (handle)
	{
	case MOVB_HANDLE:
	case MOVBH_HANDLE:
	case MOVBW_HANDLE:
	case MOVBA_HANDLE:
		firsttype = userspace::fbyte;
		break;

	case MOVH_HANDLE:
	case MOVHB_HANDLE:
	case MOVHW_HANDLE:
	case MOVHA_HANDLE:
		firsttype = userspace::fhalfword;
		break;

	case MOVW_HANDLE:
	case MOVWB_HANDLE:
	case MOVWH_HANDLE:
	case MOVWA_HANDLE:
		firsttype = userspace::fword;
		break;

	case MOVA_HANDLE:
	case MOVAB_HANDLE:
	case MOVAH_HANDLE:
	case MOVAW_HANDLE:
		firsttype = userspace::faddress;
		break;

	}

	// Decode second type
	switch (handle)
	{
	case MOVB_HANDLE:
	case MOVHB_HANDLE:
	case MOVWB_HANDLE:
	case MOVAB_HANDLE:
		secondtype = userspace::sbyte;
		break;

	case MOVH_HANDLE:
	case MOVBH_HANDLE:
	case MOVWH_HANDLE:
	case MOVAH_HANDLE:
		secondtype = userspace::shalfword;
		break;

	case MOVW_HANDLE:
	case MOVBW_HANDLE:
	case MOVHW_HANDLE:
	case MOVAW_HANDLE:
		secondtype = userspace::sword;
		break;

	case MOVA_HANDLE:
	case MOVBA_HANDLE:
	case MOVHA_HANDLE:
	case MOVWA_HANDLE:
		secondtype = userspace::saddress;
		break;
	}

	processor.setInstruction(userspace::MEM, firsttype | secondtype, 0, 2);
	processor.expectAddress(0);
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(inc)
{
	std::byte suffix = 0;
	
	switch (handle)
	{
	case INC_HANDLE:
		suffix = 0;
		break;
	case INCF_HANDLE:
		suffix = userspace::ffloat;
		break;
	}

	processor.setInstruction(userspace::INC, suffix, 0, 1);
	processor.expectAddress(0);

	return nullptr;
}

INSTRUCTION(dec)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case DEC_HANDLE:
		suffix = 0;
		break;
	case DECF_HANDLE:
		suffix = userspace::ffloat;
		break;
	}

	processor.setInstruction(userspace::DEC, suffix, 0, 1);
	processor.expectAddress(0);

	return nullptr;
}

INSTRUCTION(neg)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case NEG_HANDLE:
		suffix = 0;
		break;
	case NEGF_HANDLE:
		suffix = userspace::ffloat;
		break;
	}

	processor.setInstruction(userspace::NEG, suffix, 0, 1);
	processor.expectAddress(0);

	return nullptr;
}

INSTRUCTION(add)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case ADD_HANDLE:
		suffix = 0;
		break;
	case ADDF_HANDLE:
		suffix = userspace::ffloat;
		break;
	case ADDIF_HANDLE:
		suffix = userspace::sfloat;
		break;
	case ADDFF_HANDLE:
		suffix = userspace::ffloat | userspace::sfloat;
		break;
	}

	// If the handle is ADD, expect any amount of args
	if (handle == ADD_HANDLE)
	{
		processor.setInstruction(userspace::ADD, suffix, 0, 8);
		processor.surpressArgCountWarning();
		processor.expectAny(0);
		processor.expectAny(1);
		processor.expectAny(2);
		processor.expectAny(3);
		processor.expectAny(4);
		processor.expectAny(5);
		processor.expectAny(6);
		processor.expectAny(7);

		return nullptr;
	}

	processor.setInstruction(userspace::ADD, suffix, 0, 2);
	processor.expectAny(0);
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(sub)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case SUB_HANDLE:
		suffix = 0;
		break;
	case SUBF_HANDLE:
		suffix = userspace::ffloat;
		break;
	case SUBIF_HANDLE:
		suffix = userspace::sfloat;
		break;
	case SUBFF_HANDLE:
		suffix = userspace::ffloat | userspace::sfloat;
		break;
	}

	// If the handle is SUB, expect any amount of args
	if (handle == SUB_HANDLE)
	{
		processor.setInstruction(userspace::SUB, suffix, 0, 8);
		processor.surpressArgCountWarning();
		processor.expectAny(0);
		processor.expectAny(1);
		processor.expectAny(2);
		processor.expectAny(3);
		processor.expectAny(4);
		processor.expectAny(5);
		processor.expectAny(6);
		processor.expectAny(7);

		return nullptr;
	}

	processor.setInstruction(userspace::SUB, suffix, 0, 2);
	processor.expectAny(0);
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(mul)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case MUL_HANDLE:
		suffix = 0;
		break;
	case MULF_HANDLE:
		suffix = userspace::ffloat;
		break;
	case MULIF_HANDLE:
		suffix = userspace::sfloat;
		break;
	case MULFF_HANDLE:
		suffix = userspace::ffloat | userspace::sfloat;
		break;
	}

	// If the handle is MUL, expect any amount of args
	if (handle == MUL_HANDLE)
	{
		processor.setInstruction(userspace::MUL, suffix, 0, 8);
		processor.surpressArgCountWarning();
		processor.expectAny(0);
		processor.expectAny(1);
		processor.expectAny(2);
		processor.expectAny(3);
		processor.expectAny(4);
		processor.expectAny(5);
		processor.expectAny(6);
		processor.expectAny(7);

		return nullptr;
	}

	processor.setInstruction(userspace::MUL, suffix, 0, 2);
	processor.expectAny(0);
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(div)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case DIV_HANDLE:
		suffix = 0;
		break;
	case DIVF_HANDLE:
		suffix = userspace::ffloat;
		break;
	case DIVIF_HANDLE:
		suffix = userspace::sfloat;
		break;
	case DIVFF_HANDLE:
		suffix = userspace::ffloat | userspace::sfloat;
		break;
	}

	// Divide always expects 2 arguments
	processor.setInstruction(userspace::DIV, suffix, 0, 2);
	processor.expectAny(0);
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(mod)
{
	std::byte suffix = 0;

	switch (handle)
	{
	case ADD_HANDLE:
		suffix = 0;
		break;
	case ADDF_HANDLE:
		suffix = userspace::ffloat;
		break;
	case ADDIF_HANDLE:
		suffix = userspace::sfloat;
		break;
	case ADDFF_HANDLE:
		suffix = userspace::ffloat | userspace::sfloat;
		break;
	}
	
	// Mod always expects 2 arguments
	processor.setInstruction(userspace::MOD, suffix, 0, 2);
	processor.expectAny(0);
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(lbs)
{
	// Set based on handle
	processor.setInstruction(userspace::LBS, handle == LBS_HANDLE ? 0 : userspace::inplace, 0, 2);

	if (handle == LBS_HANDLE)
		processor.expectAny(0);
	else
		processor.expectAddress(0);

	processor.expectAny(1);
	return nullptr;
}

INSTRUCTION(rbs)
{
	// Set based on handle
	processor.setInstruction(userspace::LBS, handle == RBS_HANDLE ? 0 : userspace::inplace, 0, 2);

	if (handle == RBS_HANDLE)
		processor.expectAny(0);
	else
		processor.expectAddress(0);

	processor.expectAny(1);
	return nullptr;
}

INSTRUCTION(or )
{

	std::byte suffix = 0;

	switch (handle)
	{
	case OR_HANDLE:
		suffix = 0;
		break;
	case ORIP_HANDLE:
		suffix = userspace::inplace;
		break;
	case ORB_HANDLE: // I AM ORB!!!!!!!!!!!!!!!!!!!!!!!!! FUCK YOU MOTHERFUCKERSSSSSSSSSSSSSSSSS
		suffix = userspace::bitwise;
		break;
	case ORBIP_HANDLE:
		suffix = userspace::bitwise | userspace::inplace;
		break;
	}

	// Set instruction
	processor.setInstruction(userspace::OR, suffix, 0, 2);

	// If in place, expect writeable value
	if (suffix | userspace::inplace)
		processor.expectAddress(0);
	else
		processor.expectAny(0);

	// Expect anything on second arg
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(xor)
{

	std::byte suffix = 0;

	switch (handle)
	{
	case XOR_HANDLE:
		suffix = 0;
		break;
	case XORIP_HANDLE:
		suffix = userspace::inplace;
		break;
	case XORB_HANDLE: // I AM ORB!!!!!!!!!!!!!!!!!!!!!!!!! FUCK YOU MOTHERFUCKERSSSSSSSSSSSSSSSSS
		suffix = userspace::bitwise;
		break;
	case XORBIP_HANDLE:
		suffix = userspace::bitwise | userspace::inplace;
		break;
	}

	// Set instruction
	processor.setInstruction(userspace::XOR, suffix, 0, 2);

	// If in place, expect writeable value
	if (suffix | userspace::inplace)
		processor.expectAddress(0);
	else
		processor.expectAny(0);

	// Expect anything on second arg
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(and)
{

	std::byte suffix = 0;

	switch (handle)
	{
	case AND_HANDLE:
		suffix = 0;
		break;
	case ANDIP_HANDLE:
		suffix = userspace::inplace;
		break;
	case ANDB_HANDLE: // I AM ORB!!!!!!!!!!!!!!!!!!!!!!!!! FUCK YOU MOTHERFUCKERSSSSSSSSSSSSSSSSS
		suffix = userspace::bitwise;
		break;
	case ANDBIP_HANDLE:
		suffix = userspace::bitwise | userspace::inplace;
		break;
	}

	// Set instruction
	processor.setInstruction(userspace::AND, suffix, 0, 2);

	// If in place, expect writeable value
	if (suffix | userspace::inplace)
		processor.expectAddress(0);
	else
		processor.expectAny(0);

	// Expect anything on second arg
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(not)
{

	std::byte suffix = 0;

	switch (handle)
	{
	case NOT_HANDLE:
		suffix = 0;
		break;
	case NOTIP_HANDLE:
		suffix = userspace::inplace;
		break;
	case NOTB_HANDLE: // I AM ORB!!!!!!!!!!!!!!!!!!!!!!!!! FUCK YOU MOTHERFUCKERSSSSSSSSSSSSSSSSS
		suffix = userspace::bitwise;
		break;
	case NOTBIP_HANDLE:
		suffix = userspace::bitwise | userspace::inplace;
		break;
	}

	// Set instruction
	processor.setInstruction(userspace::NOT, suffix, 0, 2);

	// If in place, expect writeable value
	if (suffix | userspace::inplace)
		processor.expectAddress(0);
	else
		processor.expectAny(0);

	// Expect anything on second arg
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(flag)
{
	// If simple flag handle, then just do this
	if (handle == FLAG_HANDLE)
	{
		processor.setInstruction(userspace::FLAG, 1, 0, 1);
		processor.expectLiteral(0);

		return nullptr;
	}

	// Otherwise, this is compare flag.
	processor.setInstruction(userspace::FLAG, 0, 0, 2);
	processor.surpressArgCountWarning(1);
	processor.expectAny(0);
	processor.expectAny(1);

	return nullptr;
}

INSTRUCTION(jmp)
{
	if (handle == JMP_HANDLE)
	{
		processor.setInstruction(userspace::JMP, 0, 0, 1);
		processor.expectAddress(0);
		
		return nullptr;
	}

	processor.setInstruction(userspace::JMP, userspace::jmproutine, 0, 0);
	return nullptr;
}

INSTRUCTION(jmpc)
{

	std::byte suffix = 0;

	switch (handle)
	{
	case JLTNR_HANDLE:
		suffix = userspace::negated;
	case JLTR_HANDLE:
		suffix |= userspace::lessthan | userspace::jmproutine;
		processor.setInstruction(userspace::JMPC, suffix, 0, 0);
		return nullptr;

	case JLTN_HANDLE:
		suffix = userspace::negated;
	case JLT_HANDLE:
		suffix |= userspace::lessthan;
		processor.setInstruction(userspace::JMPC, suffix, 0, 1);
		processor.expectAddress(0);
		return nullptr;


	case JGTNR_HANDLE:
		suffix = userspace::negated;
	case JGTR_HANDLE:
		suffix |= userspace::greaterthan | userspace::jmproutine;
		processor.setInstruction(userspace::JMPC, suffix, 0, 0);
		return nullptr;

	case JGTN_HANDLE:
		suffix = userspace::negated;
	case JGT_HANDLE:
		suffix |= userspace::greaterthan;
		processor.setInstruction(userspace::JMPC, suffix, 0, 1);
		processor.expectAddress(0);
		return nullptr;
	}

	return nullptr;
}

INSTRUCTION(jmpz)
{

	std::byte suffix = 0;

	switch (handle)
	{
	case JLZNR_HANDLE:
		suffix = userspace::negated;
	case JLZR_HANDLE:
		suffix |= userspace::jmproutine | userspace::lessthan;
		processor.setInstruction(userspace::JMPZ, suffix, 0, 0);
		return nullptr;

	case JLZN_HANDLE:
		suffix = userspace::negated;
	case JLZ_HANDLE:
		suffix |= userspace::lessthan;
		processor.setInstruction(userspace::JMPZ, suffix, 0, 1);
		processor.expectAddress(0);
		return nullptr;




	case JEZNR_HANDLE:
		suffix = userspace::negated;
	case JEZR_HANDLE:
		suffix |= userspace::jmproutine | userspace::equalto;
		processor.setInstruction(userspace::JMPZ, suffix, 0, 0);
		return nullptr;

	case JEZN_HANDLE:
		suffix = userspace::negated;
	case JEZ_HANDLE:
		suffix |= userspace::equalto;
		processor.setInstruction(userspace::JMPZ, suffix, 0, 1);
		processor.expectAddress(0);
		return nullptr;




	case JGZNR_HANDLE:
		suffix = userspace::negated;
	case JGZR_HANDLE:
		suffix |= userspace::jmproutine | userspace::greaterthan;
		processor.setInstruction(userspace::JMPZ, suffix, 0, 0);
		return nullptr;

	case JGZN_HANDLE:
		suffix = userspace::negated;
	case JGZ_HANDLE:
		suffix |= userspace::greaterthan;
		processor.setInstruction(userspace::JMPZ, suffix, 0, 1);
		processor.expectAddress(0);
		return nullptr;
	}


	return nullptr;
}

INSTRUCTION(page)
{
   switch(handle)
   {
   case PAGE_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::assign | userspace::read | userspace::write, 0, 1);
	   processor.expectLiteral(0);
	   return nullptr;

   case PAGEAW_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::assign | userspace::write, 0, 1);
	   processor.expectLiteral(0);
	   return nullptr;
   case PAGEAR_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::assign | userspace::read, 0, 1);
	   processor.expectLiteral(0);
	   return nullptr;

   case PAGEW_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::write, 0, 2);
	   processor.expectLiteral(0);
	   processor.expectAny(1);
	   return nullptr;
   case PAGER_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::read, 0, 2);
	   processor.expectLiteral(0);
	   processor.expectAny(1);
	   return nullptr;

   case PALLOC_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::assign | userspace::memoryOp | userspace::sizeOp, 0, 1);
	   processor.expectLiteral(0);
	   return nullptr;
   case PFREE_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::memoryOp | userspace::sizeOp, 0, 0);
	   return nullptr;

   case PAGES_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::sizeOp | userspace::read, 0, 2);
	   processor.expectAddress(0);
	   processor.expectLiteral(1);
	   return nullptr;
   case PAGEBC_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::sizeOp | userspace::read | userspace::bytes, 0, 2);
	   processor.expectAddress(0);
	   processor.expectLiteral(1);
	   return nullptr;

   case PASSIGN_HANDLE:
	   processor.setInstruction(userspace::PAGE, userspace::assign | userspace::read | userspace::write | userspace::memoryOp, 0, 1);
	   processor.expectAddress(0);
	   return nullptr;
   }

   return "APage instruction not recognized. Please ensure you are using the right version of ELSA.";
}

INSTRUCTION(signal)
{
	processor.setInstruction(userspace::SIGNAL, handle == SIGNALF_HANDLE ? 1 : 0, 0, handle == SIGNALF_HANDLE ? 2 : 1);
	if (handle == SIGNALF_HANDLE)
	{
		processor.expectLiteral(0);
		processor.expectLiteral(1);
	}
	else
	{
		processor.expectLiteral(0);
	}

	return nullptr;
}

INSTRUCTION(halt)
{
	switch (handle)
	{
	case HLT_HANDLE:
		processor.setInstruction(userspace::HALT, 0, 0, 0);
		return nullptr;
	case HCF_HANDLE:
		processor.setInstruction(userspace::HALT, 1, 0, 0);
		processor.esaParser.debugInformation.errName = "-Wuse-of-hcf";
		return "WHCF instruction emitted.";
	case HBU_HANDLE:
		processor.setInstruction(userspace::HALT, 2, 0, 0);
		processor.esaParser.debugInformation.errName = "-Wuse-of-hbu";
		return "WHBU instruction emitted.";
		return nullptr;
	}
	return "AUnrecognized halt instruction. Please make sure you are using the right version of ELSA.";
}

/*

INSTRUCTION(push)
{
	// Set instruction
	processor.setInstruction(userspace::STACK, 0, 0, 1);
	processor.expectAny(0);

	return nullptr;
}

INSTRUCTION(pop)
{
	// Set instruction
	processor.setInstruction(userspace::STACK, 0, 0, 1);
	processor.expectAny(0);

	return nullptr;
}

INSTRUCTION(popa)
{
	// Set instruction
	processor.setInstruction(userspace::STACK, 1, 0, 1);
	processor.expectAny(0);

	return nullptr;
}

INSTRUCTION(mov)
{
	// Set instruction
	processor.setInstruction(userspace::MOV, 0, 0, 2);
	processor.expectAddress(0);
	processor.expectAny(1);

	return nullptr;
}


*/