#ifndef BBP_STDLIB_SEQMEM_H
#define BBP_STDLIB_SEQMEM_H

#include "Resources.h"

namespace BBP
{
	namespace std
	{

		// Sequential memory is made up of two parts: The part that actually stores the memory (indexed memory), and the sequential traverser.
		// The sequential memory class contains both, and is used to index a part.

		template<typename T>
		class IndexedPageMemory : protected pageAllocator
		{
		protected:

			// Static page to store order (the value stored at an index indicates the next index). A value of 0 indicates no next page.
			STATIC_PAGE<index_t, max_allocations> order;

			// Active 'T' page (converted from allocator), and its index
			PAGE<T> activePage;
			index_t activeIndex;

		public:

			// Set active page 
			void setActivePage(index_t);

			// Move to next page
			void stepActivePage();
		
			// Set next thingy for current active index
			void setNextIndex();
			void setNextIndex(index_t);

			// Allocate sequentially
			T *sequentialMalloc(size_t);
			T *sequentialCalloc(size_t, size_t);
			
			// Moves to the last used frame. Returns false on failure.
			bool moveToLast();

			// Moves to the first free frame after the last used frame. Returns false on failure.
			bool moveToFree();

			// Free a pointer, and close up the thingies
			void sequentialFree(T *);

			// Free everything
			void freeEverything();

			// Templated constructor
			IndexedPageMemory()
			{
				// Set all values of 'order' to 0
				for (std::index_t idx = 0; idx < order.dataSize; idx++)
					order[idx] = 0;
			}

		};

		template<typename T>
		class SequentialMemory : protected IndexedPageMemory<T>, public virtual VPAGE<T>
		{

			// Previous and next pages
			PAGE<T> *_nextPage;
			PAGE<T> *_prevPage;

			// For L-Value binding
			size_t dataSizeLV;
			size_t bytesLV;

		public:

			T *Malloc(size_t size) { return this->sequentialMalloc(size); }
			T *Calloc(size_t count, size_t size) { return this->sequentialCalloc(count, size); }

			void Free(T *ptr) { this->sequentialFree(ptr); }
			void freeAllData() { this->freeEverything(); }

			virtual size_t &dataSize() override;
			virtual size_t &bytes() override;

			virtual PAGE<T> *&prevPage() { return _prevPage; }
			virtual PAGE<T> *&nextPage() { return _nextPage; }

			virtual T *&data() { /* INVALID */ throw std::exception("Cannot retrieve 'data' from Sequential memory.", EBADR); }

			virtual T &operator[](std::index_t idx) override;

			// Hashing function - Deleted function - Needs to be explicitly specialized.
			virtual explicit operator hash_t () const = delete;

			// Comparison function - Deleted function - Needs to be explicitly specialized.
			virtual bool operator !=(std::PAGE<T> &b) = delete;
			virtual bool operator ==(std::PAGE<T> &b) = delete;

		};



	}
}

#include "IndexedPageMemory.hxx"
#include "SequentialMemory.hxx"

#endif