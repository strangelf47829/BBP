#include "../include/Lex.h"
#include "../include/stdio.h"
#include "../include/FileSys.h"

void BBP::std::Lexer::lex_context::translateUnit(std::PATH &file)
{
	// Reset user paths
	usrpaths.atElement = 0;
	usrpaths <<= file.pathName();
	usrpaths <<= workingDirectory->relName();

	// Parse the source file.
	parseFile(file);

	// Check if ill-formed
	if (illformed)
	{
		debugInformation.showLinesAndColoumns = false;
		Log(file.relName(), "ETranslation unit failed to compile.", nullptr);
		return;
	}

}

void BBP::std::Lexer::lex_context::parseFile(std::PATH &file)
{
	// Read the file
	std::FILE source(file);

	// Read the data from the file
	data = *source.b().page;

	// Calculate data size
	dataSize = std::seqlen(data);

	// Set string
	std::c_string name = file.fileName();
	strcpy(&activeFile, name);

	// Reset flags
	commentDepth = 0;
	literalDepth = 0;
	atPosition = 0;

	// Parse everything
	parseFile();

	// Close file
	source.close();

	// Clear file data
	data = PAGE<string_element>(0, nullptr);

	// Reset file name
	strcpy(&activeFile, "");
}

void BBP::std::Lexer::lex_context::executeKeywordAction(Lexer::lex_keywordhandle handle, Lexer::lex_keyword *&keyword)
{
	// If keyword is nullptr, do nothing
	if (keyword == nullptr)
		return;

	// Otherwise, if action exists, execute it.
	if (keyword->action)
	{
		// Call action
		debugInformation.msg = keyword->action(this, atPosition - keyword->length, handle);

		// if msg exists, log error
		if (debugInformation.msg)
		{
			Log(debugInformation);
			resetInfo(debugInformation);
		}

	}
}

// 
BBP::std::Lexer::lex_keywordhandle BBP::std::Lexer::lex_context::moveToNextKeyword()
{

	lex_keywordhandle handle = 0;
	size_t kwCount = keywordStack.atElement;

	// These flags keep track of if any non-whitespace sequence had any meaning.
	bool hasFoundMeaningfulToken = false;
	std::index_t foundAt = 0;

	// Loop until something is found
	while (atPosition < dataSize && !abort)
	{
		// Get character
		char character = std::read(&data, atPosition++);
		debugInformation.coloumn++;


		if (isWhitespace(character))
		{
			// If the lexer has previously found meaningful text but now sees a whitespace, do some stuff.
			// Only do this when not intentionally skipping over meaningful tokens (such as comments, literals).
			if (hasFoundMeaningfulToken && !commentDepth && !literalDepth)
			{
				// Call action
				executeSpecialFunction(foundAt, atPosition - 1);

				// Reset flag 
				hasFoundMeaningfulToken = false;

				// If need to abort, break out.
if (abort)
return keywordStack.atElement + 1;
			}
		}
		else
		{
		if (!hasFoundMeaningfulToken)
			foundAt = atPosition - 1;
		hasFoundMeaningfulToken = true;
		}

		if (character == '\n')
		{
			debugInformation.coloumn = 0;
			debugInformation.line++;
		}

		// Mask each keyword word
		for (handle = 0; handle < kwCount; handle++)
		{

			// Get a handle on the keyword
			lex_keyword *keyword = &read(&keywords, handle);

			// If inside a literal context, and the word is not forced to be evalated,
			if ((literalDepth && !keyword->forceParseOutsideLiterals) && !keyword->forceEvaluation)
			{
				// Continue only if keyword's mask equals literal depth
				if (literalDepth != keyword->considerLiteralDepth && !keyword->forceParseInLiterals)
					continue;
			}

			// If inside a comment context, and the word is not forced to be evalated,
			if ((commentDepth && !keyword->forceParseOutsideComments) && !keyword->forceEvaluation)
			{
				// Continue only if keyword's mask equals comment depth
				if (commentDepth != keyword->considerCommentDepth && !keyword->forceParseInComments)
					continue;
			}

			// Operate on keywords mask based on current character
			keyword->mask +=
				(character == keyword->word[keyword->mask]) ?
				1 :
				-keyword->mask;

			// Check if we are in white space, if so, reset flag and stop searching, since it is no use.
			// This must occur after mask code, since otherwise "whi le" would register as "while", unless
			// the keyword consists of whitespaces.
			if (isWhitespace(character))
			{

				lastWhiteSpace = atPosition;

				// Check each character in the word
				for (index_t idx = 0; idx < keyword->length; idx++)
					if (!isWhitespace(keyword->word[idx]))
						continue; // If any of these are not whitespaces, skip
			}

			// If the mask is not the same as the length of the keyword, the keyword is not found	
			if (keyword->mask != keyword->length)
				continue;

			// If keyword needs to be treated like a delimiter, do that here.
			if (keyword->actAsDelimiter)
			{
				// Check for any prefixed things here.
				if (atPosition - lastWhiteSpace > keyword->length && !literalDepth && !commentDepth)
				{
					// Call action
					executeSpecialFunction(lastWhiteSpace, atPosition - keyword->length);

					// If need to abort, break out.
					if (abort)
						return keywordStack.atElement + 1;
				}

				lastWhiteSpace = atPosition;
			}

			// Something was found anyway.
			hasFoundMeaningfulToken = false;

			// Check if keyword is infixed. Delimiters can, however, be infixed.
			if (atPosition - lastWhiteSpace > keyword->length && !keyword->actAsDelimiter)
				continue;

			// Check if word is prefixed if it dissallows prefixing. A word is not considered prefixed if it as the end of the file.
			if (atPosition >= data.dataSize == false)
			{
				// Get next character
				std::string_element nextChar = std::read(&data, atPosition);

				// If it doesn't allow prefixing, nor is the next character a whitespace, continue looking.
				if ((isWhitespace(nextChar) || keyword->allowsPrefixing) == false)
				{
					keyword->mask = 0;
					continue;
				}

			}

			// Reset mask, since keywords may come after eachother.
			keyword->mask = 0;

			// Keyword found, return that.
			return handle;
		}
	}

	// Nothing was found, return invalid handle
	return kwCount;

}

