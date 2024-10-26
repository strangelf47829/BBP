#include "../include/Hashmap.h"

void BBP::std::Hashmap::allocateNewFrame()
{
	// Allocate new page
	PAGE<Hashmap_frame_t> *newFrame = allocator.add_object(new PAGE<Hashmap_frame_t>());

	// Allocate new frames
	allocator.page_calloc(*newFrame, framesPerAllocation);

	// Get end of frames
	std::last(Frames).nextPage = newFrame;

	// Increase frame count
	totalChainCount += framesPerAllocation;
}

BBP::std::HashMap_entry_t &BBP::std::Hashmap::getEntryForIndex(BBP::std::index_t idx)
{
	// Calculate bin and frame index
	std::halfword frameIndex = idx / binCount;
	std::halfword binIndex = idx % binCount;

	// Return values
	Hashmap_frame_t &Frame = Frames[frameIndex];

	// Get frame stuff
	return Frame[binIndex];
}

BBP::std::hashmap_val_t BBP::std::Hashmap::getValueForIndex(BBP::std::index_t idx)
{
	// Return values
	return getEntryForIndex(idx).valueHash;
}

void BBP::std::Hashmap::start_search(Hashmap_iterator_t &iterator, hashmap_key_t key)
{
	// Set bin count
	iterator.binCount = binCount;

	// Set current index
	iterator.idx = key % iterator.binCount + iterator.binCount;

	// Check if index is valid
	while (iterator.idx >= totalChainCount)
		allocateNewFrame();

	// Set value to whatever is stored there
	iterator.result = getValueForIndex(iterator.idx);
}

bool BBP::std::Hashmap::stepIterator(Hashmap_iterator_t &iterator)
{
	// Get entry
	HashMap_entry_t *entry = &getEntryForIndex(iterator.idx);

	// Get entry at current index
	std::index_t nextIndex = entry->indexOfNextElementInChain;

	// Check if this index is 0, or itself
	if (isIndexValid(nextIndex) == false || nextIndex == iterator.idx)
		return false; // End of chain.

	// Move to that index
	iterator.idx = nextIndex;

	// Also retrieve the value at that index
	iterator.result = getValueForIndex(nextIndex);

	// Return true
	return true;
}

void BBP::std::Hashmap::Reset()
{
	// Clear all allocator data
	allocator.clearAll();

	// Then reset all other values
	this->Frames = std::PAGE<std::Hashmap::Hashmap_frame_t>(0, nullptr);

	// Then reset totalChain count
	totalChainCount = 0;
}

BBP::std::Hashmap::~Hashmap()
{
	Reset();
}

void BBP::std::Hashmap::moveToEndOfChain(Hashmap_iterator_t &iterator, halfword bin)
{
	// Initialize iterator
	start_search(iterator, bin);

	// Move until end
	while (stepIterator(iterator));
}

void BBP::std::Hashmap::moveToEndOfChain(Hashmap_iterator_t &iterator)
{
	// Move until end
	while (stepIterator(iterator));
}

bool BBP::std::Hashmap::moveUntilValue(Hashmap_iterator_t &iterator, hashmap_val_t value)
{
	// Check if iterator has returned a value
	bool hasValue = false;

	// Max attempts
	std::word maxAttempts = 0x10000;
	std::word attempts = 0x0;

	// Check if currently at value
	if (iterator.result == value)
	{
		// Step once
		stepIterator(iterator);

		// Return true
		return true;
	}

	// Step at least once
	do
	{
		// Step iterator, once to make sure that 
		hasValue = stepIterator(iterator);

		// If has no value, return false
		if (hasValue == false)
			return false;

		// Check if value is the same
		if (iterator.result == value)
			return true;

	} while (attempts++ < maxAttempts);

	// If exhausted attempts, throw error
	throw std::exception("Hashmap exhausted search attempts", EBADFD);
}

bool BBP::std::Hashmap::moveUntilBeforeValue(Hashmap_iterator_t &iterator, hashmap_val_t value)
{
	// Check if iterator has returned a value
	bool hasValue = false;

	// Max attempts
	std::word maxAttempts = 0x10000;
	std::word attempts = 0x0;

	// The last index set
	std::index_t lastIndex = iterator.idx;
	hashmap_val_t lastValue = iterator.result;

	// Step at least once
	do
	{
		// Step iterator
		hasValue = stepIterator(iterator);

		// If has no value, return false
		if (hasValue == false)
			return false;

		// Check if value is the same as result.
		if (iterator.result == value)
		{
			// Step back once.
			iterator.idx = lastIndex;
			iterator.result = lastValue;

			// Return found.
			return true;
		}

		// Set lastIndex and lastValue
		lastIndex = iterator.idx;
		lastValue = iterator.result;

	} while (attempts++ < maxAttempts);

	// If exhausted attempts, throw error
	throw std::exception("Hashmap exhausted search attempts", EBADFD);

}

