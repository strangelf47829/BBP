#define USE_32_BITS

#include "../include/Resources.h"
#include "../include/stdio.h"
#include "../include/Signal.h"


void *BBP::std::ResourceManager::malloc(std::size_t size)
{
	// See if allocation is possible
	if (this->nextAllocationAvailable == this->invalidAllocationIndex)
		throw std::exception("Could not allocate bytes to Resource manager: Out of allocation space.", ENOMEM);

	// Actually malloc data
	void *data = std::ext_malloc(size);
	this->totalAllocations += size;
	//std::printf("(%p) Application allocated %u bytes during runtime. (malloc: %u)\n", this, this->totalAllocations, size);

	// Add that data
	add_alloc(data);

	// Return data
	return data;
}

void *BBP::std::ResourceManager::calloc(std::size_t count, std::size_t size)
{
	// See if allocation is possible
	if (this->nextAllocationAvailable == this->invalidAllocationIndex)
		throw std::exception("Could not allocate bytes to Resource manager: Out of allocation space.", ENOMEM);

	// Actually calloc data
	void *data = std::ext_calloc(count, size);
	this->totalAllocations += size * count;
	//std::printf("(%p) Application allocated %u bytes during runtime. (calloc: %u)\n", this, totalAllocations, size * count);

	// Add that data
	add_alloc(data);

	// Return data
	return data;
}

void BBP::std::ResourceManager::add_alloc(void *ptr)
{
	// See if allocation is possible
	if (this->nextAllocationAvailable == this->invalidAllocationIndex)
		throw std::exception("Could not register alloc'd data to Resource manager: Out of allocation space.", ENOMEM);

	// Store that write into next possible alloc
	std::write(&this->allocations, ptr, this->nextAllocationAvailable);

	// Find next possible location
	this->nextAllocationAvailable = find_next_empty_alloc();
}

void BBP::std::ResourceManager::free(void *ptr)
{
	// Check if pointer is not actually nullptr
	if (ptr == nullptr)
	{
		// Attempting to free nullptr: Do nothing but output something to stderr
		std::STDERR << "Attempt to free nullptr was made." <<= std::endl;
		return;
	}

	// Attempt to find index of pointer
	std::index_t ptrIndex = find_Alloc_pointer(ptr);

	// Call to internal free function
	this->free(ptr, ptrIndex);
}

void BBP::std::ResourceManager::free(void *ptr, std::index_t ptrIndex)
{
	// If index invalid, throw sigsegv signal
	if (ptrIndex == this->invalidAllocationIndex)
		std::raise(std::SIGSEGV);

	// Pointer exists, so free it.
	std::ext_free(ptr);

	// Then also write nullptr to index
	this->allocations.data[ptrIndex] = (void *)0;
	//__UNSAFE__(std::write)(&this->allocations, nul, ptrIndex);
}

void BBP::std::ResourceManager::mark_deleted(void *ptr)
{
	// Check if pointer is not actually nullptr
	if (ptr == nullptr)
	{
		// Attempting to free nullptr: Do nothing but output something to stderr
		std::STDERR << "Attempt to delete nullptr was made." <<= std::endl;
		return;
	}

	// Attempt to find index of pointer
	std::index_t ptrIndex = find_Object_pointer(ptr);

	// Write nullptr to index
	delete(this->objects.data[ptrIndex]);
	this->objects.data[ptrIndex] = nullptr;
}

void BBP::std::ResourceManager::_delete(void *ptr)
{
	// Check if pointer is not actually nullptr
	if (ptr == nullptr)
	{
		// Attempting to free nullptr: Do nothing but output something to stderr
		std::STDERR << "Attempt to delete nullptr was made." <<= std::endl;
		return;
	}

	// Attempt to find index of pointer
	std::index_t ptrIndex = find_Object_pointer(ptr);

	// Call to internal _delete function
	this->_delete(ptrIndex);
}

void BBP::std::ResourceManager::_delete(std::index_t ptrIndex)
{
	// If index invalid, throw sigsegv signal
	if (ptrIndex == this->invalidObjectIndex)
		std::raise(std::SIGSEGV);

	// Pointer exists, so delete it.
	this->objects.data[ptrIndex]->_delete();
	delete(this->objects.data[ptrIndex]);
	this->objects.data[ptrIndex] = nullptr;
}

