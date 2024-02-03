#include "../include/BinaryExecutable.h"
#include "../include/stdio.h"

void BBP::userspace::BinaryExecutable::loadExecutable(std::ELF::ELFBuilder &binary, std::ResourceManager *allocator)
{
	// Check if allocator is null
	if (allocator == nullptr)
		throw std::exception("Could not load binary: Allocator invalid.", ENODATA);

	this->allocator = allocator;

	// Keep track of the physical size
	std::size_t BinaryPhysicalSize = 0;
	std::size_t TLSSize = 0;
	std::index_t loadedSegment = 0;

	// Loop over each segment
	for (std::index_t segment = 0; segment < binary.segments.max_elements; segment++)
	{
		// If segment is not of type 'load', do nothing
		if (binary.segments[segment].type() != std::ELF::PT_LOAD && binary.segments[segment].type() != std::ELF::PT_TLS)
			continue;

		// Get physical and virtual memory sizes
		std::size_t physicalSize = binary.segments[segment].filesz();
		std::size_t virtualSize = binary.segments[segment].memsz();

		// Get ending address of segment
		std::size_t actualSize = ((physicalSize > virtualSize) ? physicalSize : virtualSize);
		std::address_t segmentEndsAt = binary.segments[segment].paddr() + actualSize;

		// If segment ends at address greater than physical size, increase physical size
		if (segmentEndsAt > BinaryPhysicalSize && binary.segments[segment].type() == std::ELF::PT_LOAD)
			BinaryPhysicalSize += actualSize;
		else if (segmentEndsAt > BinaryPhysicalSize && binary.segments[segment].type() == std::ELF::PT_TLS)
			TLSSize += actualSize;

		// Since we are here anyway, set segment data head data
		mapping.static_data[loadedSegment].physicalMemoryHead = binary.segments[segment].paddr();
		mapping.static_data[loadedSegment].virtualMemoryHead = binary.segments[segment].vaddr();

		// Also set size and flags
		std::word flag = binary.segments[segment].flags();

		mapping.static_data[loadedSegment].readable = flag & 0b100;
		mapping.static_data[loadedSegment].writeable = flag & 0b010;
		mapping.static_data[loadedSegment].executable = flag & 0b001;
		mapping.static_data[loadedSegment].allocated = true;

		mapping.static_data[loadedSegment].size = actualSize;

		// Increase loaded segment
		loadedSegment++;
	}

	totalSize = BinaryPhysicalSize + TLSSize;

	// Allocate the specified amount of bytes into memory.
	BinaryData = std::PAGE<std::string_element>(BinaryPhysicalSize, (std::string_element*)allocator->calloc(BinaryPhysicalSize, sizeof(std::string_element)));
	TLSData = std::PAGE<std::string_element>(TLSSize, (std::string_element *)allocator->calloc(TLSSize, sizeof(std::string_element)));
	allocated = true;

	// Link TLS data and Binary Data together
	BinaryData.nextPage = &TLSData;
	TLSData.prevPage = &BinaryData;

	// Now loop over each segment again
	for (std::index_t segment = 0; segment < binary.segments.max_elements; segment++)
	{
		// If segment is not of type 'load', do nothing
		if (binary.segments[segment].type() != std::ELF::PT_LOAD && binary.segments[segment].type() != std::ELF::PT_TLS)
			continue;

		// Get physical size, and offset
		std::size_t physicalSegmentSize = binary.segments[segment].filesz();
		std::offset_t segmentOffset = binary.segments[segment].offset();
		std::address_t loadIntoAddress = binary.segments[segment].paddr();

		// If offset is 0, continue
		if (segmentOffset == 0)
			continue;

		// Now copy binary data into physical memory
		for (std::index_t idx = 0; idx < physicalSegmentSize; idx++)
			__UNSAFE__(std::write)(&BinaryData, __UNSAFE__(std::read)(&binary.file, segmentOffset + idx), loadIntoAddress + idx);
	}

}

void BBP::userspace::BinaryExecutable::unloadExecutable()
{
	// If binary is not allocated, do nothing
	if (allocated == false)
		return;

	// Free binary page
	allocator->free(BinaryData.data);
	allocator->free(TLSData.data);
	BinaryData = std::PAGE<std::string_element>(0, nullptr);
	TLSData = std::PAGE<std::string_element>(0, nullptr);
	allocated = false;

	// Clear size
	totalSize = 0;

	// Loop over each segment and set them to zero
	for (std::index_t map = 0; map < mapping.dataSize; map++)
		mapping.static_data[map] = MemoryRegion();
	
	allocator = nullptr;
}


BBP::std::address_t BBP::userspace::BinaryExecutable::virtualToPhysical(std::address_t address, std::index_t cached)
{
	// If cached index is out of static_data index, throw exception
	if (cached >= mapping.dataSize)
		throw std::exception("Virtual memory cache index invalid.", EADDRNOTAVAIL);

	// Check if cached block contains address
	if (mapping.static_data[cached].allocated && mapping.static_data[cached].virtualMemoryHead <= address && (mapping.static_data[cached].virtualMemoryHead + mapping.static_data[cached].size) > address)
	{
		// If so, find difference and return
		std::offset_t offset = address - mapping.static_data[cached].virtualMemoryHead;
		return mapping.static_data[cached].physicalMemoryHead + offset;
	}

	// Otherwise, just return virtual to physical
	return virtualToPhysical(address);
}