bool BBP::std::Hashmap::removeNextValue(Hashmap_iterator_t &iterator)
{
	// Get current entry
	HashMap_entry_t *currentEntry = &getEntryForIndex(iterator.idx);

	// Step next
	bool CouldStepOnce = stepIterator(iterator);

	// If could not step, return false
	if (CouldStepOnce == false)
		return false;

	// If could step, get next entry
	HashMap_entry_t *nextEntry = &getEntryForIndex(iterator.idx);

	// Step next
	bool CouldStepTwice = stepIterator(iterator);

	// If could not step again, return false
	if (CouldStepTwice == false)
		return false;

	// 'Remove' nextEntry by making currentEntry point to index of nextElementInChain
	currentEntry->indexOfNextElementInChain = iterator.idx;

	// Invalidate nextEntry
	nextEntry->indexOfNextElementInChain = 0;
	nextEntry->valueHash = emptyHash;

	// Return success
	return true;
}

bool BBP::std::Hashmap::isIndexValid(index_t index)
{
	// If index is less than bincount, return false
	if (index < binCount)
		return false;

	// Return true.
	return true;
}

BBP::std::index_t BBP::std::Hashmap::findNextEmptyIndex()
{
	// Possible optimization: Since we can check which indicies point to something, we already known which indicies contain
	// Something before 'index' reaches it.

	// Start at bincount, since any index below 'bincount' is considered to mean 'invalid'
	std::index_t index = binCount;

	// Get pointer to frames
	Hashmap_frame_t *frame;
	std::index_t frameIndex = 1;
	
	// Search for thingy
	while (index < totalChainCount)
	{
		// Get frame pointer
		frame = &Frames[frameIndex];

		// Look for each thing in frame
		for (std::index_t subindex = 0; subindex < binCount; subindex++)
		{
			// Get entry
			HashMap_entry_t *entry = &(*frame)[subindex];

			// If entry is not empty, increase
			if ((entry->valueHash == emptyHash) == false)
			{
				index++;
				continue;
			}

			// Empty index, return index
			return index;
		}
	}

	// Exhausted indicies, none of the current entries are valid, so allocate more frames.
	allocateNewFrame();

	// Return index as is since index now points to new empty frame.
	return index;
}

BBP::std::index_t BBP::std::Hashmap::appendValueToChain(std::halfword bin, hashmap_val_t value)
{
	// Create iterator
	Hashmap_iterator_t iterator;

	// Move to end of bin
	start_search(iterator, bin);

	// Move to end of chain
	moveToEndOfChain(iterator);

	// Get iterator index
	std::index_t chainEndIndex = iterator.idx;

	// Look for empty index
	std::index_t emptyIndex = findNextEmptyIndex();

	// If these two values are the same, set write values to emptyIndex and return.
	if (chainEndIndex == emptyIndex)
	{
		// Write value then dip.
		getEntryForIndex(emptyIndex).valueHash = value;

		return emptyIndex;
	}

	// Set index of next element in chain for end of chain to empty index, only if empty index is not chain end index
	getEntryForIndex(chainEndIndex).indexOfNextElementInChain = emptyIndex;
	
	// Now set value
	getEntryForIndex(emptyIndex).valueHash = value;
	
	// Set that
	return emptyIndex;
}

void BBP::std::Hashmap::initiate(hashmap_val_t value)
{
	// Set search
	searchingFor = value;

	// Start search using iterator
	start_search(internalIterator, value);
}

BBP::std::index_t BBP::std::Hashmap::iterate()
{
	// Move to next value
	bool found = moveUntilValue(internalIterator, searchingFor);

	// If not found, return empty index.
	if (found == false)
		return 0;

	// Return that index
	return internalIterator.idx;
}

BBP::std::index_t BBP::std::Hashmap::add(hashmap_val_t value)
{
	// Return append value
	return appendValueToChain(value % binCount, value);
}