BBP::std::index_t BBP::std::ResourceManager::find_next_empty_alloc()
{
	// Check if we are not already at free pointer
	void *ptr = std::read(&this->allocations, this->nextAllocationAvailable);

	// If we are at nullptr already, just return current location
	if (ptr == nullptr)
		return this->nextAllocationAvailable;

	// Check +1, since chances are next pointer is free
	// Careful not to cause overflow
	if (this->nextAllocationAvailable + 1 < this->allocations.dataSize)
	{
		// Read pointer
		ptr = std::read(&this->allocations, this->nextAllocationAvailable + 1);
		
		// If null, return.
		if (ptr == nullptr)
			return this->nextAllocationAvailable + 1;
	}

	// Here and Here + 1 are not free. Just search entire page.
	for (std::index_t idx = 0; idx < this->allocations.dataSize; idx++)
	{
		// Read one pointer
		ptr = __UNSAFE__(std::read)(&this->allocations, idx);
		
		// If pointer is not free, skip
		if (ptr)
			continue;

		// Return idx
		return idx;	
	}

	// Nothing found at all. Return invalid pointer
	return this->invalidAllocationIndex;
}

BBP::std::index_t BBP::std::ResourceManager::find_next_empty_object()
{
	// Declare ptr
	void *ptr = nullptr;

	// Check if pointer exists
	if (this->objects.data[this->nextObjectAvailable])
	{
		// Check if we are not already at free pointer
		ptr = std::read(&this->objects, this->nextObjectAvailable)->getRawPtr();

		// If we are at nullptr already, just return current location
		if (ptr == nullptr)
			return this->nextObjectAvailable;
	}
	else
	{
		// Since th object at this index is apparently empty, return this index
		return this->nextObjectAvailable;
	}

	// Check +1, since chances are next pointer is free
	// Careful not to cause overflow
	if (this->nextObjectAvailable + 1 < this->objects.dataSize)
	{
		// Check if pointer exists
		if (this->objects.data[this->nextAllocationAvailable + 1] == nullptr)
			// Since th object at this index is apparently empty, return this index
			return this->nextObjectAvailable + 1;
	}

	// Here and Here + 1 are not free. Just search entire page.
	for (std::index_t idx = 0; idx < this->objects.dataSize; idx++)
	{
		// Check if pointer exists. If not, this object is empty, so return this index
		if (!this->objects.data[idx])
			return idx;

		// Read one pointer
		ptr = __UNSAFE__(std::read)(&this->objects, idx)->getRawPtr();

		// If pointer is not free, skip
		if (ptr)
			continue;

		// Return idx
		return idx;
	}

	// Nothing found at all. Return invalid pointer
	return this->invalidObjectIndex;
}

BBP::std::index_t BBP::std::ResourceManager::find_Alloc_pointer(void *ptr)
{
	// Loop over every pointer
	for (std::index_t idx = 0; idx < allocations.dataSize; idx++)
	{
		// Read value stored at 'idx'
		void *stored = __UNSAFE__(std::read)(&allocations, idx);

		// If 'stored' and 'ptr' are equal, return idx
		if (stored == ptr)
			return idx;
	}

	// Nothing found, return invalid pointer
	return invalidAllocationIndex;
}

BBP::std::index_t BBP::std::ResourceManager::find_Object_pointer(void *ptr)
{
	// Loop over every pointer
	for (std::index_t idx = 0; idx < objects.dataSize; idx++)
	{
		// Check if pointer exists
		if (!this->objects.data[idx])
			continue;

		// Read value stored at 'idx'
		void *stored = __UNSAFE__(std::read)(&objects, idx)->getRawPtr();

		// If 'stored' and 'ptr' are equal, return idx
		if (stored == ptr)
			return idx;
	}

	// Nothing found, return invalid pointer
	return invalidObjectIndex;
}