void BBP::std::Lexer::lex_context::executeSpecialFunction(std::index_t from, std::index_t to)
{
	// If special functions are inhibited, do nothing
	if (inhibitSpecialFunctions)
		return;

	// Check if it is number
	{
		// Check for number
		bool isFloat = false;
		int iresult = 0;
		float fresult = 0;

		// Try to parse
		bool isNumber = tryParseNumerical(from, to, isFloat, fresult, iresult);

		// If a number, run and debug.
		if (isNumber)
		{
			// Build numerical
			numerical_t number;

			if (isFloat)
				number.f = fresult;
			else
				number.i = iresult;

			// Call action
			debugInformation.msg = onNumericalLiteral(number, isFloat);

			// if msg exists, log error
			if (debugInformation.msg)
			{
				Log(debugInformation);
				resetInfo(debugInformation);
			}
			return;
		}
		else if (isNumber)
		{
			Log("ELSA", "WNumerical found, but no action is defined.", "-Wmissing-function");
			return;
		}
	}

	// Nothing found, call unkown
	// Call action
	debugInformation.msg = onUnkown(from, to);

	// if msg exists, log error
	if (debugInformation.msg)
	{
		Log(debugInformation);
		resetInfo(debugInformation);
	}
	return;

	Log("ELSA", "WUnkown sequence found, but no action is defined.", "-Wmissing-function");
}

void BBP::std::Lexer::lex_context::parseFile()
{
	// Reset debug information
	debugInformation.line = 1;

	// While within bounds...
	while (atPosition < dataSize)
	{
		// Calculate the amount of keywords.
		size_t kwCount = keywordStack.atElement;

		// Find the next keyword
		lex_keywordhandle handle = moveToNextKeyword();

		if (abort)
		{
			Log(activeFile.data, "ECompilation terminated.", nullptr);
			return;
		}

		// If no keyword is found, exit out.
		if (handle == kwCount)
			break;

		// Otherwise, get keyword pointer
		lex_keyword *keyword = &__UNSAFE__(read)(&keywords, handle);

		// Then execute the keyword action
		executeKeywordAction(handle, keyword);

		if (abort)
		{
			Log(activeFile.data, "ECompilation terminated.", nullptr);
			return;
		}

	}

	// Do final checks
	if (literalDepth)
		Log("lexer", "EExpected literal termination before 'EOF'", nullptr);
	if (commentDepth)
		Log("lexer", "EExpected comment termination before 'EOF'", nullptr);

	literalDepth = 0;
	commentDepth = 0;
}

