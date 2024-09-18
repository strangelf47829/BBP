#ifndef BBP_STDLIB_SEQMEM_HXX
#define BBP_STDLIB_SEQMEM_HXX

#include "SequentialMemory.h"

template<typename T>
BBP::std::size_t &BBP::std::SequentialMemory<T>::dataSize()
{
	// Keep track of active count
	std::size_t currentSize = 0;

	// Reset to 0
	this->setActivePage(0);

	// Do while ..
	do
	{
		// Increase count
		currentSize += this->activePage.dataSize;

		// Step page
		this->stepActivePage();

	} while (this->activeIndex != 0);

	// Return count
	dataSizeLV = currentSize;
	return dataSizeLV;
}

template<typename T>
BBP::std::size_t &BBP::std::SequentialMemory<T>::bytes()
{
	// Basically just a modified dataSize. IDKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK KILL YOURSELF
	bytesLV = dataSize() * (sizeof(T) / sizeof(byte));
	return bytesLV;
}

template<typename T>
T &BBP::std::SequentialMemory<T>::operator[] (std::index_t idx)
{
	// Reset to 0
	this->setActivePage(0);
	
	// Keep track of base
	std::size_t base = 0;

	// Look through each thing
	do
	{
		// Check if valid
		if (idx - base < this->activePage.dataSize)
			return this->activePage[idx - base];

		// Otherwise append page size to base
		base += this->activePage.dataSize;

		// Step
		this->stepActivePage();

	} while (this->activeIndex != 0);

	// Otherwise look through next page
	if (_nextPage)
		return (*_nextPage)[idx - base];

	// Error
	std::raise(SIGSEGV);
}

#endif
