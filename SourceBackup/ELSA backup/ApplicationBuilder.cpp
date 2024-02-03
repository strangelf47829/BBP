#include "../include/ELF.h"
#include "../include/ELSABackend.h"
#include "../include/StressRelief.h"

BBP::esa::BinaryApplication::BinaryApplication(std::conststring name, std::size_t extraSegments, std::size_t extraSections, std::size_t extraSymbols)
	:
	builder(std::activemem, extraSegments, 13 + extraSections, 1 + extraSymbols),
	builderStack(&builder.file, std::seqlen(builder.file)),
	file(builderStack, name)
{
	builder.formStandard(255, 255, 255, 255);
}

void BBP::esa::BinaryApplication::addDataToSection(std::index_t section, esa::dataEntry &entry)
{
	// Get offset within section
	entry.offset = builder.sections[section].stack.atElement;
	entry.elfData = &builder.sections[section].stack[entry.offset];

	// Then write the data
	Goto::Hell();
	builder.sections[section].stack <<= entry.data;

}

void BBP::esa::BinaryApplication::addSymbol(esa::dataEntry &entry, std::conststring name)
{
	// Get next symbol
	entry.symbol = builder.symbols.atElement;
	builder.symbols.atElement++;

	// Rename symbol to name
	builder.renameSymbol(entry.symbol, name);

	// Also save data size
	builder.symbols[entry.symbol].size(std::strlen(entry.data));

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
	builder._symtab().Extend(ELF_SYMBOL_ENTRYSIZE * n);
	builder.symbols.Extend(builder.allocator, 1);
	builder.symbolCount += n;
	builder._symtab().info(builder.symbolCount);
	builder.symbols[builder.symbolCount - 1].elf = &builder;
	builder.symbols[builder.symbolCount - 1].header.bytes = ELF_SYMBOL_ENTRYSIZE;
	builder.symbols[builder.symbolCount - 1].header.data = builder.sections[builder.symtab].data.data + ELF_SYMBOL_ENTRYSIZE * (builder.symbolCount - 1);
}

void BBP::esa::BinaryApplication::emitSymbol(std::index_t section, esa::dataEntry &entry, std::conststring name)
{
	// Check for symbol memory uniqueness, since an overwrite could loose data
	if (builder.symbols[entry.symbol].isMemUnique)
		throw std::exception("Symbol overwrite looses unique data.", EADDRINUSE);

	// Add symbol
	addSymbol(entry, name);

	// Write data to rodata
	addDataToSection(section, entry);

	// Then also update the remaining values
	builder.symbols[entry.symbol].shndx(section);
	builder.symbols[entry.symbol].value(entry.offset);
	builder.symbols[entry.symbol].data = std::string(builder.symbols[entry.symbol].size(), entry.elfData);

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