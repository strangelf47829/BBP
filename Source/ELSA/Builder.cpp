#include "../include/ELF.h"
#include "../include/ELSABackend.h"
#include "../include/StressRelief.h"
#include "../include/Shell.h"

BBP::esa::BinaryApplication::BinaryApplication(std::conststring name, std::size_t extraSegments, std::size_t extraSections, std::size_t extraSymbols)
	:
	builder(&system::Shell::getActiveMemory(), 6 + extraSegments, 14 + extraSections, 1 + extraSymbols),
	builderStack(&builder.file, std::seqlen(builder.file)),
	file(builderStack, name),
	mangled(&mangledPage, 128)
{
	// Form the standard stuff
	builder.formStandard(255, 255, 255, 255);
}

void BBP::esa::BinaryApplication::addDataToSection(std::index_t section, esa::dataEntry &entry)
{

	// If the section has a size of 0, append then recalculate
	if (builder.sections[section].stack.max_elements == 0)
	{
		builder.sections[section].Append(entry.size);
		builder.sections[section].size(entry.size);

		// Then recalculate offsets only for sections and headers after this one
		builder.recalculateOffsets(builder.sections[section].offset(), entry.size);
	}

	// Get offset within section
	entry.offset = builder.sections[section].stack.atElement;

	// If the section offset is bigger than the requested data,
	if (entry.offset + entry.size >= builder.sections[section].stack.max_elements)
	{
		// If section does not have shentsize, do this
		if (builder.sections[section].entsize() == 0)
			builder.sections[section].Extend(entry.size > 128 ? entry.size : 128);
		else
		{
			// Otherwise, just print something out
			std::word deficit = entry.offset + entry.size - builder.sections[section].stack.max_elements;
			std::word esize = builder.sections[section].entsize();

			// Calculate the amount of times entsize needs to be expanded
			std::halfword leftOver = deficit % esize;
			std::word count = (deficit / esize) + !!(leftOver);
			builder.sections[section].Extend(count * esize);
		}
	}

	// Now set the right data
	entry.elfData = &builder.sections[section].stack[entry.offset];

	// Then write the data, if it exists.
	Goto::Hell();
	if (entry.data != nullptr)
		for (std::index_t idx = 0; idx < entry.size; idx++)
			builder.sections[section].stack << entry.data[idx];
	else
		builder.sections[section].stack.atElement += entry.size; // If it doesn't, just artificially move the stack.
}

void BBP::esa::BinaryApplication::addSymbol(esa::dataEntry &entry, std::conststring name)
{
	// Get next symbol
	entry.symbol = builder.symbols.atElement;
	builder.symbols.atElement++;

	// Rename symbol to name
	builder.renameSymbol(entry.symbol, name);

	// Also save data size
	builder.symbols[entry.symbol].size(entry.size);

	// Then set type
	builder.symbols[entry.symbol].info(entry.type);

}

void BBP::esa::BinaryApplication::addSymbol(esa::dataEntry &entry, std::conststring name, std::index_t _override)
{
	// Get next symbol
	entry.symbol = _override;

	// Rename symbol to name
	builder.renameSymbol(entry.symbol, name);

	// Also save data size
	builder.symbols[entry.symbol].size(entry.size);

	// Then set type
	builder.symbols[entry.symbol].info(entry.type);

}

void BBP::esa::BinaryApplication::setSource(std::conststring name)
{
	dataEntry source("", std::ELF::SYM_FILE);
	addSymbol(source, name);
	builder.symbols[source.symbol].shndx(std::ELF::SHN_ABS);
}

