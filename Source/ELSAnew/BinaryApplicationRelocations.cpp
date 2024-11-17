#include "../include/ELSA/BinaryApplication.h"

void BBP::elsa::BinaryApplication::getRelocationPage(std::PAGE<std::byte> & page)
{
	// Get relocation section
	BBP::ELF::Section elfSection;

	// Get index of reltext
	std::index_t reltextIndex = elf.find(".rel.text");

	// elfSection (which is .rel) is stored in application, and has fixed size e_shentsize (stored in elf header)
	elfSection.data = std::PAGE<std::byte>(elf.header.e_shentsize, &elf.sectionTable[elf.header.e_shentsize * reltextIndex]);

	// Read section from file
	elfSection.readData(elf.header.ident.littleEndian);

	// Get size for relocation 
	std::size_t allocationEntitySize = elfSection.entsize;

	// Then allocate that amount from reltext
	reltext.Allocate(page, allocationEntitySize);
}
