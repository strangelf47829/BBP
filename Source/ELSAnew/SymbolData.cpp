#include "../include/ELSA/Symbol.h"

// Section constructor - Default
BBP::elsa::symbol_data_t::symbol_data_t()
	: allocator(nullptr), allocated_data(), dynamic_data(0, nullptr, std::DYN_PAGE::Alignment::Byte, true)
{
}

// Section constructor - No known data
BBP::elsa::symbol_data_t::symbol_data_t(symbol_section_data_t &sectData, Section *section)
	: allocator(section), allocated_data(), dynamic_data(0, nullptr, std::DYN_PAGE::Alignment::Byte, true)
{
	// Write data
	sectData.dataSize = 0;
	sectData.sectionIndex = 0;
	sectData.sectionIndex = 0;
}

// Section constructor - Known data size
BBP::elsa::symbol_data_t::symbol_data_t(symbol_section_data_t &sectData, Section *section, std::size_t size)
	: allocator(section), allocated_data(), dynamic_data(size, nullptr, std::DYN_PAGE::Alignment::Byte, true)
{
	// Allocate at least 4 bytes
	sectData.sectionOffset = allocator->Allocate(allocated_data, (size < 4) ? 4 : size);

	// Then set dynamic data (guaranteed at least word since 4 bytes.
	dynamic_data = std::DYN_PAGE(allocated_data.dataSize, (std::word *)allocated_data.data, std::DYN_PAGE::Alignment::Byte, true);

	// Then write sectData data
	sectData.dataSize = size;
	sectData.sectionIndex = section->index();
}

// Section constructor - Constant data
BBP::elsa::symbol_data_t::symbol_data_t(symbol_section_data_t &sectData, Section *section, std::DYN_PAGE::Alignment alignment, std::word value)
	: allocator(section), allocated_data(), dynamic_data()
{
	// Allocate 4 bytes
	sectData.sectionOffset = allocator->Allocate(allocated_data, 4);

	// Then set dynamic data (guaranteed at least word since 4 bytes, then set alignment.
	dynamic_data = std::DYN_PAGE(!!((4 * alignment) / std::DYN_PAGE::Alignment::Word), (std::word *)allocated_data.data, alignment, true);

	// Then write 'value'
	dynamic_data.write(0, value);

	// Then write sectData data
	sectData.dataSize = 1;
	sectData.sectionIndex = section->index();
}

BBP::std::word BBP::elsa::symbol_data_t::read(std::index_t idx)
{
	return dynamic_data[idx];
}

void BBP::elsa::symbol_data_t::write(std::index_t idx, std::word value)
{
	dynamic_data.write(idx, value);
}

void BBP::elsa::symbol_data_t::write(std::index_t idx, std::string str)
{
	// Get string length
	std::size_t stringLength = std::strlen(str);

	// Write data at index
	for (std::index_t stridx = 0; stridx < stringLength; stridx++)
		dynamic_data.write(stridx + idx, str[stridx]);
}

void BBP::elsa::symbol_data_t::write(std::index_t idx, std::PAGE<std::byte> buffer)
{
	// Get buffer length
	std::size_t bufferLength = std::seqlen(buffer);

	// Write data at index
	for (std::index_t bufidx = 0; bufidx < bufferLength; bufidx++)
		dynamic_data.write(bufidx + idx, buffer[bufidx]);
}

void BBP::elsa::symbol_data_t::write(std::index_t idx, std::size_t stringLength, std::string str)
{
	// Write data at index
	for (std::index_t stridx = 0; stridx < stringLength; stridx++)
		dynamic_data.write(stridx + idx, str[stridx]);
}

void BBP::elsa::symbol_data_t::write(std::index_t idx, std::size_t bufferLength, std::PAGE<std::byte> buffer)
{
	// Write data at index
	for (std::index_t bufidx = 0; bufidx < bufferLength; bufidx++)
		dynamic_data.write(bufidx + idx, buffer[bufidx]);
}