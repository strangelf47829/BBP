#ifndef BBP_STDLIB_RESOURCES_H
#define BBP_STDLIB_RESOURCES_H

#include "Memory.h"
#include "Stack.h"

namespace BBP
{
	namespace std
	{

		extern void ext_free(void *);
		extern void *ext_malloc(std::size_t);
		extern void *ext_calloc(std::size_t, std::size_t);

		static constexpr size_t max_objects = 1000;
		static constexpr size_t max_allocations = 1000;

		struct AbstractGCEntry
		{
			virtual ~AbstractGCEntry()
			{};
			virtual void _delete() = 0;
			virtual void markDeleted() = 0;
			virtual void *getRawPtr() = 0;
		};

		template<typename T>
		class GCEntry : public AbstractGCEntry
		{
		public:
			T *ptr;
			virtual ~GCEntry()
			{
			}
			void markDeleted()
			{
				ptr = nullptr;
			}
			void _delete()
			{
				delete(ptr);
			}
			void *getRawPtr()
			{
				return (void *)ptr;
			}
		};

		class ResourceManager
		{
		public:

			ResourceManager();

			void *malloc(std::size_t);
			void *calloc(std::size_t, std::size_t);

			std::size_t totalAllocations = 0;

			template <typename T>
			inline T *add_object(T *ptr)
			{

				// See if allocation is possible
				if (this->nextObjectAvailable == this->invalidObjectIndex)
					throw std::exception("Could not register new'd data to Resource manager: Out of allocation space.", ENOMEM);

				// Store that write into next possible object

				GCEntry<T> *entry = new GCEntry<T>();
				entry->ptr = ptr;
				this->objects.data[this->nextObjectAvailable] = entry;
				totalAllocations += sizeof(T);

				// Find next possible location
				this->nextObjectAvailable = find_next_empty_object();

				// Return ptr, in case it is needed for something
				return ptr;
			}
			void add_alloc(void *);

			void free(void *);

			void mark_deleted(void *);
			void _delete(void *);

			std::index_t freeAll();
			std::index_t deleteAll();
			std::index_t dealloc();

			// allocation tools for pages
			template<typename T>
			inline void page_malloc(PAGE<T> &page, size_t count)
			{
				// Allocate from this
				T *data = (T *)this->malloc(count);

				// If nullptr, set page to 0 data
				if (data == nullptr)
				{
					page = PAGE<T>(0, nullptr);
					return;
				}

				// Otherwise, set data
				page = PAGE<T>(count, data);
			}

			template<typename T>
			inline void page_calloc(PAGE<T> &page, size_t count, size_t size)
			{
				// Allocate from this
				T *data = (T *)this->calloc(count, size);

				// If nullptr, set page to 0 data
				if (data == nullptr)
				{
					page = PAGE<T>(0, nullptr);
					return;
				}

				// Otherwise, set data
				page = PAGE<T>(count, data);
			}

			template<typename T>
			inline void page_calloc(PAGE<T> &page, size_t count)
			{
				page_calloc(page, count, sizeof(T));
			}

		private:

			// Internal functions to quickly free / delete data at known indicies
			void free(void *, std::index_t);
			void _delete(std::index_t);

			// Searches in the allocations and object page respectively.
			std::index_t find_next_empty_alloc();
			std::index_t find_next_empty_object();

			// Searches in the 'allocations' and 'new' 
			std::index_t find_Alloc_pointer(void *);
			std::index_t find_Object_pointer(void *);

			// Used to store the indicies of 'alloc'-'free' things
			std::index_t invalidAllocationIndex;
			std::index_t nextAllocationAvailable;
			std::STATIC_PAGE<void *, max_allocations> allocations;

			// Used to store the indicies of 'new'-'delete' things
			std::index_t invalidObjectIndex;
			std::index_t nextObjectAvailable;
			std::STATIC_PAGE<AbstractGCEntry *, max_objects> objects;
		};

		//extern ResourceManager progmem;
		//extern ResourceManager *activemem;
	}
}

#endif