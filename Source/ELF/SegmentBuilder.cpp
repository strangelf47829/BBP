#include "../include/ELF.h"
#include "../include/Signal.h"

void BBP::std::ELF::SegmentBuilder::Append(std::size_t size)
{
	// If builder is null, signal SIGSEV
	if (elf == nullptr)
		std::raise(std::SIGSEGV);

	// Get current length of elf page
	std::size_t currentLength = std::seqlen(elf->file);

	// Save offset and size to header
	offset(currentLength);
	filesz(size);
	memsz(size);

	// Now create a new page
	data = std::string(size, (std::string_element *)elf->allocator->calloc(size, sizeof(std::string_element)));
	isMemUnique = true;

	// Then set the stack to that page
	stack = std::Stack<string_element>(&data, size);

	// Then append the data
	std::last(elf->file).nextPage = &data;
}

void BBP::std::ELF::SegmentBuilder::close()
{
	// If builder is null, signal SIGSEV
	if (elf == nullptr)
		std::raise(std::SIGSEGV);

	// If mem is unique, free it.
	if (isMemUnique)
		elf->allocator->free(data.data);
}