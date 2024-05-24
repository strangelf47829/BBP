#include "../include/ELSA.h"
#include "../include/ELSABackend.h"
#include "../include/Shell.h"
#include "Actions.h"

BBP::std::errno_t BBP::esa::esaProcessor::ExpectDelimiters(std::Lexer::lex_context *context, std::index_t argc, std::Lexer::lex_keywordhandle const argv[], bool singleLine, std::index_t min, std::index_t maxDepth, std::Lexer::lex_keywordhandle escape, std::Lexer::lex_keywordhandle stop)
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

	// Reset depth
	delimiterDepth = 0;

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
	bool hasReachedMinimum = min == 0;

	do
	{
		// Get handle to next keyword
		handle = context->moveToNextKeyword();

		// Update delimiter depth
		delimiterDepth = (delimiterDepth < depth) ? depth : delimiterDepth;

		// If handle is EOF, return error
		if (handle == context->keywordStack.atElement)
			return EEOF;

		// Get keyword
		std::Lexer::lex_keyword* keyword = &context->keywordStack[handle];

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
		if (index == lastIndex + 1)
		{
			// This delimiter is the closing delimiter to the last found delimiter.
			// Decrease the depth by 1, and retrieve the last index from stack.
			depth--;

			// If there is depth (I.E., there is still a closing delimiter needed)
			if (depth)
			{
				for (std::index_t i = 0; i < argc; i++)
				{
					if (argv[i] != delimiterStack[depth - 1])
						continue;
					lastIndex = i;
					break;
				}
			}

			// Since this must be first closer, set index here
			if (isAscending)
				firstCloser = context->atPosition - keyword->length;

			// Set flag to descending
			isAscending = false;

			// Then add the handle to the stack
			delimiterStack << handle;
			continue;
		}

		// Set last index to this
		lastIndex = index;

		// If true, this handle is for an opening keyword.
		bool isOpening = (index % 2 == 0);

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
		if (!isOpening && lastIndex > argc - 1)
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


	} while (depth);

	// If minimum has not been reached, return error
	if (!hasReachedMinimum)
		return ENOMSG; // Nothing was found, so no message.

	// If done successfully, congrats! My algorithm worked! Good job me :)
	return ENONE;
}

bool BBP::esa::esaProcessor::includeFromPath(std::FILE &file, std::PATH &filename, std::Stack<std::string_element> &dirs, std::PATH &out)
{
	// Save stack state
	std::index_t oldPointer = dirs.atElement;

	// Create char to write indicies to.
	char *path = 0;

	// If a file is found, this will be set to true.
	bool exists = false;

	// Continue for all paths
	while (dirs.atElement)
	{
		// Read next path into 'path'
		dirs >>= &path;

		// Then create path from stored string.
		std::PATH searchIn(&BBP::system::Shell::getPrimaryVolume(), path);

		// Make searchIn relative to physical root.
		searchIn.makeAbsolutePath(nullptr);

		// Create path which will eventually become the fullpath.
		std::PATH rel(filename);

		// Make the file relative to searchin.
		rel.makeAbsolutePath(&searchIn);

		if (beVerbose)
			std::printf("Searching directory: %s ... ", rel.relName());

		try
		{
			// Try to open file. If it does not exist, an exception is thrown.
			file = std::FILE(rel);

			// If this point is reached, file opened succesfully.
			exists = true;

			// Also write to out
			out = rel;

			// Print message
			if (beVerbose)
				std::printf("\e[0;32mExists\e[0;37m.\n");

			// File exists, exit from loop
			break;
		}
		catch (std::except &e)
		{
			// File does not exist
			if (beVerbose)
			{
				if (e.code)
					std::printf("\e[0;31mFile does not exist\e[0;37m\n");
				else
					std::printf("\e[0;91mError reading file:\e[0;37m %u\n", e.code);
			}
		}

	}

	// Restore stack state
	dirs.atElement = oldPointer;

	return exists;
}



BBP::std::conststring BBP::esa::esaProcessor::includeFile(std::Lexer::lex_context *context, std::index_t idx)
{

	// allocate a page for path
	std::static_string<std::max_path_length> requestedPath = std::static_string<std::max_path_length>();

	// Then write into requestedPath, the file requested
	char nul = '\0';
	for (std::index_t index = lastOpener, k = 0; index < firstCloser; index++, k++)
		__UNSAFE__(std::write)(&requestedPath, __UNSAFE__(std::read)(&context->data, index), k);
	__UNSAFE__(std::write)(&requestedPath, nul, firstCloser - lastOpener);

	// Now that we know which file we want to include, we can do this
	std::FILE file;
	std::PATH imported;
	std::PATH filename(requestedPath);

	// First try to include system path
	bool opened = includeFromPath(file, filename, context->syspaths, imported);

	// If that failed, try using user path
	if (opened == false)
		opened = includeFromPath(file, filename, context->usrpaths, imported);
	


	if (opened)
	{

		// Start including file if verbose flag is set
		if (beVerbose)
			std::printf("Including file: '%s'\n\n", imported.relName());

		// Ensure null termination. Don't know why I need to do this. Shouldn't this be done somewhere else already anyway?
		imported.fileName()[std::strlen(requestedPath)] = 0;

		// Save old file name
		std::static_string<128> oldFileName;
		std::memcpy(context->activeFile, oldFileName, 128);

		// Save old file data
		std::string oldString(context->data);
		std::size_t oldLength = context->dataSize;

		// Save debug information
		std::Lexer::lex_debugInfo oldInfo(context->debugInformation);

		// Add the included path into the usr path
		context->usrpaths <<= imported.pathName();

		// Wipe all the debug data before and after parsing
		context->resetInfo(context->debugInformation);

		// Keep track of comment and literal depth
		char commentD = context->commentDepth;
		char literalD = context->literalDepth;
		std::index_t posNow = context->atPosition;
		context->commentDepth = 0;
		context->literalDepth = 0;

		// Start parsing second file
		context->parseFile(imported);

		// Close the file since we are done with it.
		file.close();

		// Restore comment and literal depths
		context->atPosition = posNow;
		context->literalDepth = literalD;
		context->commentDepth = commentD;

		// Wipe all the debug data before and after parsing
		context->resetInfo(context->debugInformation);

		// Remove that path again. Since 'requestedPath' is not needed anymore, just get rid of it.
		context->usrpaths >>= &requestedPath.data;

		// Restore debug information
		context->debugInformation = oldInfo;

		// Restore old file data
		context->data = oldString;
		context->dataSize = oldLength;

		// Restore old file name
		std::memcpy(oldFileName, context->activeFile, 128);

		if (beVerbose)
			std::printf("File '%s' done. Returning to: '%s'.\n", imported.relName(), context->activeFile.data);

		context->resetInfo(context->debugInformation);
		return nullptr;
	}

	context->debugInformation.highlight = idx;
	context->debugInformation.highlightLength = firstCloser - idx + 1;
	context->debugInformation.highlightmsg = "NIncluded here";
	return "ENo such file or directory.";
}


BBP::std::word BBP::esa::esaProcessor::calculateArgumentOffset()
{
	std::word offset = 1;
	for (std::word index = 0; index < atArgument; index++)
		offset += instruction.args[index].trailerCount + 1;

	offset *= 4;
	return offset;
}

BBP::std::word BBP::esa::esaProcessor::calculateAbsoluteArgumentOffset()
{
	std::word offset = 1;
	for (std::word index = 0; index < atArgument; index++)
		offset += instruction.args[index].trailerCount + 1;

	offset *= 4;
	return offset + application->builder.sections[application->builder.text].stack.atElement;
}