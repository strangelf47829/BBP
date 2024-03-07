#include "../include/ELSA.h"
#include "../include/ELSABackend.h"
#include "Actions.h"


BBP::std::conststring BBP::esa::esaProcessor::numerical(BBP::std::Lexer::numerical_t numerical, bool isFloat)
{
	// If line is defining whatever, don't do anything
	if (lineIsDefiningProcedure)
		return nullptr;

	return addNumerical(numerical, isFloat);
}

BBP::std::conststring BBP::esa::esaProcessor::numericalArgument(BBP::std::Lexer::numerical_t numerical, bool isFloat)
{

	// Check if correct thing is expected
	bool expectsLiteral = instruction.args[atArgument].isLiteralArg || instruction.args[atArgument].anyvalue;

	// Now set to non-literal
	expectLiteral(atArgument);

	// Get intermediate and write
	std::word intermediate = isFloat ? (*((std::word*)&numerical.f)) : numerical.i;
	instruction.args[atArgument].data = intermediate;

	// Move up argument
	atArgument++;

	// If literal was not expected, return warning
	if (expectsLiteral)
		return nullptr;

	esaParser.debugInformation.errName = "-WUnexpected-arg-type";
	return "WDid not expect supplied argument to be numerical literal.";
}

BBP::std::conststring BBP::esa::esaProcessor::unkown(BBP::std::index_t at, BBP::std::index_t lastspace)
{
	esaParser.debugInformation.highlight = lastspace;
	esaParser.debugInformation.highlightLength = at - lastspace;
	esaParser.debugInformation.highlightmsg = "NHere";

	return "EUnrecognized character sequence.";
}

BBP::std::index_t BBP::esa::esaProcessor::lookupSymbol(std::index_t from, std::index_t to, std::word *byteSize, std::hash_t &hash)
{
	// Calculate hash
	for (std::index_t index = from; index < to; index++)
	{
		// Get character
		std::string_element c = std::read(&esaParser.data, index);

		// Operate on character
		hash = (hash * 33) + c;
	}

	// Create page for 32 bit sized values
	std::PAGE<std::word> hashTable(2 + application->builder.hashBucketCount + application->builder.hashChainCount, (std::word *)application->builder.sections[application->builder.hashtab].data.data);

	// Get bucket
	std::word bucket = hash % application->builder.hashBucketCount;

	// Check what is at bucket
	std::word bucketValue = std::read(&hashTable, 2 + bucket);

	// If value is 0, return invalid
	if (bucketValue == 0)
	{
		return 0;
	}
	else
	{
		// Move elsewhere later, idc right now.
		// Walk through each symbol table
		char *c = &application->builder._strtab().stack[application->builder.symbols[bucketValue].name()] + 2;

		// Get string size
		std::size_t nameSize = 0;
		std::word radix = 1;

		for (; *c; c++)
		{
			// Get numerical
			std::byte numeral = esaParser.isDecimalNumeral(*c);

			if (numeral == 0)
				break;

			// Update length
			nameSize += (numeral - 10) * radix;
			radix *= 10;
		}

		for (std::index_t character = 0; character < nameSize; character++)
		{
			// Get each character to be able to check later
			std::string_element c1 = std::read(&esaParser.data, from + character);
			std::string_element c2 = *(c + character);

			// Compare characters
			if (c1 != c2)
				goto start_chain;

		}

		// Get bytesize
		c += nameSize + 1;
		*byteSize = 0;
		radix = 1;

		for (; *c; c++)
		{
			// Get numerical
			std::byte numeral = esaParser.isDecimalNumeral(*c);

			if (numeral == 0)
				break;

			// Update length
			*byteSize += (numeral - 10) * radix;
			radix *= 10;
		}

		return bucketValue;

	start_chain:
		radix = radix;
	}

	// Read chain value
	for (std::index_t chainIndex = std::read(&hashTable, 2 + bucket); chainIndex < application->builder.hashChainCount; chainIndex++)
	{
		// Read value at chain
		std::word chainValue = std::read(&hashTable, 2 + application->builder.hashBucketCount + chainIndex);

		// If chain has no value, return invalid
		if (chainValue == 0)
			return 0;

		// Walk through each symbol table
		char *c = &application->builder._strtab().stack[application->builder.symbols[chainValue].name()] + 2;

		// Get string size
		std::size_t nameSize = 0;
		std::word radix = 1;

		for (; *c; c++)
		{
			// Get numerical
			std::byte numeral = esaParser.isDecimalNumeral(*c);

			if (numeral == 0)
				break;

			// Update length
			nameSize += (numeral - 10) * radix;
			radix *= 10;
		}

		for (std::index_t character = 0; character < nameSize; character++)
		{
			// Get each character to be able to check later
			std::string_element c1 = std::read(&esaParser.data, from + character);
			std::string_element c2 = *(c + character);

			// Compare characters
			if (c1 != c2)
				goto next_chain;

		}

		// Get bytesize
		c += nameSize + 1;
		*byteSize = 0;
		radix = 1;

		for (; *c; c++)
		{
			// Get numerical
			std::byte numeral = esaParser.isDecimalNumeral(*c);

			if (numeral == 0)
				break;

			// Update length
			*byteSize += (numeral - 10) * radix;
			radix *= 10;
		}

		return chainValue;
	next_chain:
		continue;
	}




	return 0;
}