bool BBP::std::Lexer::lex_context::isWhitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == 1 || c == 2);
}

bool BBP::std::Lexer::lex_context::isNewLine(char c)
{
	return (c == '\n' || c == '\r');
}

char BBP::std::Lexer::lex_context::isBinaryNumeral(char c)
{
	switch (c)
	{
	case '0':
		return 10;
	case '1':
		return 11;
	default:
		return 0;
	}
}

char BBP::std::Lexer::lex_context::isOctalNumeral(char c)
{
	switch (c)
	{
	case '0':
		return 10;
	case '1':
		return 11;
	case '2':
		return 12;
	case '3':
		return 13;
	case '4':
		return 14;
	case '5':
		return 15;
	case '6':
		return 16;
	case '7':
		return 17;
	default:
		return 0;
	}
}

char BBP::std::Lexer::lex_context::isDecimalNumeral(char c)
{
	switch (c)
	{
	case '0':
		return 10;
	case '1':
		return 11;
	case '2':
		return 12;
	case '3':
		return 13;
	case '4':
		return 14;
	case '5':
		return 15;
	case '6':
		return 16;
	case '7':
		return 17;
	case '8':
		return 18;
	case '9':
		return 19;
	default:
		return 0;
	}
}

char BBP::std::Lexer::lex_context::isHexadecimalNumeral(char c)
{
	switch (c)
	{
	case '0':
		return 10;
	case '1':
		return 11;
	case '2':
		return 12;
	case '3':
		return 13;
	case '4':
		return 14;
	case '5':
		return 15;
	case '6':
		return 16;
	case '7':
		return 17;
	case '8':
		return 18;
	case '9':
		return 19;
	case 'A':
	case 'a':
		return 20;
	case 'B':
	case 'b':
		return 21;
	case 'C':
	case 'c':
		return 22;
	case 'D':
	case 'd':
		return 23;
	case 'E':
	case 'e':
		return 24;
	case 'F':
	case 'f':
		return 25;
	default:
		return 0;
	}
}

char BBP::std::Lexer::lex_context::isNumericalBase(std::string_element c, std::byte &base)
{
	switch (c)
	{
	case 'b':
		base = 2;
		return 30;
	case 'o':
		base = 8;
		return 31;
	case 'f':
		base = 10;
		return 32;
	case 'x':
		base = 16;
		return 33;
	default:
		base = 0;
		return 0;
	}
}

bool BBP::std::Lexer::lex_context::tryParseNumerical(std::index_t from, std::index_t to, bool &isFloat, float &fresult, int &iresult)
{
	// Define some variables to idk, figure it out goddamn 
	std::byte base; 
	std::string_element firstCharacter = std::read(&data, from);
	
	// If first character is '-', outcome is opposite signed
	int sign = 1;
	while (firstCharacter == '-')
	{
		// Check if 'from' isn't to. (I.E., '---------')
		if (from >= to)
			return false;

		// advance one character and save result in firstCharacter
		firstCharacter = std::read(&data, ++from);

		// Flip sign
		sign *= -1;
	}
	
	// Check if has prefix
	bool hasPrefix = (isNumericalBase(firstCharacter, base) != 0);

	// If base is 0, it means the first character is not a valid numerical character.
	if (base == 0 && !isDecimalNumeral(firstCharacter))
		return false;
	else if (base == 0)
		base = 10;

	// This is used for the float
	float divideBy = 1;

	// Loop over each character now
	for (std::index_t idx = from + hasPrefix; idx < to; idx++)
	{
		// Fetch a character
		std::string_element c = std::read(&data, idx);

		// If the character is a dot, switch over to float mode.
		if (c == '.' && !isFloat)
		{
			isFloat = true;
			continue;
		}
		else if (c == '.')
			return false; // Since isFloat must be true, a return value of zero still gives a unique return value.
		// TODO: Explain wtf that means.

		// If character is not numerical number based on base type, return
		std::byte number = 0;
		switch (base)
		{
		case 2:
			number = isBinaryNumeral(c);
			break;
		case 8:
			number = isOctalNumeral(c);
			break;
		case 10:
			number = isDecimalNumeral(c);
			break;
		case 16:
			number = isHexadecimalNumeral(c);
			break;
		}

		// If not a number, return false.
		if (number == 0)
			return false;

		// If not a float, increment both accordingly
		if (!isFloat)
		{
			iresult = (iresult * base) + (number - 10);
			fresult = (fresult * base) + (number - 10);
			divideBy *= base;
		}
		else
		{
			fresult += (number - 10) / divideBy;
			divideBy *= base;
		}
	}

	iresult *= sign;
	fresult *= sign;

	// Done
	return true;
}


