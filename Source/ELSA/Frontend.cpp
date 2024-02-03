#include "../include/ELSA.h"
#include "../include/ELSABackend.h"

BBP::std::conststring BBP::esa::esaProcessor::markConst()
{
	// Check if symbol is extern
	if (symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN)
		return "EIllegal 'const': symbol marked extern.";

	// If the size for this symbol has already been defined, return error
	if (this->symbol.subtype & 0b111111100000)
		return "EIllegal 'const' after size definition";
	// If the symbol has already been hashed, return error
	if (this->symbol.hash)
		return "E'Const' modifier must come before identifier.";

	// Check that this is not the access type
	if (!lineIsDefiningSymbolDefinition)
		return "eModifier 'const' is not a valid access type.";

	// Check that symbol isn't already marked const
	if (this->symbol.subtype & SYMBOL_TYPE::SYMTMOD_CONST)
		return "eDuplicate 'const' modifier.";

	this->symbol.subtype |= SYMBOL_TYPE::SYMTMOD_CONST;

	return nullptr;
}

BBP::std::conststring BBP::esa::esaProcessor::markVirtual()
{
	// Check if symbol is extern
	if (symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN)
		return "EIllegal 'virtual': symbol marked extern.";

	// If the size for this symbol has already been defined, return error
	if (this->symbol.subtype & 0b111111100000)
		return "EIllegal 'virtual' after size definition";

	// If the symbol has already been hashed, return error
	if (this->symbol.hash)
		return "E'Virtual' keyword must come before identifier.";

	// Not a modifier, so if called and line is already being defined it means that the line is illegal
	if (lineIsDefiningSymbolDefinition)
		return "eMisplaced 'virtual': virtual is not modifier.";
	lineIsDefiningSymbolDefinition = true;
	// Otherwise, flag can be set

	// Check if symbol is shared
	bool wasMarkedShared = symbol.subtype & SYMBOL_TYPE::SYMTMOD_SHARED;

	// Since that pretty much eliminates any other case, just set this, and return with success.
	this->symbol.subtype = SYMBOL_TYPE::SYMTACC_VIRTUAL;

	if (wasMarkedShared)
		return "W[[shared]] or [[export]] must come after 'virtual'. Attribute ignored.";

	return nullptr;

}

BBP::std::conststring BBP::esa::esaProcessor::markExtern()
{
	// Check if symbol is not already marked extern
	if (symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN)
		return "ERemove excess extern.";

	// Check if extern comes after name
	if (symbol.hash)
		return "E'Extern' keyword must come before identifier.";

	// Check if subtype is anything at all
	if (symbol.subtype)
		return "E'Extern' keyword may not be used with 'Static', 'Virtual', or 'const'.";

	// Mark symbol extern
	symbol.subtype = SYMBOL_TYPE::SYMTMOD_EXTERN;

	// Mark function extern, just in case
	externProcedure = true;

	return nullptr;
}

BBP::std::conststring BBP::esa::esaProcessor::markStatic()
{
	// Check if symbol is extern
	if (symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN)
		return "EIllegal 'static': symbol marked extern.";

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

	// Check if symbol is shared
	bool wasMarkedShared = symbol.subtype & SYMBOL_TYPE::SYMTMOD_SHARED;

	// Static modifier legal.
	if (lineIsDefiningSymbolDefinition)
		symbol.subtype |= SYMBOL_TYPE::SYMTMOD_STATIC;
	else
		symbol.subtype = SYMBOL_TYPE::SYMTACC_STATIC;
	lineIsDefiningSymbolDefinition = true;

	if (wasMarkedShared)
		return "W[[shared]] or [[export]] must come after 'static'. Attribute ignored.";

	return nullptr;
}

BBP::std::conststring BBP::esa::esaProcessor::setHash(std::hash_t hash)
{
	// Cannot hash if: Hash already exists
	// Symbol has already been hashed

	// Check if the symbol doesn't already have a hash
	if (symbol.hash)
		return "EIgnoring misplaced identifier.";

	symbol.hash = hash;

	return nullptr;
}

BBP::std::conststring BBP::esa::esaProcessor::setSize(std::size_t size)
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

void BBP::esa::esaProcessor::addWord(std::word w)
{
	symbolDataWord << w;
	symbolDataHalfword.atElement += 2;
	symbolDataByte.atElement += 4;
}

void BBP::esa::esaProcessor::addHalfword(std::halfword w)
{
	symbolDataWord++;
	symbolDataHalfword << w;
	symbolDataByte.atElement += 2;
}

void BBP::esa::esaProcessor::addByte(std::byte w)
{
	symbolDataWord++;
	symbolDataHalfword++;
	symbolDataByte << w;
}

