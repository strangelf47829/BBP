#include "../include/Memory.h"
#include "../include/Signal.h"
#include "../include/stdio.h"

// Creates a new page from scratch
BBP::std::DYN_PAGE::DYN_PAGE(std::size_t size, std::word *_data, Alignment _alignment, bool WPerm)
{
	// Set write permission
	canUserWrite = WPerm;

	// Check if alignment is supported by machine.
	if (_alignment > sizeof(std::word) * 8)
		throw std::exception("Failed to align dynamic page: Unsupported bit size.", ENOMEM);

	// If attempting to set alignment to 0, set to unallocated.
	if (_alignment == Alignment::Unallocated || _alignment == 0)
	{
		dataSize = 0;
		bytes = 0;
	}
	else
	{
		// Calculate the size
		dataSize = size;
		bytes = size * (Alignment::Word / _alignment);
	}

	// Set data to raw
	data = _data;

	// Set alignment
	alignment = _alignment;
}

// Default constructor
BBP::std::DYN_PAGE::DYN_PAGE()
{
	// Set everything to 0
	data = nullptr;
	dataSize = 0;
	bytes = 0;
	alignment = Alignment::Unallocated;
}

void BBP::std::DYN_PAGE::calculateIndicies(std::index_t idx, std::index_t &base, std::index_t &top, std::offset_t &bitOffset)
{
	// Calculate the base index of the 32 bit word to be read
	base = (idx * alignment) / Alignment::Word;

	// Calculate the bitoffset and top index
	bitOffset = (idx * alignment) % Alignment::Word;
	top = base + (Alignment::Word - bitOffset < alignment);
}

// Get stuff
BBP::std::word BBP::std::DYN_PAGE::operator[](std::index_t idx)
{
	// Check if index is within bounds
	if (idx >= dataSize)
		std::raise(std::SIGSEGV);

	// Calculate the base index of the 32 bit word to be read
	std::index_t base, top, bitOffset = 0;
	calculateIndicies(idx, base, top, bitOffset);

	// Now calculate the baseComponent and topcomponent
	std::word baseComponent = (data[base] >> bitOffset);
	std::word topComponent = (data[top] & getMask(alignment - (Alignment::Word - bitOffset)));

	// Set value to the truncated value
	std::word value = truncate(baseComponent);

	// If top is distinct from base, also add top back in.
	if (top != base)
		value |= topComponent << (Alignment::Word - bitOffset);

	// Return value
	return value;
}

// Write stuff
void BBP::std::DYN_PAGE::write(std::index_t idx, std::word value)
{
	// Check if index is withing bounds
	if (idx >= dataSize)
		std::raise(std::SIGSEGV);

	// Calculate the base index of the 32 bit word to be read
	std::index_t base, top, bitOffset = 0;
	calculateIndicies(idx, base, top, bitOffset);
	std::byte outOfRange = (alignment - (Alignment::Word - bitOffset)) * (top != base);

	// Calculate the whole value that needs to be written
	std::word complete = truncate(value);
	
	// Get the bottom and top components
	std::word baseComponent = complete << bitOffset;
	std::word topComponent = complete >> (alignment - outOfRange);

	// Clear base target of residual bits, then write
	data[base] &= ~(getMask() << bitOffset);
	data[base] |= baseComponent;

	// If top is distinct from bottom, write that too
	if (top != base)
	{
		data[top] &= ~(getMask(alignment - (Alignment::Word - bitOffset)));
		data[top] |= topComponent;
	}
}

// Truncating and masking
BBP::std::word BBP::std::DYN_PAGE::getMask()
{
	return getMask(alignment);
}

BBP::std::word BBP::std::DYN_PAGE::getMask(std::byte al)
{
	if (al == 32)
		return 0xFFFF;

	if (al > 32)
		return 0;

	return (1 << al) - 1;
}

BBP::std::word BBP::std::DYN_PAGE::truncate(std::word value)
{
	return truncate(value, alignment);
}

BBP::std::word BBP::std::DYN_PAGE::truncate(std::word value, std::byte bits)
{
	return value & getMask(bits);
}

// Getting and setting values
BBP::std::DYN_PAGE::Alignment BBP::std::DYN_PAGE::getAlignment()
{
	return alignment;
}

void BBP::std::DYN_PAGE::setAlignment(BBP::std::DYN_PAGE::Alignment al)
{
	// Check if alignment is 0
	if (al == Alignment::Unallocated)
	{
		dataSize = 0;
		alignment = al;
		return;
	}

	// Set alignment and data size
	alignment = al;

	// For things below a byte,
	if (al < Alignment::Byte)
		dataSize = bytes * (Alignment::Byte / al);
	else
		dataSize = bytes / (al / Alignment::Byte);
}