BBP::std::conststring BBP::esa::esaProcessor::resolve(BBP::std::index_t at, BBP::std::index_t lastspace)
{
	// Since file names can appear between '<>' or '""' (but not both), create the following handle
	std::Lexer::lex_keywordhandle handles[] = { OPENANGLEBRACKET_HANDLE, CLOSEANGLEBRACKET_HANDLE, OPENCURLBRACKET_HANDLE, CLOSECURLBRACKET_HANDLE, OPENSQBRACKET_HANDLE, CLOSESQBRACKET_HANDLE };

	// Store position information
	std::index_t position = esaParser.atPosition;

	// Move to beginning of word
	esaParser.atPosition--;
	esaParser.moveToStartOfThisWord(true);
	esaParser.atPosition--;

	// Now save start
	std::index_t start = esaParser.atPosition++;

	// Get delimiters
	esaParser.inhibitSpecialFunctions = true;
	std::errno_t error = ExpectDelimiters(&esaParser, 6, handles, true, 0, 32, BACKSLASH_HANDLE, COMMA_HANDLE);
	esaParser.inhibitSpecialFunctions = false;

	// Restore position
	esaParser.atPosition = position;

	switch (error)
	{
	case EFAULT: // Somehow a nullpointer got here. Abort immediately.
		atArgument++;
		return "AEFAULT";
	case EDOM: // Somehow, an odd number ended up as argc. Either programmer fault, or stack error. Abort immediately.
		atArgument++;
		return "AEDOM";
	case EEOF: // Found End Of File before anything was found.
		atArgument++;
		return "EEOF";
	case ENOMSG:// Found End of Line before anything was found.
		atArgument++;
		return "ENOMSG";
	case ELINEEND: // End of Line found before second delimiter
		atArgument++;
		return "ELINEEND";
	case ENOANO: // Attempted to close delimiter before starting it.
		atArgument++;
		return "EENOANO";
	case EILSEQ: // Mismatched delimiters
		atArgument++;
		return "EILSEQ";
	case EUNFOCS: // Somehow unfocused?
		atArgument++;
		return "AUNFOCS";
	case EOVERFLOW: // More than one delimiter, can only really happen with <
		atArgument++;
		return "EOVERFLOW";
	}

	std::halfword referenceDepth = 0;
	std::halfword dereferenceDepth = 0;
	std::word sizeDepth = 0;

	// Parse out delimiters
	for (std::index_t i = 0; i < delimiterDepth; i++)
	{
		switch (delimiterStack[i])
		{
		case OPENSQBRACKET_HANDLE:
			dereferenceDepth++;
			break;
		case OPENCURLBRACKET_HANDLE:
			referenceDepth++;
			break;
		case OPENANGLEBRACKET_HANDLE:
			sizeDepth++;

		}
	}

	// Check if the request is legal
	if (sizeDepth && (referenceDepth || dereferenceDepth))
		return "ECannot do that";

	// Look for symbol
	std::hash_t hash = 0;
	std::word byteSize = 0;
	std::index_t symbol = lookupSymbol(delimiterDepth ? lastOpener : (start + 1), delimiterDepth ? firstCloser : (position - 1), &byteSize, hash);

	// Now reset delimiterDepth and delimiterStack
	delimiterDepth = 0;
	delimiterStack.atElement = 0;

	// If symbol is 0, try to resolve quasi or reserved.
	if (symbol == 0)
	{

		// Check if value is quasi symbol
		for (std::index_t quasiSymbol = 0; quasiSymbol < quasiSymbols.dataSize; quasiSymbol++)
		{
			// Get value
			std::hash_t quasiHash = std::read(&quasiSymbols, quasiSymbol);

			// If hash is not equal to hash, skip this part
			if (quasiHash != hash)
				continue;

			// Symbol is quasi symbol. Get value
			std::word quasiValue = std::read(&quasiSymbolValues, quasiSymbol);

			// Check if the user is trying to reference or dereference symbol
			if (referenceDepth || dereferenceDepth)
			{
				esaParser.debugInformation.errName = nullptr;
				return "ECannot reference or dereference quasi symbol";
			}

			// Check if the user is trying to obtain size info
			if (sizeDepth)
			{
				// Check if size depth is higher than 2
				if (sizeDepth > 2)
				{
					esaParser.debugInformation.errName = nullptr;
					return "EInvalid size request: Must be '<<>>' or '<>'.";
				}

				if (sizeDepth == 1)
					return numericalArgument({ 4 }, false);
				return numericalArgument({ 32 }, false);

			}

			return numericalArgument({ quasiValue }, false);
		}

		for (std::index_t reservedIndex = 0; reservedIndex < reservedIdentifiers.dataSize; reservedIndex++)
		{
			// Get value
			std::hash_t reservedHash = std::read(&reservedIdentifiers, reservedIndex);

			// Skip if not what we are looking for
			if (reservedHash != hash)
				continue;

			// Check if depth requested
			if (sizeDepth)
			{
				// Save numerical error based on thingy
				std::conststring numericalError = nullptr;
				if (sizeDepth == 2)
					numericalError = numericalArgument({ 4 }, false);
				else if (sizeDepth == 1)
					numericalError = numericalArgument({ 32 }, false);
				else
					return "EInvalid size request: Must be '<<>>' or '<>'.";

				esaParser.debugInformation.errName = "-WUnsupported-size-collapse";
				return "WSize request is not yet supported for registers. Defaulting to 4 bytes.";
			}

			// Get argument
			userspace::Instruction::Arg argument = std::read(&reservedIdentifiersValues, reservedIndex);

			// Check if referenced
			if (referenceDepth == 1)
				argument.referenced = true;
			else if (referenceDepth)
				return "ECannot reference a reference.";
			else
				argument.trailers[0] += dereferenceDepth;

			// Now emit data
			return setArg(argument);
		}

		esaParser.resetInfo(esaParser.debugInformation);
		esaParser.debugInformation.highlight = start + 1;
		esaParser.debugInformation.highlightLength = position - start - 2;
		esaParser.debugInformation.highlightmsg = "NHere:";
		esaParser.calculateDebugPosition();

		atArgument++;
		return "ELinker error: Cannot find reference for a given identifier.";
	}

	std::address_t value = application->builder.symbols[symbol].value();
	std::word atPage = application->builder.symbols[symbol].shndx();

	// If looking for size
	if (sizeDepth)
	{
		// Check if symbol is extern
		if (atPage == 0)
			return "ECannot resolve sizes for extern symbols.";

		// Check if request is legal
		if (sizeDepth > 2)
			return "ECannot resolve more than 2 size layers.";

		// Do an action based on the amount of brackets
		std::conststring errMsg = nullptr;
		if (sizeDepth == 1)
			errMsg = numericalArgument({ byteSize }, false);
		else
			errMsg = numericalArgument({ byteSize * 8 }, false);

		// Return error message
		return errMsg;
	}

	// Check if reference was expected
	bool expectedReference = !instruction.args[atArgument].isLiteralArg || instruction.args[atArgument].anyvalue;
	esaParser.debugInformation.errName = "-WUnexpected-arg-type";

	// Get total referenced
	BBP::int32_t totalRefDepth = dereferenceDepth - referenceDepth;


	// Check if page is 0
	// TODO: Move to different function
	if (atPage == 0 || true)
		emitRelocation(symbol);

	// Check if request is legal
	if (totalRefDepth < -1)
		return "ECannot reference a value more than once.";

	// If totalRefDepth is -1, emit symbol
	if (totalRefDepth == -1)
		return setReference(value, atPage);
	
	// Otherwise, just emit dereferenced.
	return setDereference(value, atPage, totalRefDepth + 1);
}