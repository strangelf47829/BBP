#ifndef BBP_STDLIB_STACK_HXX
#define BBP_STDLIB_STACK_HXX

#include "Signal.h"
#include "Stack.h"
#include "Resources.h"

namespace BBP
{
	namespace std
	{
		template<typename K> // Write one element
		inline Stack<K> &BBP::std::Stack<K>::operator++()
		{
			this->atElement++;
			return *this;
		}

		template<typename K> // Read one element
		inline Stack<K> &BBP::std::Stack<K>::operator--()
		{
			this->atElement--;
			return *this;
		}

		template<typename K>
		inline Stack<K> &BBP::std::Stack<K>::operator<<(K value)
		{
			// Check if stack has space
			if (atElement >= max_elements)
				throw std::exception("Cannot add value to stack: maximum elements reached.", EPERM);

			// Write to page
			std::write(page, value, atElement);

			// Advance one position
			this->operator++();

			return *this;
		}

		template<typename K>
		inline Stack<K> &BBP::std::Stack<K>::operator>>(K &value)
		{
			// Check if stack has anything
			if (atElement == 0)
				throw std::exception("Cannot read from stack: stack is empty.", EPERM);

			// Decrement by one position
			this->operator--();

			// Read value
			value = std::read(page, atElement);

			return *this;
		}

		template<typename K>
		inline Stack<K> &BBP::std::Stack<K>::operator<<(Stack<K> &value)
		{
			char c = 0;
			value >> c;
			*this << c;
			return *this;
		}

		template<typename K>
		inline Stack<K> &BBP::std::Stack<K>::operator>>(Stack<K> &value)
		{
			char c = 0;
			*this >> c;
			value << c;
			return *this;
		}

		template<typename K>
		inline Stack<K> &BBP::std::Stack<K>::operator<=(K value)
		{
			// Write to page without safety
			__UNSAFE__(std::write)(page, value, atElement);

			// Advance one position
			this->operator++();

			return *this;
		}

		template<typename K>
		inline Stack<K> &BBP::std::Stack<K>::operator>=(K &value)
		{
			// Decrement by one position
			this->operator--();

			// Read value without safety
			value = __UNSAFE__(std::read)(page, atElement);

			return *this;
		}

		template<typename K>
		inline K &BBP::std::Stack<K>::operator[](std::index_t access)
		{
			return read(this->page, access);
		}


		template<typename K>
		inline K &BBP::std::Stack<K>::operator+=(std::s_offset_t offset)
		{
			// Check bounds of offset
			if ((atElement < offset && offset < 0) || atElement + offset >= max_elements)
				throw std::exception("Cannot peek element from stack: offset index exceeds stack page bounds.", EPERM);

			return std::read(page, atElement + offset);
		}

		template<typename K>
		inline K &BBP::std::Stack<K>::operator%=(std::s_offset_t offset)
		{
			// Just read unsafely and return directly.
			return __UNSAFE__(std::read)(page, atElement + offset);
		}

		template<typename K>
		inline void BBP::std::Stack<K>::Extend(std::ResourceManager *allocator, std::index_t by)
		{
			// Create temporary string
			std::PAGE<K> temp(*page);

			// Save size
			size_t oldSize = max_elements;

			// Now create a new page
			*page = std::PAGE<K>(page->dataSize + by, (K *)allocator->calloc(page->dataSize + by, sizeof(K)));

			// Copy old data into data
			memcpy(temp, *page, oldSize);

			// Then free old string
			allocator->free(temp.data);

			// Then save the prev and next pages
			page->prevPage = temp.prevPage;
			page->nextPage = temp.nextPage;

			// Update stack length
			max_elements = page->dataSize;
		}

		template<typename K>
		inline void BBP::std::Stack<K>::foreach(Iterator lambda)
		{
			for (std::index_t index = 0; index < atElement; index++)
				lambda(read(page,index));
		}

		template<typename K>
		inline bool BBP::std::Stack<K>::all(Predicate lambda)
		{
			bool result = true;
			for (std::index_t index = 0; index < atElement; index++)
				result &= lambda(read(page, index));
			return result;
		}

		template<typename K>
		inline bool BBP::std::Stack<K>::any(Predicate lambda)
		{
			bool result = true;
			for (std::index_t index = 0; index < atElement; index++)
				result |= lambda(read(page, index));
			return result;
		}


	}
}

#endif