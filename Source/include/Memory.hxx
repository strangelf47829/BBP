#include "Except.h"
#include "Memory.h"

#ifndef BBP_STDLIB_MEMORY_HXX
#define BBP_STDLIB_MEMORY_HXX


template <typename T>
inline void __UNSAFE__(BBP::std::write)(PAGE<T> *page, T &data, index_t index)
{
	// Check if index is within bounds, and if next page is not null
	if (index >= page->dataSize && page->nextPage)
		__UNSAFE__(write)(page->nextPage, data, index - page->dataSize);
	else
		page->data[index] = data;
}

template <typename T>
inline void __UNSAFE__(BBP::std::write_a)(PAGE<T> *page, T *data, index_t index, size_t amount)
{
	for (index_t idx = 0; idx < amount; idx++)
		__UNSAFE__(std::write)(page, data[idx], index + idx);
}

template <typename T>
inline void __UNSAFE__(BBP::std::write_r)(PAGE<T> *page, mem_t &data, index_t index)
{
	// Check if index is within bounds, and if next page is not null
	if (index > page->dataSize && page->nextPage)
		__UNSAFE__(write_r)(page->nextPage, data, index - page->dataSize);
	else
		page->raw[index] = data;
}


template <typename T>
void BBP::std::write(PAGE<T> *page, T &data, index_t index)
{
	// If page is nullptr, throw error
	if (page == nullptr)
		throw BBP::std::exception("Cannot write to page: page is null", EINVAL);

	// Get sequence size of page.
	size_t page_size = std::seqlen(*page);

	// If requested index is too large, throw error
	if (index >= page_size)
		throw std::exception("Cannot write to page: index exceeds page bounds.", ENOBUFS);

	// Copy over data. We can use unsafe, since we already validated everything.
	__UNSAFE__(write)(page, data, index);
}

template <typename T>
void BBP::std::write_r(PAGE<T> *page, mem_t &data, index_t index)
{
	// If page is nullptr, throw error
	if (page == nullptr)
		throw BBP::std::exception("Cannot write to page: page is null.", EINVAL);

	// Get sequence size of page.
	size_t page_size = std::seqlen_r(page);

	// If requested index is too large, throw error
	if (index >= page_size)
		throw std::exception("Cannot write to page: index is out of bounds.", ENOBUFS);

	// Copy over data. We can use unsafe, since we already validated everything.
	__UNSAFE__(write_r)(page, data, index);
}

template <typename T>
void BBP::std::write_a(PAGE<T> *page, T *data, index_t index, size_t amount)
{
	// If page is nullptr, throw error
	if (page == nullptr)
		throw std::exception("Cannot write to page: page is null.", EINVAL);

	// If data is invalid, throw error
	if (data == nullptr)
		throw std::exception("Cannot write to page: attempt to write null.", EINVAL);

	// Get sequence size of page.
	size_t page_size = std::seqlen(*page);

	// If amount requested is too large, throw error
	if (amount + index > page_size)
		throw std::exception("Cannot write to page: offset index is out of bounds.", ENOBUFS);

	// Copy over data. We can use unsafe, since we already validated everything.
	for (index_t idx = 0; idx < amount; idx++)
		__UNSAFE__(std::write)(page, data[idx], idx + index);
}

template <typename T>
void BBP::std::write_ar(PAGE<T> *page, mem_t *data, index_t index, size_t amount)
{
	// If page is nullptr, throw error
	if (page == nullptr)
		throw std::exception("Cannot write to page: page is null.", EINVAL);

	// If data is invalid, throw error
	if (data == nullptr)
		throw std::exception("Cannot write to page: attempt to write null.", EINVAL);

	// Get raw sequence size of page.
	size_t page_size = std::seqlen_r(page);

	// If amount requested is too large, throw error
	if (amount + index > page_size)
		throw std::exception("Cannot write to page: offset index is out of bounds.", ENOBUFS);

	// Copy over data. We can use unsafe, since we already validated everything.
	for (index_t idx = 0; idx < amount; idx++)
		__UNSAFE__(std::write_r)(page, data[idx], idx + index);
}


template <typename T>
inline T &__UNSAFE__(BBP::std::read)(PAGE<T> *page, index_t index)
{
	// Check if index is within bounds, and if next page is not null
	if (index >= page->dataSize && page->nextPage)
		return __UNSAFE__(read)(page->nextPage, index - page->dataSize);
	else
		return page->data[index];
}

