#include "../include/ELSA.h"
#include "../include/ELSABackend.h"
#include "../include/Shell.h"
#include "Actions.h"

BBP::std::static_string<100> Identifier;

void BBP::esa::esaProcessor::saveIdentifier(BBP::std::index_t start, BBP::std::index_t end)
{
	for (BBP::std::index_t idx = start, n = 0; idx < end; idx++, n++)
		Identifier.static_data[n] = BBP::esa::processor.esaParser.data.data[idx];
	Identifier.static_data[end - start] = 0;
}

BBP::std::conststring BBP::esa::esaProcessor::mangleSymbolIdentifier(BBP::esa::dataEntry &entry)
{
	// Save the identifier
	saveIdentifier(symbol.start, symbol.end);

	// Then mangle
	application->mangle(Identifier.static_data, symbol, entry);

	// Return mangled name
	return application->mangledPage.static_data;
}

BBP::std::hash_t BBP::esa::esaProcessor::hashIdentifier(BBP::esa::dataEntry &entry)
{
	// Save the identifier
	saveIdentifier(symbol.start, symbol.end);

	// Define hash number 'h'
	std::hash_t hash = 0;

	for (std::index_t index = 0; index < Identifier.dataSize; index++)
	{
		// Get character
		std::string_element c = Identifier.static_data[index];

		// If c is 0, break out of loop.
		if (c == 0)
			break;

		// Operate on the hash
		hash = (hash * 33) + c;
	}

	// Return hash
	return hash;
}

void BBP::esa::esaProcessor::setupBackend(std::conststring str, std::size_t seg, std::size_t sec, std::size_t sym)
{
	application = system::Shell::getActiveMemory().add_object(new BinaryApplication(str, seg, sec, sym));
	application->setSource(esaParser.activeFile.static_data);
}

void BBP::esa::esaProcessor::saveAndClose()
{
	application->save();
	application->close();

	system::Shell::getActiveMemory()._delete(application);
	application = nullptr;
}

void BBP::esa::esaProcessor::discardAndClose()
{
	application->close();

	system::Shell::getActiveMemory()._delete(application);
	application = nullptr;
}

void BBP::esa::esaProcessor::emitRelocation(std::index_t symbolIndex, std::byte type)
{
	// Calculate offset of instruction from .text (in words)
	std::word offset = calculateArgumentOffset();

	// Create data entry
	std::ELF::RelEntry rel_entry;
	rel_entry.r_offset = application->builder._text().stack.atElement + offset;
	rel_entry.r_info = symbolIndex << 8 | type;

	// Set rel data
	std::word rel_data[2] = { rel_entry.r_offset, rel_entry.r_info };

	// Emit data
	dataEntry entry((char *)rel_data, 8, 0);
	application->addDataToSection(application->builder.rel_text, entry);
}

