#define USE_32_BITS

#include "../include/Resources.h"
#include "../include/stdio.h"
#include "../include/Signal.h"
#include "../include/Kernel.h"

BBP::std::objectAllocator::objectAllocator()
{
	// I should really remove that pesky lvalue restriction..... Oh well, idc
	void *nul = nullptr;
	AbstractGCEntry *nul2 = nullptr;

	// Zero initialize objects
	for (std::index_t t = 0; t < objects.dataSize; t++)
		write(&objects, nul2, t);

	// Set invalid indicies
	this->invalidObjectIndex = objects.dataSize;

	// Zero-init next available things
	this->nextObjectAvailable = 0;
}

void BBP::std::objectAllocator::mark_deleted(void *ptr)
{
	// Check if pointer is not actually nullptr
	if (ptr == nullptr)
	{
		// Attempting to free nullptr: Do nothing but output something to stderr
		//system::kernelSS()->activeContext->STDERR << "Attempt to delete nullptr was made." <<= std::endl;
		return;
	}

	// Attempt to find index of pointer
	std::index_t ptrIndex = find_Object_pointer(ptr);

	// Write nullptr to index
	delete(this->objects.data[ptrIndex]);
	this->objects.data[ptrIndex] = nullptr;
}

bool BBP::std::objectAllocator::_delete(void *ptr)
{
	// Check if pointer is not actually nullptr
	if (ptr == nullptr)
	{
		// Attempting to free nullptr: Do nothing but output something to stderr
		//system::kernelSS()->activeContext->STDERR << "Attempt to delete nullptr was made." <<= std::endl;
		return false;
	}

	// Attempt to find index of pointer
	std::index_t ptrIndex = find_Object_pointer(ptr);

	// Call to internal _delete function
	this->_delete(ptrIndex);

	// Success
	return true;
}

void BBP::std::objectAllocator::_delete(std::index_t ptrIndex)
{
	// If index invalid, throw sigsegv signal
	if (ptrIndex == this->invalidObjectIndex)
		std::raise(std::SIGSEGV);

	// Pointer exists, so delete it.
	this->objects.data[ptrIndex]->_delete();
	delete(this->objects.data[ptrIndex]);
	this->objects.data[ptrIndex] = nullptr;
}

BBP::std::index_t BBP::std::objectAllocator::find_next_empty_object()
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
		if (this->objects.data[this->nextObjectAvailable + 1] == nullptr)
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

BBP::std::index_t BBP::std::objectAllocator::find_Object_pointer(void *ptr)
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

BBP::std::index_t BBP::std::objectAllocator::deleteAll()
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