template <typename T>
inline BBP::std::mem_t &__UNSAFE__(BBP::std::read_r)(PAGE<T> *page, index_t index)
{
	// Check if index is within bounds, and if next page is not null
	if (index >= page->dataSize && page->nextPage)
		return __UNSAFE__(read_r)(page->nextPage, index - page->dataSize);
	else
		return page->raw[index];
}

template <typename T>
inline T *__UNSAFE__(BBP::std::read_a)(PAGE<T> *page, index_t index)
{
	return page->data + index;
}

template <typename T>
inline BBP::std::mem_t *__UNSAFE__(BBP::std::read_ar)(PAGE<T> *page, index_t index)
{
	return page->raw + index;
}

template <typename T>
T &BBP::std::read(PAGE<T> *page, index_t index)
{
	// If source is invalid, throw error
	if (page == nullptr)
		throw std::exception("Cannot read from page: page is null.", EINVAL);

	// Get size of page
	size_t pageSize = std::seqlen(*page);

	// If index is out of bounds, throw error
	if (index >= pageSize)
		throw std::exception("Cannot read from page: index exceeds page bounds.", ERANGE);

	// Return read data
	return __UNSAFE__(std::read)(page, index);
}

template <typename T>
BBP::std::mem_t &BBP::std::read_r(PAGE<T> *page, index_t index)
{
	// If source is invalid, throw error
	if (page == nullptr)
		throw std::exception("Cannot read from page: page is null.", EINVAL);

	// Get size of page
	size_t pageSize = std::seqlen_r(*page);

	// If index is out of bounds, throw error
	if (index >= pageSize)
		throw std::exception("Cannot read from page: index exceeds page bounds.", ERANGE);

	// Return read data
	return __UNSAFE__(std::read_r)(page, index);
}

template <typename T>
T *BBP::std::read_a(PAGE<T> *page, index_t index)
{
	// If source is invalid, throw error
	if (page == nullptr)
		throw std::exception("Cannot read from page: page is null.", EINVAL);

	// Get size of page
	size_t pageSize = std::seqlen(*page);

	// If index is out of bounds, throw error
	if (index >= pageSize)
		throw std::exception("Cannot read from page: index exceeds page bounds.", ERANGE);

	// Return read data
	return __UNSAFE__(std::read_a)(page, index);
}

template <typename T>
BBP::std::mem_t *BBP::std::read_ar(PAGE<T> *page, index_t index)
{
	// If source is invalid, throw error
	if (page == nullptr)
		throw std::exception("Cannot read from page: page is null.", EINVAL);

	// Get size of page
	size_t pageSize = std::seqlen_r(*page);

	// If index is out of bounds, throw error
	if (index >= pageSize)
		throw std::exception("Cannot read from page: index exceeds page bounds.", ERANGE);

	// Return read data
	return __UNSAFE__(std::read_ar)(page, index);
}


template<typename T>
BBP::std::size_t __UNSAFE__(BBP::std::seqlen)(std::PAGE<T> &page)
{
	// Declare pointer which will traverse tree
	std::PAGE<T> *ptr = page;

	// Move to back of chain
	while (ptr->prevPage)
		ptr = ptr->prevPage;

	// Declare runningSize
	size_t runningSize = 0;

	while (ptr)
	{
		runningSize += ptr->dataSize;
		ptr = ptr->nextPage;
	}

	return runningSize;
}

template<typename T>
BBP::std::size_t __UNSAFE__(BBP::std::seqlen_r)(std::PAGE<T> &page)
{
	// Declare pointer which will traverse tree
	std::PAGE<T> *ptr = page;

	// Move to back of chain
	while (ptr->prevPage)
		ptr = ptr->prevPage;

	// Declare runningSize
	size_t runningSize = 0;

	while (ptr)
	{
		runningSize += ptr->bytes;
		ptr = ptr->nextPage;
	}

	return runningSize;
}

template<typename T>
BBP::std::PAGE<T> &BBP::std::last(std::PAGE<T> &page)
{
	// Define pointer which shall traverse the tree
	std::PAGE<T> *ptr = &page;

	// Define variable that will prevent infinite loop issues
	word tries = 0;

	// Keep going until last page is found.
	while (ptr->nextPage && ++tries <= BBP::std::max_page_sequence_length)
		ptr = ptr->nextPage;

	// If tries found nothing, throw error
	if (tries > BBP::std::max_page_sequence_length)
		throw std::exception("Failed to calculate page sequence length: exhausted tries during backwards page search.", ELNRNG);

	// Otherwise, return *ptr
	return *ptr;
}

