#include "Actions.h"
#include "../include/ELSA.h"

BBP::esa::Frontend BBP::esa::frontend;


bool BBP::esa::lineIsDefiningSymbolDefinition = false;

BBP::std::conststring BBP::esa::Frontend::markConst()
{
	// If the size for this symbol has already been defined, return error
	if (symbol.subtype & 0b111111100000)
		return "EIllegal 'const' after size definition";
	// If the symbol has already been hashed, return error
	if (symbol.hash)
		return "E'Const' modifier must come before identifier.";

	// Check that this is not the access type
	if (!lineIsDefiningSymbolDefinition)
		return "eModifier 'const' is not a valid access type.";

	// Check that symbol isn't already marked const
	if (symbol.subtype & SYMBOL_TYPE::SYMTMOD_CONST)
		return "eDuplicate 'const' modifier.";

	symbol.subtype |= SYMBOL_TYPE::SYMTMOD_CONST;

	return nullptr;
}

BBP::std::conststring BBP::esa::Frontend::markVirtual()
{
	// If the size for this symbol has already been defined, return error
	if (symbol.subtype & 0b111111100000)
		return "EIllegal 'virtual' after size definition";

	// If the symbol has already been hashed, return error
	if (symbol.hash)
		return "E'Virtual' keyword must come before identifier.";

	// Not a modifier, so if called and line is already being defined it means that the line is illegal
	if (lineIsDefiningSymbolDefinition)
		return "eMisplaced 'virtual': virtual is not modifier.";
	lineIsDefiningSymbolDefinition = true;
	// Otherwise, flag can be set

	// Since that pretty much eliminates any other case, just set this, and return with success.
	symbol.subtype = SYMBOL_TYPE::SYMTACC_VIRTUAL;

	return nullptr;
}

BBP::std::conststring BBP::esa::Frontend::markExtern()
{
	//TODO: Implement
	return "eExtern not implemented yet.";
}

BBP::std::conststring BBP::esa::Frontend::markStatic()
{
	// If the size for this symbol has already been defined, return error
	if (symbol.subtype & 0b111111100000)
		return "EIllegal 'static' after size definition";

	// If the symbol has already been hashed, return error
	if (symbol.hash && symbol.subtype & SYMBOL_TYPE::SYMTACC_STATIC)
		return "E'Static' modifier must come before identifier.";
	else if (symbol.hash)
		return "E'Static' keyword must come before identifier.";

	// If this is used as a modifier, check if it is right after virtual
	if (lineIsDefiningSymbolDefinition && (symbol.subtype & SYMBOL_TYPE::SYMTMOD_CONST || symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN))
		return "E'Static' modifier must come after 'virtual'. Modifier ignored.";

	// Check if it isn't already used
	if (lineIsDefiningSymbolDefinition && symbol.subtype & SYMBOL_TYPE::SYMTACC_STATIC)
		return "ECannot use modifier 'static' on 'static' symbol access type. Modifier ignored.";

	if (lineIsDefiningSymbolDefinition && symbol.subtype & SYMBOL_TYPE::SYMTMOD_STATIC)
		return "eDuplicate 'static' modifier.";

	// Static modifier legal.
	if (lineIsDefiningSymbolDefinition)
		symbol.subtype |= SYMBOL_TYPE::SYMTMOD_STATIC;
	else
		symbol.subtype = SYMBOL_TYPE::SYMTACC_STATIC;
	lineIsDefiningSymbolDefinition = true;

	return nullptr;
}

bool BBP::esa::beVerbose = false;

bool BBP::esa::Frontend::includeFromPath(std::FILE &file, std::PATH &filename, std::Stack<std::string_element> &dirs, std::PATH &out)
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
		std::PATH searchIn(BBP::std::primaryVolume, path);

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



BBP::std::conststring BBP::esa::Frontend::includeFile(std::Lexer::lex_context *context, std::index_t idx)
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
	bool opened = false;

	// Choose which paths to search based on delimiter used
	switch (delimiterStack[0])
	{
	case DOUBLEQUOTES_HANDLE:
		opened = includeFromPath(file, filename, context->usrpaths, imported);
		break;
	case OPENANGLEBRACKET_HANDLE:
		opened = includeFromPath(file, filename, context->syspaths, imported);
		break;
	}

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