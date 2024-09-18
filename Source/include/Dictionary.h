#ifndef BBP_STD_DICTIONARY_H
#define BBP_STD_DICTIONARY_H

#include "Hashmap.h"
#include "Concepts.h"

namespace BBP
{
	namespace std
	{

		// Using specifier
		template<Identifiable Key_t, Identifiable Value_t>
		struct DictionaryKeyValuePair
		{
			Key_t key;
			Value_t value;
		};

		// Virtual class used to ...
		template<Identifiable Key_t, Identifiable Value_t>
		class Dictionary
		{
		public:

			// Add stuff to the dictionary
			virtual void add(Key_t, Value_t) = 0;

			// Get stuff from the dictionary
			virtual Value_t &get(Key_t &) = 0;

			// Same as the get function
			virtual Value_t &operator[](Key_t &idx) = 0;

			// Check if dictionary contains value or key
			virtual bool containsKey(Key_t &) = 0;
			virtual bool containsValue(Value_t &) = 0;

			// Remove stuff
			virtual void remove(Key_t &&) = 0;

			// Remove everything
			virtual void Reset() = 0;

		};

		// A hashed dictionary trades space for speed.
		template<Identifiable Key_t, Identifiable Value_t>
		class HashedDictionary : public virtual Dictionary<Key_t, Value_t>
		{
			// The amount of frames per allocated page.
			static constexpr std::size_t FramesPerAllocation = (MAX_HALFWORD + 1) / max_allocations;
			static_assert(FramesPerAllocation *max_allocations > MAX_HALFWORD, "Dictionary cannot store enough frames to satisfy 'MAX_HALFWORD' chain count.");

			// A frame is what the dictionary uses to 
			using Dictionary_Frame_t = DictionaryKeyValuePair<Key_t, Value_t>;

			// A page of these frames
			PAGE<Dictionary_Frame_t> Frames;

			// A hashmap to track indicies
			Hashmap Indicies;

			// The resource allocator
			ResourceManager allocator;

			// Get the maximum available index
			std::index_t maxIndex;

			// Allocate more space if more is needed
			void allocateMoreValues();

		public:

			// Constructor
			HashedDictionary();

			// Add stuff to the dictionary
			void add(Key_t, Value_t) override;

			// Get stuff from the dictionary
			Value_t &get(Key_t &) override;

			// Same as the get function
			Value_t &operator[](Key_t &idx) override;

			// Check if dictionary contains value or key
			bool containsKey(Key_t &) override;
			bool containsValue(Value_t &) override;

			// Remove stuff
			void remove(Key_t &&) override;

			// Remove everything
			void Reset();

			// Clear resources
			~HashedDictionary();

		};

		// A compact dictionary trades speed for space
		template<Identifiable Key_t, Identifiable Value_t>
		class CompactDictionary : public virtual Dictionary<Key_t, Value_t>
		{
			// The amount of frames per allocated page.
			static constexpr std::size_t allocationsPerFrame = (MAX_HALFWORD + 1) / max_allocations;
			static_assert(allocationsPerFrame *max_allocations > MAX_HALFWORD, "Dictionary cannot store enough frames to satisfy 'MAX_HALFWORD' chain count.");

			// A frame is what the dictionary uses to 
			using Dictionary_Frame_t = DictionaryKeyValuePair<Key_t, Value_t>[allocationsPerFrame];

			// A page of these frames
			PAGE<Dictionary_Frame_t> Frames;

			// The resource allocator
			pageAllocator allocator;
		};

	}
}

#include "Dictionary.hxx"

#endif