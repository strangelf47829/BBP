#include "../include/ELF.h"

void BBP::std::ELF::SectionBuilder::Append(std::size_t size)
{
	// If size is 0, do nothing
	if (size == 0)
		return;

	// If builder is null, signal SIGSEV
	if (elf == nullptr)
	{
		__SIGNAL__(SIGSEGV);
	}

	// Do not overwrite old data. if old data exists, throw error
	if (data.dataSize)
		throw std::exception("Data overwrite attempt.", EALREADY);

	// Get current length of elf page
	std::size_t currentLength = std::seqlen(elf->file);

	// Save offset and size to header
	offset(currentLength);
	this->size(size);

	// Now create a new page
	data = std::string(size, (std::string_element *)elf->allocator->calloc(size, sizeof(std::string_element)));
	isMemUnique = true;

	// Then set the stack to that page
	stack = std::Stack<string_element>(&data, size);

	// Then append the data
	std::last(elf->file).nextPage = &data;
}

void BBP::std::ELF::SectionBuilder::Extend(std::size_t size)
{
	// If builder is null, signal SIGSEV
	if (elf == nullptr)
	{
		__SIGNAL__(SIGSEGV);
	}

	// Can only extend old data, so if data does not exist, throw error.
	if (!data.dataSize)
		throw std::exception("Attempt to extend empty data.", EADDRNOTAVAIL);

	stack.Extend(elf->allocator, size);

	// Then update the size variable in this header
	this->size(this->size() + size);

	// Then recalculate offsets only for sections and headers after this one
	elf->recalculateOffsets(offset(), size);

}

void BBP::std::ELF::SectionBuilder::close()
{
	// If builder is null, signal SIGSEV
	if (elf == nullptr)
	{
		__SIGNAL__(SIGSEGV);
	}

	// If mem is unique, free it.
	if (isMemUnique)
		elf->allocator->free(data.data);
}