// Used to move the lexer up and down
void BBP::std::Lexer::lex_context::moveToEndOfLastWord(bool stopAtNewLine)
{
	// Move until start of this word
	moveToStartOfThisWord(stopAtNewLine);

	// While possible, move back one character until no whitespace is found
	while (--atPosition < dataSize)
	{
		char c = __UNSAFE__(read)(&data, atPosition);
		if (!isWhitespace(c) || (isNewLine(c) && stopAtNewLine))
		{
			if (isNewLine(c))
				atPosition++;
			break;
		}
	}
}
void BBP::std::Lexer::lex_context::moveToStartOfLastWord(bool stopAtNewLine)
{
	moveToEndOfLastWord(stopAtNewLine);
	moveToStartOfThisWord(stopAtNewLine);
}

void BBP::std::Lexer::lex_context::moveToEndOfThisWord(bool stopAtNewLine)
{
	// While possible, move forward one character until whitespace is found
	while (++atPosition < dataSize)
	{
		char c = __UNSAFE__(read)(&data, atPosition);
		if (isWhitespace(c))
			break;
	}
	atPosition--;
}
void BBP::std::Lexer::lex_context::moveToStartOfThisWord(bool stopAtNewLine)
{
	// While possible, move backward one character until whitespace is found
	while (--atPosition < dataSize)
		if (isWhitespace(__UNSAFE__(read)(&data, atPosition)))
			break;
	atPosition++;
}

void BBP::std::Lexer::lex_context::moveToEndOfNextWord(bool stopAtNewLine)
{
	moveToStartOfNextWord(stopAtNewLine);
	moveToEndOfThisWord(stopAtNewLine);
}
void BBP::std::Lexer::lex_context::moveToStartOfNextWord(bool stopAtNewLine)
{
	moveToEndOfThisWord(stopAtNewLine);

	// While possible, move forward one character until no whitespace is found
	while (++atPosition < dataSize)
	{
		char c = __UNSAFE__(read)(&data, atPosition);
		if (!isWhitespace(c) || (isNewLine(c) && stopAtNewLine))
			break;
	}
}

bool BBP::std::Lexer::lex_context::moveToAndHashNextWord(std::hash_t &hash, char stop)
{
	// Returns true when it hashed anything, false otherwise.
	// Move back one position
	atPosition--;

	// Move to start of next word
	moveToStartOfNextWord(true);

	return moveToAndHashThisWord(hash, stop);
}

bool BBP::std::Lexer::lex_context::moveToAndHashThisWord(std::hash_t &hash, char stop)
{

	// Check if character is newline
	if (isNewLine(__UNSAFE__(std::read)(&data, atPosition)) || atPosition + 1 >= dataSize)
		return false;

	// Calculate size of macro, hash, and index
	std::word Power = 1;
	std::index_t idx = 0;
	bool isOnlyWhitespace = true;

	// Now calculate running hash
	for (; idx < dataSize; idx++)
	{
		// Get character at index
		std::string_element c = __UNSAFE__(std::read)(&data, atPosition + idx);

		// Keep isOnlyWhiteSpace updated
		isOnlyWhitespace &= (isWhitespace(c) || c == 0);

		// If asked to stop here, we stop. We also stop for whitespaces.
		if (c == stop || (!isOnlyWhitespace && isWhitespace(c) && stop == '\n'))
			break;

		// If we are at end character, error.
		if (c == 0)
			return false;

		// Run the hashing algorithm
		std::strhsh(hash, Power, c);
	}

	// Move to end here
	moveToEndOfThisWord(true);


	// If macro is only whitespace, there isn't really anything to do.
	if (isOnlyWhitespace)
		return false;
	return true;
}