BBP::std::conststring BBP::esa::esaProcessor::addNumerical(std::Lexer::numerical_t numerical, bool isFloat)
{
	// Check if symbol is extern
	if (symbol.subtype & SYMBOL_TYPE::SYMTMOD_EXTERN)
		return "ECannot add data to extern symbol";

	// If no size specified, default to nil
	if (!(symbol.subtype & 0b111111100000))
		setSize(0);
	
	// If is quasi symbol or virtual, return error if multi-byte
	if ((symbol.subtype & SYMTSZ_NIL) && symbolDataByte.atElement >= 4)
		return "EQuasi symbols cannot be a multi word sequence. (Default size specifier is 32 bit, maybe you forgot a 'db'?)";

	// If we want to add a float, always do a 32bit word.
	if (isFloat)
	{
		// Just add the word
		std::word intermediate = *((std::word *)&numerical.f);
		addWord(intermediate);

		// If it is a 32bit variable, do nothing.
		if (symbol.subtype & (SYMTSZ_DW))
			return nullptr;

		// Else, log warning.
		esaParser.debugInformation.errName = "-Wfloat-size-mismatch";
		return "WFloat in non-32bit size sequence.";
	}

	// Add based on size
	esaParser.debugInformation.errName = "-Wtruncated-literal";
	switch (symbol.subtype & 0b111111100000)
	{
	case SYMTSZ_DN:
	case SYMTSZ_DB:
		addByte(numerical.i);
		if (numerical.i > 0xFFFF)
			return "WTruncated 32 bit literal to 8 bits.";
		else if (numerical.i > 0xFF)
			return "WTruncated 16 bit literal to 8 bits.";
		break;
	case SYMTSZ_DH:
		addHalfword(numerical.i);
		if (numerical.i > 0xFFFF)
			return "WTruncated 32 bit literal to 8 bits.";
		break;
	case SYMTSZ_DA:
	case SYMTSZ_DW:
	case SYMTSZ_DI:
		addWord(numerical.i);
		break;
	case SYMTSZ_NIL:
		addWord(numerical.i);
		break;
	}

	// Do nothing
	esaParser.debugInformation.errName = nullptr;
	return nullptr;
}

void BBP::esa::esaProcessor::resetSymbol()
{
	// Reset flags
	symbol.hash = 0;
	symbol.subtype = 0;
	lineIsDefiningSymbolDefinition = false;
	symbolDataByte.atElement = 0;
	symbolDataHalfword.atElement = 0;
	symbolDataWord.atElement = 0;

}

void BBP::esa::esaProcessor::logInstruction()
{
	// Print header
	std::printf("Instruction: 0x%02x (0x%02x), %u args.\n", instruction.header.opcode, instruction.header.suffix, instruction.header.args);
	
	for (std::byte i = 0; i < instruction.header.args; i++)
	{
		// Print argument number
		std::printf("\tArg %u:\t", i);

		// Print argument type
		if (!((instruction.header.prefix >> i) & 1))
		{
			std::printf("(Literal):\t0x%08x\n", instruction.args[i].data);
			continue;
		}
		else
			std::printf("(Reference)\t");

		// Check for referenced or dereferenced
		if (instruction.args[i].dereferenced)
			std::printf("*");
		
		if (instruction.args[i].referenced)
			std::printf("&");

		if (instruction.args[i].dereferenced && instruction.args[i].referenced)
			std::printf("\e[0;31mILLEGAL\e[0;37m");

		std::printf("\t");

		switch (instruction.args[i].offset)
		{
		case 0b00: // Starts at beginning of this instance
			std::printf("Rel: Instance\t");
			break;

		case 0b01: // Starts at beginning of whole chain
			std::printf("Rel: Chain\t");
			break;

		case 0b10: // Starts at beginning of shared user memory
			std::printf("Rel: User\t");
			break;

		case 0b11: // Starts at beginning of shared system memory
			std::printf("Rel: System\t");
			break;
		}

		std::printf("Page: 0x%01x\t", instruction.args[i].data >> 20);
		std::printf("Address: 0x%05x\n", instruction.args[i].data & 0x4ff);

		std::printf("\t\tTrailers: %u\n", instruction.args[i].trailerCount);

		for (std::byte k = 0; k < instruction.args[i].trailerCount; k++)
			std::printf("\t\tTrailer %u: 0x%08x\n", k, instruction.args[i].trailers[k]);

		std::printf("\n");


	}

}

void BBP::esa::esaProcessor::setInstruction(userspace::OPCODES opcode, std::byte suffix, std::nibble flags, std::nibble args)
{
	// Set flags
	processor.lineIsDefiningSymbolDefinition = false;
	processor.lineIsDefiningProcedure = false;
	processor.lineIsDefiningInstruction = true;

	instruction.header.prefix = suffix;
	instruction.header.opcode = opcode;

	instruction.header.flags = flags;
	instruction.header.args = args;

	// Set suffix
	instruction.header.suffix = suffix;

}


void BBP::esa::esaProcessor::expectLiteral(std::byte n)
{
	// Reset prefix
	instruction.header.prefix &= ~(0x1 << n);

	// Reset flags
	instruction.args[n].isLiteralArg = true;
	instruction.args[n].anyvalue = false;
}