BBP::std::address_t BBP::userspace::BinaryExecutable::physicalToVirtual(std::address_t address, std::index_t cached)
{
	// If cached index is out of static_data index, throw exception
	if (cached >= mapping.dataSize)
		throw std::exception("Physical memory cache index invalid.", EADDRNOTAVAIL);

	// Check if cached block contains address
	if (mapping.static_data[cached].allocated && mapping.static_data[cached].physicalMemoryHead <= address && (mapping.static_data[cached].physicalMemoryHead + mapping.static_data[cached].size) > address)
	{
		// If so, find difference and return
		std::offset_t offset = address - mapping.static_data[cached].physicalMemoryHead;
		return mapping.static_data[cached].virtualMemoryHead + offset;
	}

	// Otherwise, just return virtual to physical
	return physicalToVirtual(address);
}

BBP::std::address_t BBP::userspace::BinaryExecutable::virtualToPhysical(std::address_t address)
{
	// Find the address of the memory mapping for this address
	std::index_t block = findIndexOfVirtualMemory(address);

	// If no address is found, signal SIGSEGV
	if (block == mapping.dataSize)
	{
		__SIGNAL__(SIGSEGV);
	}

	// Retrieve variables
	std::address_t virtualHead = mapping.static_data[block].virtualMemoryHead;
	std::address_t physicalHead = mapping.static_data[block].physicalMemoryHead;

	// Valid block found. Now find difference between address and virtual head
	std::offset_t difference = address - virtualHead;

	// Then Add that difference to physical head
	return difference + physicalHead;
}

BBP::std::address_t BBP::userspace::BinaryExecutable::physicalToVirtual(std::address_t address)
{
	// Find the address of the memory mapping for this address
	std::index_t block = findIndexOfPhysicalMemory(address);

	// If no address is found, signal SIGSEGV
	if (block == mapping.dataSize)
	{
		__SIGNAL__(SIGSEGV);
	}

	// Retrieve variables
	std::address_t virtualHead = mapping.static_data[block].virtualMemoryHead;
	std::address_t physicalHead = mapping.static_data[block].physicalMemoryHead;

	// Valid block found. Now find difference between address and virtual head
	std::offset_t difference = address - physicalHead;

	// Then Add that difference to physical head
	return difference + virtualHead;
}

BBP::std::index_t BBP::userspace::BinaryExecutable::findIndexOfVirtualMemory(std::address_t address)
{
	// Loop over each memory mapping
	for (std::index_t block = 0; block < mapping.dataSize; block++)
	{
		// If block is not allocated, continue search
		if (mapping.static_data[block].allocated == false)
			continue;

		// Retrieve variables
		std::address_t virtualHead = mapping.static_data[block].virtualMemoryHead;
		std::size_t virtualSize = mapping.static_data[block].size;

		// Physical memory address is found if address is within virtual memory head and virtual memory head + size
		if (virtualHead <= address && (virtualHead + virtualSize) > address)
			return block;
	}

	// None found. Return block count to indicate error
	return mapping.dataSize;
}

BBP::std::index_t BBP::userspace::BinaryExecutable::findIndexOfPhysicalMemory(std::address_t address)
{
	// Loop over each memory mapping
	for (std::index_t block = 0; block < mapping.dataSize; block++)
	{
		// If block is not allocated, continue search
		if (mapping.static_data[block].allocated == false)
			continue;

		// Retrieve variables
		std::address_t physicalHead = mapping.static_data[block].physicalMemoryHead;
		std::size_t physicalSize = mapping.static_data[block].size;

		// Physical memory address is found if address is within virtual memory head and virtual memory head + size
		if (physicalHead <= address && (physicalHead + physicalSize) > address)
			return block;
	}

	// None found. Return block count to indicate error
	return mapping.dataSize;
}


// Create a copy of the TLS data
void BBP::userspace::BinaryExecutable::createTLS(std::PAGE<std::string_element> &page)
{
	// If no binary data is loaded, throw exception
	if (allocated == false)
		throw std::exception("Could not create TLS Data: No binary loaded", ENODATA);

	// Initialize page
	page = std::PAGE<std::string_element>(TLSData.dataSize, (std::string_element*)allocator->calloc(TLSData.dataSize, sizeof(std::string_element)));

	// Copy memory
	std::memcpy(TLSData, page);
}

// Swap out
void BBP::userspace::BinaryExecutable::loadTLS(std::PAGE<std::string_element> &page)
{
	// Save page next-prev relation
	TLSData.prevPage = page.prevPage;
	TLSData.nextPage = page.nextPage;

	// Now set binary data
	BinaryData.nextPage = &page;
	page.prevPage = &BinaryData;
}

// Swap around page stuff again
void BBP::userspace::BinaryExecutable::unloadTLS(std::PAGE<std::string_element> &page)
{
	// Restore page next-prev relation
	page.prevPage = TLSData.prevPage;
	page.nextPage = TLSData.nextPage;

	// Now set binary data
	BinaryData.nextPage = &TLSData;
	TLSData.prevPage = &BinaryData;
}