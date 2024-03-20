#ifndef BBP_STDLIB_MEMORY_H
#define BBP_STDLIB_MEMORY_H

#include "stddef.h"

#define STATIC_STACK(type, identifier) static BBP::std::STATIC_PAGE<type> identifier##PAGE;\
BBP::std::Stack<type> identifier(&identifier##PAGE);

#define STATIC_STRSTACK(identifier) static BBP::std::static_string identifier##PAGE;\
static BBP::std::Stack<BBP::std::string_element> identifier(&identifier##PAGE);


namespace BBP
{
	namespace std
	{
		/* The maximum amount of things in a page, period. */
		constexpr word max_page_elements = 10000;

		/* The maximum amount of things in a page which is of type non-ptr. */
		constexpr word max_page_static_size = 10000;

		/* The maximum amount of things in a page which is of pointer types. */
		constexpr word max_page_pointer_size = max_page_elements;

		/* The maximum length of one page sequence */
		constexpr word max_page_sequence_length = 64;

		/* The maximum amount of things in a long page which is of type non-ptr. */
		constexpr word max_lpage_static_size = 0x2000;

		template <typename T>
		struct PAGE
		{
			size_t dataSize;
			size_t bytes;

			PAGE<T> *prevPage;
			PAGE<T> *nextPage;

			union {
				mem_t *raw;
				T *data;
			};

			PAGE(size_t Size, T* dat) : dataSize(Size), bytes(Size * (sizeof(T) / sizeof(mem_t))), prevPage(nullptr), nextPage(nullptr), data(dat) {}
			PAGE() : dataSize(0), bytes(0), prevPage(0), nextPage(0), data(nullptr) {}

			T &operator[](std::index_t idx);

		};

		template <typename T, int K>
		struct STATIC_PAGE : virtual PAGE<T>
		{
			T static_data[K];

			STATIC_PAGE() : PAGE<T>::PAGE(K, static_data) 
			{
				//static_assert(K <= max_page_static_size, "Static page cannot hold more than max_page_static_size items!");
			}
		};

		// The dynamic page stores a page to either a 2, 4, 8, 16, or 32 bits 
		struct DYN_PAGE : virtual PAGE<std::word>
		{
			// This struct is used to store the alignment types used.
			enum Alignment : std::byte
			{
				Unallocated = 0,
				Crumb = 2,
				Nibble = 4,
				Byte = 8,
				HalfWord = 16,
				Word = 32
			} alignment;
			
			// Helper function to calculate a bunch of stuff
			void calculateIndicies(std::index_t, std::index_t &bottom, std::index_t &top, std::offset_t &bitOffset);

			// '[]' is used to read, function is used to write
			std::word operator[](std::index_t idx);
			void write(std::index_t, std::word);

			// Resize pages dynamically
			Alignment getAlignment();
			void setAlignment(Alignment);

			// Used to manipulate data within this page
			std::word getMask();
			std::word getMask(std::byte);
			std::word truncate(std::word);
			std::word truncate(std::word, std::byte);


			// Delete old constructors since they are no longer valid.
			DYN_PAGE(std::size_t, std::word *) = delete;

			// Specify new constructors
			DYN_PAGE(std::size_t size, std::word *, Alignment align, bool WPerm);
			DYN_PAGE();

			bool canUserWrite;

		};
		
		// Write 'data' at 'index'
		template <typename T>
		void write(PAGE<T> *page, T &data, index_t index);
		
		// Write 'data' at 'index' (as raw).
		template <typename T>
		void write_r(PAGE<T> *page, mem_t &data, index_t index);
		
		// Write 'amount' amount of things from 'data' into 'page' at index 'index'.
		template <typename T>
		void write_a(PAGE<T> *page, T *data, index_t index, size_t amount);

		// Write 'amount' amount of raw data from 'data' into 'page' at index 'index'.
		template <typename T>
		void write_ar(PAGE<T> *page, mem_t *data, index_t index, size_t amount);

		// Write 'data' at 'index'. Does not perform checks.
		template <typename T>
		inline void __UNSAFE__(write)(PAGE<T> *page, T &data, index_t index);

		// Write 'amount' amount of things from 'data' into 'page' at index 'index'. Does not perform checks.
		template <typename T>
		inline void __UNSAFE__(write_a)(PAGE<T> *page, T *data, index_t index, size_t amount);

		// Write 'data' at 'index' (as raw). Does not perform checks.
		template <typename T>
		inline void __UNSAFE__(write_r)(PAGE<T> *page, mem_t &data, index_t index);




		// Read one element from page at index
		template <typename T>
		T &read(PAGE<T> *page, index_t index);

		// Read one raw byte from page at index
		template <typename T>
		mem_t &read_r(PAGE<T> *page, index_t index);

		// Return pointer to data stored at index
		template <typename T>
		T *read_a(PAGE<T> *page, index_t index);

		// Return pointer to raw data stored at index
		template <typename T>
		mem_t *read_ar(PAGE<T> *page, index_t index);

		// Read one element from page at index
		template <typename T>
		inline T &__UNSAFE__(read)(PAGE<T> *page, index_t index);

		// Read one raw byte from page at index
		template <typename T>
		inline mem_t &__UNSAFE__(read_r)(PAGE<T> *page, index_t index);

		// Return pointer to data stored at index
		template <typename T>
		inline T *__UNSAFE__(read_a)(PAGE<T> *page, index_t index);

		// Return pointer to raw data stored at index
		template <typename T>
		inline mem_t *__UNSAFE__(read_ar)(PAGE<T> *page, index_t index);


		// Fetches the last page in the list
		template<typename T>
		std::PAGE<T> &last(std::PAGE<T> &);

		// Fetches the first page in the list
		template<typename T>
		std::PAGE<T> &first(std::PAGE<T> &);


		// Returns the total size of a page sequence (even if page is in the middle of sequence).
		template <typename T>
		size_t seqlen(PAGE<T> &page);

		// Returns the total raw size of a page sequence (even if page is in the middle of sequence).
		template <typename T>
		size_t seqlen_r(PAGE<T> &page);

		// Returns the total size of a page sequence (even if page is in the middle of sequence).
		template <typename T>
		size_t __UNSAFE__(seqlen)(PAGE<T> &page);

		// Returns the total raw size of a page sequence (even if page is in the middle of sequence).
		template <typename T>
		size_t __UNSAFE__(seqlen_r)(PAGE<T> &page);



		// Copy 'amount' amount of bytes from 'src' to 'dst'.
		template <typename T>
		void memcpy(PAGE<T> &src, PAGE<T> &dst, size_t amount);

		// Copy all bytes from 'src' to 'dst'.
		template <typename T>
		inline void memcpy(PAGE<T> &src, PAGE<T> &dst)
		{
			// Get sequence length of src
			std::size_t srcLen = std::seqlen(src);
			std::size_t dstLen = std::seqlen(dst);

			memcpy(src, dst, (srcLen < dstLen) ? srcLen : dstLen);
		}

		// Compare all bytes in 'src' and 'dst'.
		template <typename T>
		bool memcmp(PAGE<T> &src, PAGE<T> &dst);

	}
}

#include "Memory.hxx"

#endif