#define USE_32_BITS

#include "../include/Resources.h"
#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Kernel.h"
#include "../include/Syscalls.h"

BBP::std::pageAllocator::pageAllocator()
{
	// I should really remove that pesky lvalue restriction..... Oh well, idc
	void *nul = nullptr;
	AbstractGCEntry *nul2 = nullptr;

	// Zero initialize allocations
	for (std::index_t t = 0; t < allocations.dataSize; t++)
	{
		allocations[t] = nullptr;
		allocationsSizes[t] = 0;
	}

	// Set invalid indicies
	this->invalidAllocationIndex = allocations.dataSize;

	// Zero-init next available things
	this->nextAllocationAvailable = 0;
}

void *BBP::std::pageAllocator::malloc(std::size_t size)
{
	// See if allocation is possible
	if (this->nextAllocationAvailable == this->invalidAllocationIndex)
		throw std::exception("Could not allocate bytes to Resource manager: Out of allocation space.", ENOMEM);

	// Actually malloc data
	void *data = systemcalls::malloc(size);

	// Add that data
	add_alloc(data, size);

	// Return data
	return data;
}

void *BBP::std::pageAllocator::calloc(std::size_t count, std::size_t size)
{
	// See if allocation is possible
	if (this->nextAllocationAvailable == this->invalidAllocationIndex)
		throw std::exception("Could not allocate bytes to Resource manager: Out of allocation space.", ENOMEM);

	// Actually calloc data
	void *data = systemcalls::calloc(count, size);

	// Add that data
	add_alloc(data, count * size);

	// Return data
	return data;
}

BBP::std::index_t BBP::std::pageAllocator::free(void *ptr)
{
	// Check if pointer is not actually nullptr
	if (ptr == nullptr)
	{
		// Attempting to free nullptr: Do nothing but output something to stderr
		system::kernelSS()->activeContext->STDERR << "Attempt to free nullptr was made." <<= std::endl;
		return invalidAllocationIndex;
	}

	// Attempt to find index of pointer
	std::index_t ptrIndex = find_Alloc_pointer(ptr);

	// Call to internal free function
	this->free(ptr, ptrIndex);

	// Return found index
	return ptrIndex;
}

void BBP::std::pageAllocator::free(void *ptr, std::index_t ptrIndex)
{
	// If index invalid, throw sigsegv signal
	if (ptrIndex == this->invalidAllocationIndex)
		std::raise(std::SIGSEGV);

	// Pointer exists, so free it.
	systemcalls::free(ptr);

	// Then also write nullptr to index
	this->allocations[ptrIndex] = nullptr;
	this->allocationsSizes[ptrIndex] = 0;
}

BBP::std::index_t BBP::std::pageAllocator::find_next_empty_alloc()
{
	// Check if we are not already at free pointer
	void *ptr = allocations[nextAllocationAvailable];

	// If we are at nullptr already, just return current location
	if (ptr == nullptr)
		return this->nextAllocationAvailable;

	// Check +1, since chances are next pointer is free
	// Careful not to cause overflow
	if (this->nextAllocationAvailable + 1 < this->allocations.dataSize)
	{
		// Read pointer
		ptr = allocations[nextAllocationAvailable + 1];

		// If null, return.
		if (ptr == nullptr)
			return this->nextAllocationAvailable + 1;
	}

	// Here and Here + 1 are not free. Just search entire page.
	for (std::index_t idx = 0; idx < this->allocations.dataSize; idx++)
	{
		// Read one pointer
		ptr = allocations[idx];

		// If pointer is not free, skip
		if (ptr)
			continue;

		// Return idx
		return idx;
	}

	// Nothing found at all. Return invalid pointer
	return this->invalidAllocationIndex;
}

BBP::std::index_t BBP::std::pageAllocator::find_Alloc_pointer(void *ptr)
{
	// Loop over every pointer
	for (std::index_t idx = 0; idx < allocations.dataSize; idx++)
	{
		// Read value stored at 'idx'
		void *stored = allocations[idx];

		// If 'stored' and 'ptr' are equal, return idx
		if (stored == ptr)
			return idx;
	}

	// Nothing found, return invalid pointer
	return invalidAllocationIndex;
}

void BBP::std::pageAllocator::add_alloc(void *ptr, size_t size)
{
	// See if allocation is possible
	if (this->nextAllocationAvailable == this->invalidAllocationIndex)
		throw std::exception("Could not register alloc'd data to Resource manager: Out of allocation space.", ENOMEM);

	// Store that write into next possible alloc
	this->allocations[nextAllocationAvailable] = ptr;
	this->allocationsSizes[nextAllocationAvailable] = size;

	// Find next possible location
	this->nextAllocationAvailable = find_next_empty_alloc();
}


BBP::std::index_t BBP::std::pageAllocator::freeAll()
{
	// Keep track of objects that were free'd
	std::index_t freeCount = 0;

	// Loop over each pointer
	for (std::index_t idx = 0; idx < allocations.dataSize; idx++)
	{
		// Read pointer
		void *ptr = allocations[idx];

		// If ptr is nullptr, skip
		if (ptr == nullptr)
			continue;

		// Since this pointer has something, free it. Also, since we know the index, just pass it.
		this->free(ptr, idx);
		freeCount++;
	}

	// Return the amount of things free'd
	return freeCount;
}