void BBP::std::Lexer::lex_context::calculateDebugPosition()
{
	// TODO: You are telling me that this is the most inefficient part of the whole program? O(N^2) worst case? Really?

	// To store position
	std::index_t pos = 0;

	// Reset debug info
	debugInformation.coloumn = 0;
	debugInformation.line = 1;

	if (activeFile.data[0] == '\0')
		return;

	// Catch up to where we are, counting the newlines and stuff
	while (pos < atPosition && pos < dataSize)
	{
		char c = __UNSAFE__(read)(&data, pos++);
		
		if (isNewLine(c))
		{
			debugInformation.line++;
			debugInformation.coloumn = 0;
		}
		else
		{
			debugInformation.coloumn++;
		}

	}

}

// DEBUG AIDS (as in, objects of aid to help with debugging)

void BBP::std::Lexer::lex_context::resetInfo(std::Lexer::lex_debugInfo &info)
{
	info.showLinesAndColoumns = true;
	info.showWhere = true;
	info.highlightmsg = nullptr;
	info.highlight = 0;
	info.errName = nullptr;
	info.highlightLength = 0;
}

void BBP::std::Lexer::lex_context::LogError(conststring from, conststring msg, conststring name)
{
	if (from)
		std::STDOUT << "\e[0;31m" << from << "\e[0;37m: " << msg;
	else
		std::STDOUT << "\e[0;31merror\e[0;37m: " << msg;
	if (name)
		std::STDOUT << " [\e[0;31m" << name << "\e[0;37m]" <<= endl;
	else
		std::STDOUT <<= endl;
}

void BBP::std::Lexer::lex_context::LogNote(conststring from, conststring msg, conststring name)
{
	if (from)
		std::STDOUT << "\e[0;36m" << from << "\e[0;37m: " << msg;
	else
		std::STDOUT << "\e[0;96mnote\e[0;37m: " << msg;
	if (name)
		std::STDOUT << " [\e[0;36m" << name << "\e[0;37m]" <<= endl;
	else
		std::STDOUT <<= endl;
}

void BBP::std::Lexer::lex_context::LogWarning(conststring from, conststring msg, conststring name)
{
	if (from)
		std::STDOUT << "\e[0;35m" << from << "\e[0;37m: " << msg;
	else
		std::STDOUT << "\e[0;35mwarning\e[0;37m: " << msg;
	if (name)
		std::STDOUT << " [\e[0;35m" << name << "\e[0;37m]" <<= endl;
	else
		std::STDOUT <<= endl;
}

void BBP::std::Lexer::lex_context::LogAbort(conststring from, conststring msg, conststring name)
{
	if (from)
		std::STDOUT << "\e[0;30;101m" << from << "\e[0;37m: " << msg;
	else
		std::STDOUT << "\e[0;30;101mfatal error\e[0;37m: " << msg;
	if (name)
		std::STDOUT << " [\e[0;30;101m" << name << "\e[0;37m]" <<= endl;
	else
		std::STDOUT <<= endl;
}

void BBP::std::Lexer::lex_context::Log(conststring msg)
{
	Log(nullptr, msg, nullptr);
}

void BBP::std::Lexer::lex_context::Log(conststring from, conststring msg, conststring name)
{
	// nullptr msg = disabled message
	if (msg == nullptr)
	{
		return;
	}

	// Reset debug position
	calculateDebugPosition();

	char c = msg[0];
	switch (c)
	{
	case 'e':
		abort = true;
	case 'E':
		illformed = true;
		LogError(from, msg + 1, name);
		break;
	case 'N':
		LogNote(from, msg + 1, name);
		break;
	case 'W':
		LogWarning(from, msg + 1, name);
		break;
	case 'A': // Urgent abort
		abort = true;
		illformed = true;
		LogAbort(from, msg + 1, name);
		break;
	default:
		abort = true;
		printf("(bad message: expected format character)\n");
		break;
	
	}

}