void BBP::esa::esaProcessor::expectAddress(std::byte n)
{
	// Reset prefix
	instruction.header.prefix |= (0x1 << n);

	// Reset flags
	instruction.args[n].isLiteralArg = false;
	instruction.args[n].anyvalue = false;
}

void BBP::esa::esaProcessor::expectAny(std::byte n)
{
	// Only this needs to be set
	instruction.args[n].anyvalue = true;
}

void BBP::esa::esaProcessor::surpressArgCountWarning(std::byte n)
{
	surpress[n] = true;
}

void BBP::esa::esaProcessor::surpressArgCountWarning()
{
	for (std::byte n = 0; n < 8; n++)
		surpress[n] = true;
}

BBP::std::conststring BBP::esa::esaProcessor::setReference(std::word address, std::word page)
{
	// Save value
	bool expectedReference = !instruction.args[atArgument].isLiteralArg || instruction.args[atArgument].anyvalue;

	// Turn this argument into address
	expectAddress(atArgument);

	// Set instruction info
	instruction.args[atArgument].referenced = true;
	instruction.args[atArgument].data = (page << 16 | address);

	// Increment argument counter and return warning if needed
	atArgument++;
	if (!expectedReference)
		return "WDid not expect supplied argument to be address.";
	return nullptr;
}

BBP::std::conststring BBP::esa::esaProcessor::setDereference(std::word address, std::word page, std::byte count)
{
	// Save value
	bool expectedReference = !instruction.args[atArgument].isLiteralArg || instruction.args[atArgument].anyvalue;

	// Turn this argument into address
	expectAddress(atArgument);

	// Set instruction info
	instruction.args[atArgument].dereferenced = true;
	instruction.args[atArgument].trailerCount = 1;
	instruction.args[atArgument].trailers[0] = count;
	instruction.args[atArgument].data = (page << 16 | address);

	// Increment argument counter and return warning if needed
	atArgument++;
	if (!expectedReference)
		return "WDid not expect supplied argument to be address.";
	return nullptr;
}

BBP::std::conststring BBP::esa::esaProcessor::setArg(userspace::Instruction::Arg &arg)
{
	// If arg is literal, do this
	if (arg.isLiteralArg)
		return numericalArgument({ arg.data }, false);

	// Otherwise, do the following
	if (arg.referenced)
		return setReference(arg.data, arg.offset);
	else if (arg.dereferenced)
		return setDereference(arg.data, arg.offset, arg.trailers[0]);

	return "ECannot add argument that is of no type.";
}

void BBP::esa::esaProcessor::resetInstruction()
{
	// Clear head
	instruction.header = userspace::Instruction::Head{ 0, 0, 0, 0, 0 };

	// Clear arguments
	for (std::byte i = 0; i < 8; i++)
		instruction.args[i] = userspace::Instruction::Arg{ 0, false, false, 0, 0, {0, 0, 0, 0}, false, false };

	// Now reset to argument 0
	atArgument = 0;
}

void BBP::esa::esaProcessor::setProcedureAddress()
{
	startedAt = processor.application->builder._text().stack.atElement;
}

void BBP::esa::esaProcessor::setProcedureIdentifier(std::index_t start, std::index_t end, std::hash_t hash)
{
	// Iterate over the text
	for (std::index_t index = start, i = 0; index < end; index++, i++)
	{
		// Write into procedure name the procedure
		std::write(&procedureName, std::read(&esaParser.data, index), i);
	}

	// Then write terminating null
	std::string_element nil = 0;
	std::write(&procedureName, nil, end - start);

	// Then write hash
	procedureHash = hash;

	// And set approriate names
	procedureNameStart = start;
	procedureNameEnd = end;
}

void BBP::esa::esaProcessor::setProcedureArgCount(std::byte count)
{
	procedureArgumentCount = count;
	doneDeclaringProcedure = true;
}

void BBP::esa::esaProcessor::addReservedIdentifier(std::conststring identifier, userspace::Instruction::Arg data)
{
	// Check for nullptr
	if (identifier == nullptr)
		throw std::exception("Cannot add nullptr identifier.", EDOM);

	// Hash string
	std::hash_t identifierHash = 0;
	std::size_t identifierSize = std::strlen(identifier);

	// Calculate hash
	for (std::index_t index = 0; index < identifierSize; index++)
	{
		// Get character
		std::string_element c = identifier[index];

		// Operate on character
		identifierHash = (identifierHash * 33) + c;
	}

	// Look for identifier
	for (std::index_t identifierIndex = 0; identifierIndex < processor.reservedIdentifiers.dataSize; identifierIndex++)
	{
		// Get value
		std::hash_t storedValue = std::read(&processor.reservedIdentifiers, identifierIndex);

		// If non-zero, continue
		if (storedValue)
			continue;

		// Save data
		std::write(&processor.reservedIdentifiers, identifierHash, identifierIndex);
		std::write(&processor.reservedIdentifiersValues, data, identifierIndex);

		// Return
		return;
	}

	throw std::exception("No free reserved identifiers.", ENOBUFS);
}