BBP::std::errno_t BBP::esa::esaProcessor::buildSymbol()
{

	// Look for already existing symbol
	std::hash_t hash = 0;
	std::word byteSize = 0;
	std::index_t existingSymbol = lookupSymbol(symbol.start, symbol.end, &byteSize, hash);
	bool symbolExists = existingSymbol != 0;

	// if symbol exists and is not 'extern'
	if (symbolExists && application->builder.symbols[existingSymbol].shndx())
		return EADDRINUSE;

	// if symbol exists, is marked 'extern', but is a procedure,
	if (symbolExists && application->builder.symbols[existingSymbol].info() & std::ELF::SYM_FUNC)
		return EBADR; // Cannot do that

	// Declare entry
	dataEntry entry = dataEntry(nullptr, 0, std::ELF::SYM_OBJECT | std::ELF::SYM_GLOBAL);
	
	// If entry is extern, just mangle and set to section 0.
	if (symbol.subtype & SYMTMOD_EXTERN)
	{
		// Create empty entry
		entry = dataEntry(nullptr, 0, std::ELF::SYM_OBJECT | std::ELF::SYM_GLOBAL);

		// Emit
		application->emitSymbol(0, entry, mangleSymbolIdentifier(entry), hashIdentifier(entry), existingSymbol);

		// Return
		resetSymbol();
		return 0;
	}

	// Do rules based on flags set
	// If access is 'static'
	if (symbol.subtype & SYMTACC_STATIC)
	{
		// Is it const?
		std::index_t section = (symbol.subtype & SYMTMOD_CONST) ? application->builder.rodata : application->builder.data;

		// Emit and return
		entry = dataEntry((std::conststring)symbolDataWord.page->data, symbolDataByte.atElement, std::ELF::SYM_OBJECT | std::ELF::SYM_GLOBAL);
		application->emitSymbol(section, entry, mangleSymbolIdentifier(entry), hashIdentifier(entry), existingSymbol);

		// Return
		resetSymbol();
		return 0;
	}
	else if (symbol.subtype & SYMTACC_VIRTUAL) // Are we virtual?
	{
		// Are we also marked constant?
		if (symbol.subtype & SYMTMOD_STATIC)
		{
			// Create another entry
			std::size_t size = application->builder.sections[application->builder.bss].stack.atElement;
			dataEntry staticEntry = dataEntry((std::conststring)&size, 4, std::ELF::SYM_LOCAL);

			// Are we also const?
			std::index_t section = (symbol.subtype & SYMTMOD_CONST) ? application->builder.rodata : application->builder.data;

			// Add data to .bss (.bss gets removed later)
			entry = dataEntry((std::conststring)symbolDataWord.page->data, symbolDataByte.atElement, std::ELF::SYM_OBJECT | std::ELF::SYM_GLOBAL);
			application->emitSymbol(application->builder.bss, entry, mangleSymbolIdentifier(entry), hashIdentifier(entry), existingSymbol);

			// Emit static symbol
			application->emitSymbol(section, staticEntry, mangleSymbolIdentifier(staticEntry), hashIdentifier(staticEntry), existingSymbol);

			// Return 
			resetSymbol();
			return 0;
		}
		else
		{
			// Not virtual, just do it normally then.
			// Add data to .bss (.bss gets removed later)
			entry = dataEntry((std::conststring)symbolDataWord.page->data, symbolDataByte.atElement, std::ELF::SYM_OBJECT | std::ELF::SYM_GLOBAL);
			
			// Emit
			application->emitSymbol(application->builder.bss, entry, mangleSymbolIdentifier(entry), hashIdentifier(entry), existingSymbol);

			// Return 
			resetSymbol();
			return 0;
		}
	}

	// Quasi symbol cannot be exported.
	// Find empty quasi symbol spot
	std::index_t emptyQuasi = 0;
	for (; emptyQuasi < processor.quasiSymbols.dataSize; emptyQuasi++)
	{
		// Get value
		std::hash_t value = std::read(&processor.quasiSymbols, emptyQuasi);

		// If value is 0, break out.
		if (value != 0)
			continue;

		// Do nothing
		break;
	}

	// Check for error condition
	if (emptyQuasi == processor.quasiSymbols.dataSize)
		return ENOBUFS;

	// Calculate hash
	std::hash_t quasiHash = hashIdentifier(entry);

	// Set hash to that, then set value
	std::write(&processor.quasiSymbols, quasiHash, emptyQuasi);
	std::write(&processor.quasiSymbolValues, symbolDataWord[0], emptyQuasi);

	// Reset symbol
	resetSymbol();

	return 0;
}

