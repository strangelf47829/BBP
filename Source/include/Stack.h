#ifndef BBP_STDLIB_STACK_H
#define BBP_STDLIB_STACK_H

#include "Memory.h"

namespace BBP
{
	namespace std
	{
		
		class ResourceManager;

		template <typename K>
		class Stack
		{
		public:
			
			// Constructors
			Stack(PAGE<K> *_page) : page(_page), max_elements(std::seqlen(*_page)), atElement(0), readFrom(nullptr), writeTo(nullptr) {}
			Stack(PAGE<K> *_page, size_t max) : page(_page), max_elements(max), atElement(0), readFrom(nullptr), writeTo(nullptr) {}

			// Static constructors
			template<int N>
			Stack(STATIC_PAGE<K, N> *_page) : page(_page), max_elements(std::seqlen(*_page)), atElement(0), readFrom(nullptr), writeTo(nullptr) {}
			template<int N>
			Stack(STATIC_PAGE<K, N> *_page, size_t max) : page(_page), max_elements(max), atElement(0), readFrom(nullptr), writeTo(nullptr) {}


			// Read and write 'k' types safely.
			Stack<K> &operator<<(K value); // Write one K
			Stack<K> &operator>>(K &value); // Read one K

			// Read and write 'k' types without safety.
			Stack<K> &operator<=(K value); // Write one K
			Stack<K> &operator>=(K &value); // Read one K

			// Read and write one byte from one stream to another
			Stack<K> &operator>>(Stack<K> &);
			Stack<K> &operator<<(Stack<K> &);

			// Peek one value 'k'
			K &operator+=(std::s_offset_t offset); // Has safety
			K &operator%=(std::s_offset_t offset); // No safety

			// Return pointer to obj
			K &operator[](std::index_t);

			Stack<K> &operator++();
			Stack<K> &operator--();

			inline Stack<K> &operator++(int a)
			{
				a = a;
				this->operator++();
				return *this;
			}
			inline Stack<K> &operator--(int a)
			{
				a = a;
				this->operator--();
				return *this;
			}

			PAGE<K> *page;

			size_t max_elements;
			index_t atElement;

			void Extend(std::ResourceManager *, size_t);

			void (*readFrom)(Stack<K> *);
			void (*writeTo)(Stack<K> *);

			using Iterator = void (*)(K &);
			using Predicate = bool (*)(K &);

			void foreach(Iterator);
			bool all(Predicate);
			bool any(Predicate);



		};

	}
}

#include "Stack.hxx"

#endif