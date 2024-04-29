#ifndef BBP_STDLIB_SEQPAGEMEM_HXX
#define BBP_STDLIB_SEQPAGEMEM_HXX

#include "SequentialMemory.h"

template<typename T>
void BBP::std::IndexedPageMemory<T>::setActivePage(index_t index)
{
	// Check for validity
	if (index >= allocations.dataSize || index >= allocationsSizes.dataSize)
		std::exception("Invalid active page", EBADE);

	// Set active index
	activeIndex = index;

	// Call constructor
	activePage = PAGE<T>(allocationsSizes[index] / (sizeof(T) / sizeof(byte)), (T *)allocations[index]);
}

template<typename T>
void BBP::std::IndexedPageMemory<T>::stepActivePage()
{
	// Get next active page
	std::index_t nextActive = order[activeIndex];

	// Load that
	setActivePage(nextActive);

}

template<typename T>
void BBP::std::IndexedPageMemory<T>::setNextIndex()
{
	// Set active index
	setNextIndex(nextAllocationAvailable);
}

template<typename T>
void BBP::std::IndexedPageMemory<T>::setNextIndex(index_t idx)
{
	// Set active index
	order[activeIndex] = idx;
}

template<typename T>
T *BBP::std::IndexedPageMemory<T>::sequentialMalloc(size_t size)
{
	// Get amount of bytes to allocate
	size_t bytes = size * (sizeof(T) / sizeof(byte));

	// Malloc amount of bytes
	void *allocated = malloc(bytes);

	// Move to next free page
	moveToFree();

	// Set next active page
	setNextIndex();

	// Step active page
	stepActivePage();

	// Return data
	return (T *)allocated;
}

template<typename T>
T *BBP::std::IndexedPageMemory<T>::sequentialCalloc(size_t count, size_t size)
{
	// Get amount of bytes to allocate
	size_t bytes = size * (sizeof(T) / sizeof(byte));

	// Malloc amount of bytes
	void *allocated = calloc(count, bytes);

	// Move to free
	moveToFree();

	// Set next active page
	setNextIndex();

	// Step active page
	stepActivePage();

	// Return data
	return (T *)allocated;
}

template<typename T>
bool BBP::std::IndexedPageMemory<T>::moveToLast()
{
	// Keep track of attempts
	std::index_t attemptCount = 0;

	// Step continuously until the next, next value is 0
	while (attemptCount++ < order.dataSize && order[order[activeIndex]] != 0)
		stepActivePage();

	// If attempts are exhausted, return false.
	if (attemptCount >= order.dataSize)
		return false;

	// Return true
	return true;
}

template<typename T>
bool BBP::std::IndexedPageMemory<T>::moveToFree()
{
	// Move to last
	bool able = moveToLast();

	// If error, return error
	if (able == false)
		return false;

	// Step once again.
	stepActivePage();

	// If next page is not 0, return false
	if (order[activeIndex] != 0)
		return false;

	// Return true
	return true;
}

template<typename T>
void BBP::std::IndexedPageMemory<T>::sequentialFree(T *ptr)
{
	// Free stuff from the page memory, get stuff
	std::index_t freedIndex = free(ptr);

	// Look for value that links to this index
	std::index_t prev = 0;
	bool found = false;

	for (std::index_t idx = 0; idx < order.dataSize && freedIndex; idx++)
	{
		// If different index, do nothing
		if (order[idx] != freedIndex)
			continue;

		// Set previous index
		prev = idx;

		// Set flag, then escape
		found = true;

		break;
	}

	// If not found, do nothing.
	if (found == false)
		return;

	// Get next index
	std::index_t next = order[freedIndex];

	// If freedIndex is 0, do nothing anymore.
	if (freedIndex == 0)
		return;

	// Link previous frame to next frame, then reset current frame.
	order[prev] = next;
	order[freedIndex] = 0;
}

template<typename T>
void BBP::std::IndexedPageMemory<T>::freeEverything()
{
	// Free everything internally
	freeAll();

	// Then reset order
	for (std::index_t idx = 0; idx < order.dataSize; idx++)
		order[idx] = 0;
}

#endif
