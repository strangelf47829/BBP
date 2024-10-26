#ifndef BBP_STD_HASHMAP_H
#define BBP_STD_HASHMAP_H

#include "Hashing.h"
#include "Resources.h"

namespace BBP
{
	namespace std
	{

		// Typenames to make things more legible
		using hashmap_key_t = hash_t;
		using hashmap_val_t = hash_t;

		// Hashtable entry
		struct HashMap_entry_t
		{
			// The hash stored at this index (The value)
			hashmap_val_t valueHash;

			// What index to lookup if this object is not it.
			std::index_t indexOfNextElementInChain;
		};

		// Hashmap
		class Hashmap
		{

			// Amount of bins
			static constexpr std::halfword binCount = 8;

			// Amount of elements per chain
			std::halfword chainCount;

			// A frame is what the hashmap uses to store at least enough for (binCount) objects.
			using Hashmap_frame_t = HashMap_entry_t[binCount];

			// Page of frames (Points to first frame)
			PAGE<Hashmap_frame_t> Frames;

			// The amount of frames allocated (not the amount of times frames have been allocated!!)
			std::halfword totalChainCount;

			// Resource allocator for this hashmap. At least 
			ResourceManager allocator;

			// The amount of frames allocated per allocation needs to be at least enough to allocate enough amount for each frame to contian enough chainElements 
			// To provide 0xFFFF chains in (max_allocations)
			static constexpr std::size_t framesPerAllocation = (0xFFFF + 1) / max_allocations;
			static_assert(framesPerAllocation *max_allocations > MAX_HALFWORD, "Hashmap cannot store enough frames to satisfy 'MAX_HALFWORD' chain count.");

			// Allocate a frame. Throws exception if not able to.
			void allocateNewFrame();

			// Return value at index
			HashMap_entry_t &getEntryForIndex(index_t);

			// Return value at index
			hashmap_val_t getValueForIndex(index_t);

			// Returns true if index is a valid index, returns false if index means 'null'
			bool isIndexValid(index_t);

		public:

			// The key is used to determine which entries are looked at.
			// The valueHash is then compared to the hash of the value stored at the current index.
			// If they match, good shit, stop looking.
			// It can happen, however, that the hash is the same but the objects are not.
			// In such case, the thing retrieving the object must recognize this and keep looking.
			// This is what the hashmap_iterator is for.

			struct Hashmap_iterator_t
			{
				// The bin this thing is searching through
				std::halfword binCount;

				// What index is currently being searched.
				std::index_t idx;

				// The hash stored at this thing
				hashmap_val_t result;
			};

		private:

			// Initialize a search.
			void start_search(Hashmap_iterator_t &, hashmap_key_t);

			// Step through, returns false if end of chain. Returns true otherwise
			bool stepIterator(Hashmap_iterator_t &);

			// Move to the absolute end of the chain, given a bin count
			void moveToEndOfChain(Hashmap_iterator_t &, halfword);

			// Move to the absolute end of the chain of a currently active iterator
			void moveToEndOfChain(Hashmap_iterator_t &);

			// Move once, then run until value hash equals the one stored (and return true), or end of chain (return false)
			bool moveUntilValue(Hashmap_iterator_t &, hashmap_val_t);

			// Move once, then run until NEXT value hash equals the one stored (and return true, iterator is one before value), or end of chain (return false, iterator is at end-of-chain)
			bool moveUntilBeforeValue(Hashmap_iterator_t &, hashmap_val_t);

			// Remove next iterator, returns true on success
			bool removeNextValue(Hashmap_iterator_t &);

			// Find empty index greedily, and creates more if none are available.
			std::index_t findNextEmptyIndex();

			// Append a value to the end of a chain, and return its index
			std::index_t appendValueToChain(halfword, hashmap_val_t);

			// internal iterator
			Hashmap_iterator_t internalIterator;

			// The value currently being searched for
			hashmap_val_t searchingFor;

		public:

			// Initiate a search for value
			void initiate(hashmap_val_t);

			// Move to next value, return index. Returns 0 if nothing found.
			std::index_t iterate();

			// Add a value to the hashmap, returns its index
			std::index_t add(hashmap_val_t);

			// Reset function
			void Reset();

			// Destructor frees all allocations
			~Hashmap();

		};

	}
}

#endif