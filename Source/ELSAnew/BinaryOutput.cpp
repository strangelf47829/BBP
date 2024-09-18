#include "../include/ELSA/BinaryApplication.h"
#include "../include/FileSys.h"

BBP::elsa::BinaryApplication::BinaryApplication()
{
	// Link each section together
	sections.Link(handlers)
		.Link(text)
		.Link(got)
		.Link(dynstr)
		.Link(data)
		.Link(bss)
		.Link(data1)
		.Link(rodata)
		.Link(reltext)
		.Link(symtab)
		.Link(shstrtab)
		.Link(hashtab);
}

// Get byte
BBP::std::byte &BBP::elsa::BinaryApplication::operator[] (std::index_t idx)
{
	// If index is within header, show that
	if (idx < 52)
		return header[idx];

	// Otherwise, return sections data
	return sections[idx - 52];
}

// Emit a file
void BBP::elsa::BinaryApplication::emitFile()
{



}