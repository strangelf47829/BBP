#ifndef BBP_STD_ITERATOR_H
#define BBP_STD_ITERATOR_H

#include "Memory.h"

// An iterator differs from a stack because of the following:
// An iterator is used to loop over a collection of items, a stack is used to create one.

namespace BBP
{
	namespace std
	{

		template <typename T>
		class Iterator
		{
			// The internal counter used to keep track of iterators position
			index_t internal_counter;

			// Address of iterator used, can be used to shuffle iterators back and forth
			index_t *counter;

			// Page used by the iterator
			PAGE<T> iteratorData;

		public:

			// Constructors
			Iterator() = delete;
			Iterator(PAGE<T>);

			// Set iterator counter
			void setIteratorCounter(index_t *);

			// Get next item
			bool getNext(T &);

		};


	}
}


#endif