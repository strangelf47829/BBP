#ifndef BBP_STDLIB_DICTIONARY_HXX
#define BBP_STDLIB_DICTIONARY_HXX

#include "Dictionary.h"

template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
void BBP::std::HashedDictionary<Key_t, Value_t>::allocateMoreValues()
{
	// Allocate a new page
	PAGE<Dictionary_Frame_t> *newPage = allocator.add_object(new PAGE<Dictionary_Frame_t>());

	// Allocate new data
	allocator.page_calloc(*newPage, FramesPerAllocation);

	// Set end of data
	std::last(Frames).nextPage = newPage;

	// Increase maximum index
	maxIndex += FramesPerAllocation;
}

// Add an object to a hashed dictionary.
template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
void BBP::std::HashedDictionary<Key_t, Value_t>::add(Key_t key, Value_t value)
{
	// Generate key and value hash
	hash_t keyHash = (hash_t)key;
	hash_t valueHash = (hash_t)value;

	// Add key hash to hashmap
	std::index_t keyIndex = Indicies.add(keyHash);

	// While not enough space, allocate more
	while (keyIndex >= maxIndex)
		allocateMoreValues();

	// Then write 'value' and 'key' to appropriate spots
	DictionaryKeyValuePair<Key_t, Value_t> *kvp = &Frames[keyIndex];
	kvp->key = key;
	kvp->value = value;
}

template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
Value_t &BBP::std::HashedDictionary<Key_t, Value_t>::get(Key_t &key)
{
	return (*this)[key];
}

template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
Value_t &BBP::std::HashedDictionary<Key_t, Value_t>::operator[](Key_t &key)
{
	// Hash the key
	hash_t keyHash = (hash_t)key;

	// Lookup hash in hashmap
	Indicies.initiate(keyHash);

	// Get index
	std::index_t keyIndex = 0;

	do
	{
		// Get key index
		keyIndex = Indicies.iterate();

		// Check for valid keyIndex
		if (keyIndex == emptyHash)
			continue;

		// Get kvp
		DictionaryKeyValuePair<Key_t, Value_t> *kvp = &Frames[keyIndex];

		// Compare keys
		if (kvp->key != key)
			continue;

		// Key found, return value
		return kvp->value;

	} while (keyIndex);

	// None found.
	throw std::exception("Key not found", ENODATA);
}

// Just do stubs
template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
bool BBP::std::HashedDictionary<Key_t, Value_t>::containsKey(Key_t &key)
{
	// Hash the key
	hash_t keyHash = (hash_t)key;

	// Lookup hash in hashmap
	Indicies.initiate(keyHash);

	// Get index
	std::index_t keyIndex = 0;

	do
	{
		// Get key index
		keyIndex = Indicies.iterate();

		// Check for valid keyIndex
		if (keyIndex == emptyHash)
			continue;

		// Get kvp
		DictionaryKeyValuePair<Key_t, Value_t> *kvp = &Frames[keyIndex];

		// Compare keys
		if (kvp->key != key)
			continue;

		// Key found, return value
		return true;

	} while (keyIndex);

	// None found.
	return false;
}

template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
bool BBP::std::HashedDictionary<Key_t, Value_t>::containsValue(Value_t &value)
{
	// Look through each value manually.
	for (std::index_t idx = 0; idx < maxIndex; idx++)
	{
		// Get kvp
		DictionaryKeyValuePair<Key_t, Value_t> *kvp = &Frames[idx];

		// Compare
		if (value == kvp->value)
			return true;

	}

	// Default is false
	return false;
}

template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
void BBP::std::HashedDictionary<Key_t, Value_t>::remove(Key_t &&key)
{

}

// Clear out everything
template <BBP::std::Identifiable Key_t, BBP::std::Identifiable Value_t>
BBP::std::HashedDictionary<Key_t, Value_t>::~HashedDictionary()
{
	allocator.clearAll();
}

#endif