void BBP::std::Lexer::lex_context::Log(lex_debugInfo &info)
{

	

	if (info.showWhere)
	{
		// Reset debug position
		if (activeFile.data[0] != '\0')
			calculateDebugPosition();

		if (info.procedure.data)
		{
			if (info.procedure.data[0] != '\0')
			{
				if (activeFile.data[0] == '\0')
					std::printf("(<Binary>): In procedure: `\e[0;97m%s\e[0;37m`:\n", info.procedure.data);
				else
					std::printf("\e[0;97m%s:\e[0;37m In procedure: `\e[0;97m%s\e[0;37m`:\n", activeFile.data, info.procedure.data);
			}
		}

		if (info.showLinesAndColoumns)
		{
			if (activeFile.data[0] == '\0')
				std::putf(std::STDOUT, "(<Binary>:%u:%u): ", info.coloumn, info.line);
			else
				std::putf(std::STDOUT, "\e[0;97m%s:%u:%u:\e[0;37m ", activeFile.data, info.coloumn, info.line);
		}
		else
		{
			if (activeFile.data[0] == '\0')
				std::putf(std::STDOUT, "(<Binary>~%4u): ", info.character);
			else
				std::putf(std::STDOUT, "\e[0;97m%s~%4u:\e[0;37m ", activeFile.data, info.character);
		}
	}

	Log(nullptr, info.msg, info.errName);
	

	if (info.highlightmsg)
	{
		if (info.showWhere)
		{
			if (info.showLinesAndColoumns)
			{
				if (activeFile.data[0] == '\0')
					std::putf(std::STDOUT, "(<Binary>:%u:%u): ", info.coloumn, info.line);
				else
					std::putf(std::STDOUT, "(%s:%u:%u): ", activeFile.data, info.coloumn, info.line);
			}
			else
			{
				if (activeFile.data[0] == '\0')
					std::putf(std::STDOUT, "(<Binary>~%4u): ", info.character);
				else
					std::putf(std::STDOUT, "(%s~%4u): ", activeFile.data, info.character);
			}
		}
		Log(info.highlightmsg);
		Highlight(info);
	}

	std::putf(std::STDOUT, "\n");

}

void BBP::std::Lexer::lex_context::Highlight(lex_debugInfo info)
{

	// This pointer traverses the highlighted text
	std::index_t highlitingText = info.highlight;

	bool isSingleLine = true;
	unsigned int length = 0;
	unsigned int linelength = 0;
	unsigned int runningLength = 0;

	std::index_t line = info.line;
	std::index_t character = info.character;
	std::index_t coloumn = info.coloumn;

	char c = 0;

	// Continue highlighting until end of string
	while ((std::read(&this->data, highlitingText) && !info.highlightLength) || (info.highlightLength && runningLength < info.highlightLength))
	{
		// Print location information
		if (info.showWhere)
		{
			if (info.showLinesAndColoumns)
				std::putf(std::STDOUT, "%5u |\t\e[0;96m", line);
			else
				std::putf(std::STDOUT, "%4u~ |\t\e[0;96m", character);
		}
		// Continue highlighting until end of string
		while ((!info.highlightLength) || (info.highlightLength && runningLength < info.highlightLength))
		{

			// Get character to be highlighted
			c = std::read(&this->data, highlitingText++);

			// If character is null, break
			if (c == 0)
				goto end; // break out of nested whiles

			character++;
			coloumn++;
			linelength++;
			runningLength++;

			// if character is newline, continue
			if (isNewLine(c))
			{
				// If c is not '\n', just continue
				if (c != '\n')
					continue;
				// Else, increment line and coloumn info
				line += 1;
				character = 0;
				std::STDOUT << "\e[0;37m" <<= endl;
				isSingleLine = false;
				length = (length < linelength) ? linelength - 1 : length;
				linelength = 0;
				break;
			}

			// Print character to screen
			std::STDOUT << c;
			length = (length < linelength) ? linelength : length;

		}

	}

	end:

	// Emit last new line
	if (c != '\n')
		std::STDOUT << "\e[0;37m\n      | \e[0;96m";
	else if (runningLength > 1)
		std::STDOUT << "\e[0;37m      | \e[0;96m";

	for (unsigned int index = 0; index < length; index++)
	{
		if (isSingleLine)
			std::STDOUT << "^";
		else
			std::STDOUT << "~";
	}
	std::STDOUT << "\e[0;37m" <<= std::endl;
	

}



BBP::std::Lexer::lex_context::lex_context()
	: keywordStack(&keywords, 64),
	usrpaths(&usrpathPage, usrpathPage.dataSize),
	syspaths(&syspathPage, syspathPage.dataSize),
	abort(false),
	atPosition(0),
	commentDepth(0),
	literalDepth(0),
	dataSize(0),
	lastWhiteSpace(0),
	inhibitSpecialFunctions(false)
{

}