BBP::std::index_t BBP::std::ResourceManager::freeAll()
{
	// Keep track of objects that were free'd
	std::index_t freeCount = 0;

	// Loop over each pointer
	for (std::index_t idx = 0; idx < allocations.dataSize; idx++)
	{
		// Read pointer
		void *ptr = __UNSAFE__(std::read)(&allocations, idx);

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

BBP::std::index_t BBP::std::ResourceManager::deleteAll()
{
	// Keep track of objects that were delete'd
	std::index_t deleteCount = 0;

	// Loop over each pointer
	for (std::index_t idx = 0; idx < objects.dataSize; idx++)
	{
		// Check if pointer exists
		if (!__UNSAFE__(std::read)(&objects, idx))
			continue;

		// Read pointer
		void *ptr = __UNSAFE__(std::read)(&objects, idx)->getRawPtr();

		// If ptr is nullptr, skip
		if (ptr == nullptr)
			continue;

		// Since this pointer has something, delete it. Also, since we know the index, just pass it.
		this->_delete(idx);
		deleteCount++;
	}

	// Return the amount of things delete'd
	return deleteCount;
}

BBP::std::index_t BBP::std::ResourceManager::dealloc()
{
	std::index_t count = 0;
	count += this->freeAll();
	count += this->deleteAll();
	return count;
}

BBP::std::ResourceManager::ResourceManager()
{
	// I should really remove that pesky lvalue restriction..... Oh well, idc
	void *nul = nullptr;
	AbstractGCEntry *nul2 = nullptr;

	// Zero initialize allocations
	for (std::index_t t = 0; t < allocations.dataSize; t++)
		write(&allocations, nul, t);

	// Zero initialize objects
	for (std::index_t t = 0; t < objects.dataSize; t++)
		write(&objects, nul2, t);

	// Set invalid indicies
	this->invalidAllocationIndex = allocations.dataSize;
	this->invalidObjectIndex = objects.dataSize;

	// Zero-init next available things
	this->nextAllocationAvailable = 0;
	this->nextObjectAvailable = 0;

}

/*
BBP::std::STATIC_PAGE<void *> BBP::std::page_allocations;
BBP::std::Stack<void *> BBP::std::allocationStack(&BBP::std::page_allocations);

void BBP::std::find_next_empty()
{
	// Loop over each index
	for (std::index_t idx = 0; idx < allocationStack.max_elements; idx++)
	{
		// Get the pointer at that location
		void *ptr = std::read(allocationStack.page, idx);

		// If pointer has something, continue
		if (ptr)
			continue;

		// Set index
		allocationStack.atElement = idx;
		return;
	}

	// There are no more available pointers
	std::STDERR << "Could not find an empty pointer to allocate memory to." <<= std::endl;

}

BBP::std::index_t BBP::std::find_pointer(void *ptr)
{
	// Loop over each pointer
	for (std::index_t idx = 0; idx < allocationStack.max_elements; idx++)
	{
		// Get pointer
		void *element = std::read(allocationStack.page, idx);

		// Compare ptr to element
		if (element == ptr)
			return idx; // Return index
	}

	// Nothing was found
	std::STDERR << "Could not find pointer" <<= std::endl;
	return allocationStack.max_elements;

}

BBP::std::size_t BBP::std::destroy_all()
{
	std::size_t count = 0;

	// Loop over everything
	for (std::index_t idx = 0; idx < allocationStack.max_elements; idx++)
	{
		// Get void
		void *ptr = std::read(allocationStack.page, idx);

		// If ptr is nullptr, skip
		if (ptr == nullptr)
			continue;

		// Count increase
		count++;

		// Free
		safe_free(ptr);
	}

	return count;
}

void *BBP::std::safe_malloc(std::size_t size)
{
	// Allocate data
	void *data = malloc(size);

	// Save data
	std::allocationStack << data;

	// Find next suitable spot
	std::find_next_empty();

	// Return malloc
	return data;
}

void *BBP::std::safe_calloc(std::size_t count, std::size_t size)
{
	// Allocate data
	void *data = calloc(count, size);

	// Save data
	std::allocationStack << data;

	// Find next suitable spot
	std::find_next_empty();

	// Return malloc
	return data;
}

void BBP::std::safe_free(void *ptr)
{
	// Find index
	index_t ptrIndex = std::find_pointer(ptr);

	// If ptrIndex is allocationStack.max_elements, return.
	if (ptrIndex == allocationStack.max_elements)
		return;

	// Free data
	free(ptr);

	// Write nullptr at index
	std::write(allocationStack.page, (void *)nullptr, ptrIndex);

	// Move pointer to index
	allocationStack.atElement = ptrIndex;
}

*/