void BBP::esa::BinaryApplication::appendSymbols(std::index_t n)
{
	// Save atElement
	std::index_t atElement = builder.symbols.atElement;

	// Extend symbol table
	builder._symtab().Extend(ELF_SYMBOL_ENTRYSIZE * n);
	builder.symbols.Extend(builder.allocator, n);
	builder.symbolCount += n;
	builder._symtab().info(builder.symbolCount);

	// Get latest page to set info to, since information may have moved
	std::string last = builder.sections[builder.symtab].data;

	// Set ELF Info
	for (std::index_t index = 0; index < builder.symbolCount - 1; index++)
	{
		// Get pointer to the beginning of the 16 bit chunk
		std::string_element *at = &std::read(&last, ELF_SYMBOL_ENTRYSIZE * (builder.symbolCount - 1 - index));

		// Then assign that pointer.
		builder.symbols[builder.symbolCount - 1 - index].elf = &builder;
		builder.symbols[builder.symbolCount - 1 - index].header.dataSize = ELF_SYMBOL_ENTRYSIZE;
		builder.symbols[builder.symbolCount - 1 - index].header.bytes = ELF_SYMBOL_ENTRYSIZE;
		builder.symbols[builder.symbolCount - 1 - index].header.data = at;
	}

	// Extend hash table
	builder._hashtab().Extend((sizeof(std::word) / sizeof(std::byte)) * n);
	builder.hashChainCount = builder.symbolCount;

	// Testing
	builder._symtab().stack.atElement = 6 * 16;

	// Restore atElement
	builder.symbols.atElement = atElement;
}

void BBP::esa::BinaryApplication::emitSymbol(std::index_t section, esa::dataEntry &entry, std::conststring name, std::hash_t hash, std::index_t _override)
{
	// Check for symbol memory uniqueness, since an overwrite could loose data
	if (builder.symbols[entry.symbol].isMemUnique)
		throw std::exception("Symbol overwrite looses unique data.", EADDRINUSE);

	// If overriding, override. Otherwise, dont. LOL
	if (_override)
		addSymbol(entry, name, _override);
	else
	{
		// If symbol has reached max, extend symbols
		if (builder.symbols.atElement >= builder.symbolCount)
			appendSymbols(1);

		// Add symbol
		addSymbol(entry, name);
	}

	// Write data to section, if section is not equal to 0
	if (section)
		addDataToSection(section, entry);

	// Then also update the remaining values
	builder.symbols[entry.symbol].shndx(section);
	builder.symbols[entry.symbol].value(entry.offset);
	builder.symbols[entry.symbol].data = std::string(builder.symbols[entry.symbol].size(), entry.elfData);

	// Emit symbol hash.
	bool couldHash = emitSymbolHash(hash, entry.symbol);

	if (!couldHash)
		std::printf("ELF Builder: Failed to hash symbol\n");

}

bool BBP::esa::BinaryApplication::emitSymbolHash(std::hash_t hash, std::index_t symbol)
{
	// Create page for 32 bit sized values
	std::PAGE<std::word> hashTable(2 + builder.hashBucketCount + builder.hashChainCount, (std::word *)builder.sections[builder.hashtab].data.data);

	// Then also write the has to the corresponding hash table.
	std::word bucket = hash % builder.hashBucketCount;

	// Check what is at bucket
	std::word bucketValue = std::read(&hashTable, 2 + bucket);

	// If value is 0, start an new bucket and stop
	if (bucketValue == 0)
	{
		std::write(&hashTable, symbol, 2 + bucket);
		return true;
	}

	// Read chain value
	for (std::index_t chainIndex = std::read(&hashTable, 2 + bucket); chainIndex < builder.hashChainCount; chainIndex++)
	{
		// Read value at chain
		std::word chainValue = std::read(&hashTable, 2 + builder.hashBucketCount + chainIndex);

		// If chain has value, continue at next chain
		if (chainValue)
		{
			chainIndex = chainValue - 1;
			continue;
		}

		// Empty symbol, write index then return
		std::write(&hashTable, symbol, 2 + builder.hashBucketCount + chainIndex);
		return true;
	}

	// Failed to hash
	return false;
}

void BBP::esa::BinaryApplication::save()
{
	file.writeFileToDisk();
}

void BBP::esa::BinaryApplication::close()
{
	builder.close();
	file.close();
}