BBP::std::conststring BBP::esa::esaProcessor::emitProcedure()
{
	// Before doing anything, emit a 'ret 0' (if procedure is at least non-extern)
	if (!(processor.symbol.subtype & SYMTMOD_EXTERN))
	{
		resetInstruction();
		setInstruction(userspace::CTRL, userspace::ret, 0, 1);
		expectLiteral(0);
		numericalArgument({ 0 }, false);
		emitInstruction();
		resetInstruction();
	}

	// Create procedure.
	dataEntry procedure(nullptr, 0, std::ELF::SYM_FUNC | ((symbol.subtype & SYMTMOD_SHARED) ? std::ELF::SYM_WEAK : std::ELF::SYM_GLOBAL));
	
	// Define symbol
	symbol = std::Lexer::lex_thing{ &esaParser, startedAt, endedAt, nullptr, esa::PROCEDURE_T, procedureArgumentCount | (symbol.subtype & SYMTMOD_SHARED), procedureHash };

	// Mangle procedure name
	application->mangle(procedureName.static_data, symbol, procedure);

	// Hash identifier
	std::hash_t hash = 0;

	for (std::index_t index = 0; index < procedureName.dataSize; index++)
	{
		// Get character
		std::string_element c = procedureName.static_data[index];

		// If c is 0, break out of loop.
		if (c == 0)
			break;

		// Operate on the hash
		hash = (hash * 33) + c;
	}


	// Yeah yeah extraction whatever idc right now
	// Check for symbol memory uniqueness, since an overwrite could loose data
	if (application->builder.symbols[procedure.symbol].isMemUnique)
		throw std::exception("Symbol overwrite looses unique data.", EADDRINUSE);

	// Check if procedure already exists
	std::hash_t hasher = 0;
	std::word byteSize = 0;
	std::index_t existingSymbol = lookupSymbol(procedureNameStart, procedureNameEnd, &byteSize, hasher);
	bool symbolExists = existingSymbol != 0;

	// if symbol exists and is not 'extern'
	if (symbolExists && application->builder.symbols[existingSymbol].shndx())
	{
		esaParser.resetInfo(esaParser.debugInformation);
		esaParser.debugInformation.highlight = procedureNameStart - 1;
		esaParser.debugInformation.highlightLength = procedureNameEnd - procedureNameStart + 2;
		esaParser.debugInformation.highlightmsg = "NSecond definition here:";
		return "EProcedure name already in use.";
	}

	// if symbol exists, is marked 'extern', but is a procedure,
	if (symbolExists && !(application->builder.symbols[existingSymbol].info() & std::ELF::SYM_FUNC))
		return "ECannot do that."; // Cannot do that

	// If exists, override
	if (symbolExists)
		application->addSymbol(procedure, application->mangledPage.static_data, existingSymbol);
	else
	{
		// If symbol has reached max, extend symbols
		if (application->builder.symbols.atElement >= application->builder.symbolCount)
			application->appendSymbols(6);

		application->addSymbol(procedure, application->mangledPage.static_data);
	}

	// Then also update the remaining values
	application->builder.symbols[procedure.symbol].shndx(externProcedure ? 0 : application->builder.text);
	application->builder.symbols[procedure.symbol].value(externProcedure ? 0 : startedAt);
	application->builder.symbols[procedure.symbol].size(externProcedure ? 0 : (endedAt - startedAt));

	// Emit symbol hash.
	bool couldHash = application->emitSymbolHash(hash, procedure.symbol);

	if (!couldHash)
		std::printf("ELF Builder: Failed to hash symbol\n");

	return nullptr;

}

BBP::std::conststring BBP::esa::esaProcessor::emitInstruction()
{
	// Check if right amount of arguments is reached.
	bool hasRightArguments = atArgument == instruction.header.args || surpress[atArgument];
	instruction.header.args = atArgument;

	// Check if there is room to do so
	// Worst case scenario: 1+8+4*8 = 41 words for an instruction.
	if (application->builder._text().stack.atElement + 41 >= application->builder._text().stack.max_elements)
	{
		std::word extendBy = (application->builder._text().stack.atElement + 41 + 128 < application->builder._text().stack.max_elements) ? 128 : (application->builder._text().stack.atElement + 41 + 128 - application->builder._text().stack.max_elements);
		application->builder._text().Extend(extendBy);
	}
	
	// Emit header
	application->builder._text().stack << instruction.header.prefix;
	application->builder._text().stack << instruction.header.opcode;
	application->builder._text().stack << instruction.header.suffix;
	application->builder._text().stack << (((instruction.header.flags & 0xF) << 4) | (instruction.header.args & 0xF));

	// Emit Arguments
	for (std::byte arg = 0; arg < instruction.header.args; arg++)
	{
		// If literal value,
		if (instruction.args[arg].isLiteralArg)
		{
			// Just emit that and continue.
			application->builder._text().stack << ((instruction.args[arg].data >> 24) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].data >> 16) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].data >> 8) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].data >> 0) & 0xFF);
		}
		else
		{

			application->builder._text().stack << (((instruction.args[arg].referenced << 7) | (instruction.args[arg].dereferenced << 6) |
				((instruction.args[arg].offset & 0xF) << 2) | (instruction.args[arg].trailerCount & 0b11)));

			application->builder._text().stack << ((instruction.args[arg].data >> 16) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].data >> 8) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].data >> 0) & 0xFF);

		}

		// Then emit trailers
		for (std::byte trailer = 0; trailer < instruction.args[arg].trailerCount; trailer++)
		{
			application->builder._text().stack << ((instruction.args[arg].trailers[trailer] >> 24) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].trailers[trailer] >> 16) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].trailers[trailer] >> 8) & 0xFF);
			application->builder._text().stack << ((instruction.args[arg].trailers[trailer] >> 0) & 0xFF);

		}
	}

	if (hasRightArguments)
		return nullptr;

	esaParser.debugInformation.errName = "-Wmismatched-arg-count";
	return "WAmount of arguments supplied does not match the amount of arguments expected.";
}