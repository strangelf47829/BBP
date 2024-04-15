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

		static constexpr size_t max_objects = 0x400;
		static constexpr size_t max_allocations = 0x400;

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
			GCEntry(T *p)
			{
				ptr = p;
			}
		};

		// Allows for dynamic object allocation using (new/delete) 
		class objectAllocator
		{
		public:

			objectAllocator();

			template<class T>
			inline T *Register(T *ptr)
			{
				// See if allocation is possible
				if (this->nextObjectAvailable == this->invalidObjectIndex)
					throw std::exception("Could not register new'd data to Resource manager: Out of allocation space.", ENOMEM);

				// Store that write into next possible object
				GCEntry<T> *entry = new GCEntry<T>(ptr);
				this->objects.data[this->nextObjectAvailable] = entry;

				// Find next possible location
				this->nextObjectAvailable = find_next_empty_object();

				// Return ptr, in case it is needed for something
				return ptr;
			}

			template<class T>
			inline void Delete(T *d)
			{
				// Dont delete nullptrs.
				if (d == nullptr)
					return;

				// Delete pointer if it was able to.
				_delete((void *)d); 
			}

			// Delete everything.
			std::size_t deleteAll();

		private:

			// Delete stuff
			bool _delete(void *);

			// Searches in the object page.
			std::index_t find_next_empty_object();

			// Searches in the 'new' 
			std::index_t find_Object_pointer(void *);

			// Used to store the indicies of 'new'-'delete' things
			std::index_t invalidObjectIndex;
			std::index_t nextObjectAvailable;
			std::STATIC_PAGE<AbstractGCEntry *, max_objects> objects;

			// Internal functions to quickly delete data at known indicies
			void _delete(std::index_t);

			// Delete stuff
			void mark_deleted(void *);

		};

		// Allows for dynamic page allocation using (malloc/calloc)
		class pageAllocator
		{
		public:

			pageAllocator();

			// Allocate stuff
			void *malloc(std::size_t);
			void *calloc(std::size_t, std::size_t);

			// Free something
			void free(void *);

			// Free Everything
			std::size_t freeAll();

		private:

			// Searches in the allocation page.
			std::index_t find_next_empty_alloc();

			// Searches in the 'allocations' 
			std::index_t find_Alloc_pointer(void *);

			// Used to store the indicies of 'alloc'-'free' things
			std::index_t invalidAllocationIndex;
			std::index_t nextAllocationAvailable;
			std::STATIC_PAGE<void *, max_allocations> allocations;

			// Internal functions to quickly free data at known indicies
			void free(void *, std::index_t);

			// Add an allocation
			void add_alloc(void *);

		};

		class ResourceManager
		{
		private:

			// Allocators
			pageAllocator pageManager;
			objectAllocator objectManager;

		public:

			// malloc stuff
			void *malloc(std::size_t);
			void *calloc(std::size_t, std::size_t);

			// Free stuff
			void free(void *);

			// Delete stuff
			void _delete(void *);

			// Add stuff
			template<class T>
			inline T *add_object(T *ptr)
			{
				return objectManager.Register(ptr);
			}

			// Free everything
			std::size_t freeAll();

			// Delete everything
			std::size_t deleteAll();

			// Complete clear everything
			std::size_t clearAll();

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

		};
	}
}

#endif