template<typename T>
BBP::std::PAGE<T> &BBP::std::first(std::PAGE<T> &page)
{
	// Define pointer which shall traverse the tree
	std::PAGE<T> *ptr = &page;

	// Define variable that will prevent infinite loop issues
	word tries = 0;

	// Keep going until first page is found.
	while (ptr->prevPage && ++tries <= BBP::std::max_page_sequence_length)
		ptr = ptr->prevPage;

	// If tries found nothing, throw error
	if (tries > BBP::std::max_page_sequence_length)
		throw std::exception("Failed to calculate page sequence length: exhausted tries during backwards page search.", ELNRNG);

	// Otherwise, return *ptr
	return *ptr;
}

template<typename T>
BBP::std::size_t BBP::std::seqlen(std::PAGE<T> &page)
{
	// Define pointer which shall traverse the tree
	std::PAGE<T> *ptr = &page;

	// Define variable that will prevent infinite loop issues
	word tries = 0;

	// Keep going until first page is found.
	while (ptr->prevPage && ++tries <= BBP::std::max_page_sequence_length)
		ptr = ptr->prevPage;

	// If tries found nothing, throw error
	if (tries > BBP::std::max_page_sequence_length)
		throw std::exception("Failed to calculate page sequence length: exhausted tries during backwards page search.", ELNRNG);

	// Declare size variable, and reset tries
	size_t runningSize = 0;
	tries = 0;

	// Keep going until last page or tries exhausted
	while (ptr && ++tries <= BBP::std::max_page_sequence_length)
	{
		runningSize += ptr->dataSize;
		ptr = ptr->nextPage;
	}

	// If tries exhausted, the user cannot trust the outcome. Throw error
	if (tries > BBP::std::max_page_sequence_length)
		throw std::exception("Failed to calculate page sequence length: exhausted tries during forward page search.", ELNRNG);

	// Everything ok, just return here
	return runningSize;
}

template<typename T>
BBP::std::size_t BBP::std::seqlen_r(std::PAGE<T> &page)
{
	// Define pointer which shall traverse the tree
	std::PAGE<T> *ptr = &page;

	// Define variable that will prevent infinite loop issues
	word tries = 0;

	// Keep going until first page is found.
	while (ptr->prevPage &&++tries <= BBP::std::max_page_sequence_length)
		ptr = ptr->prevPage;

	// If tries found nothing, throw error
	if (tries > BBP::std::max_page_sequence_length)
		throw std::exception("Failed to calculate page sequence length: exhausted tries during backwards page search.", ELNRNG);

	// Declare size variable, and reset tries
	size_t runningSize = 0;
	tries = 0;

	// Keep going until last page or tries exhausted
	while (ptr &&++tries <= BBP::std::max_page_sequence_length)
	{
		runningSize += ptr->bytes;
		ptr = ptr->nextPage;
	}

	// If tries exhausted, the user cannot trust the outcome. Throw error
	if (tries > BBP::std::max_page_sequence_length)
		throw std::exception("Failed to calculate page sequence length: exhausted tries during forwards page search.", ELNRNG);

	// Everything ok, just return here
	return runningSize;
}


template <typename T>
void BBP::std::memcpy(PAGE<T> &src, PAGE<T> &dst, size_t amount)
{
	// Get source and destination lengths
	size_t srcLen = std::seqlen(src);
	size_t dstLen = std::seqlen(dst);

	// If copying too much data from src, throw error
	if (amount > srcLen)
		throw std::exception("Could not copy memory: amount requested is bigger than the size of source.", ENOBUFS);

	// If destination is smaller than requested amount, throw error
	if (amount > dstLen)
		throw std::exception("Could not copy memory: amount requested is bigger than the size of destination.", ENOBUFS);

	// Directly write
	for (std::index_t index = 0; index < amount; index++)
		__UNSAFE__(write)(&dst, __UNSAFE__(read)(&src, index), index);
}

template <typename T>
T &BBP::std::PAGE<T>::operator[](std::index_t idx)
{
	return std::read(this, idx);
}

#endif
