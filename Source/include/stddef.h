#ifndef BBP_STDLIB_STDDEF_H
#define BBP_STDLIB_STDDEF_H

#include "stdint.h"
#include "Limits.h"
#include "Architectures.h"

#define __UNSAFE__(func) func##_unsafe

namespace BBP
{
	namespace std
	{

		/* Use this type when you explicitly mean 32 bits */
		typedef uint32_t word;

		/* Use this type when you explicitly mean 32 signed bits. Also has a cool name. */
		typedef int32_t sword;

#ifndef USE_32_BITS
		/* Use this type when you explicitly mean 64 bits */
		/* Type unavailable in 32 bit environments */
		typedef uint64_t dword;
#endif

		/* Use this type when you explicitly mean 16 bits */
		typedef uint16_t halfword;

		/* Use this type when you explicitly mean 8 bits */
		typedef uint8_t byte;

		/* Use this type when referring to 4 bits explicitly */
		typedef uint8_t nibble;

		/* Use this type when referring to 2 bits */
		typedef uint8_t crumb;

		/* Use this type when referring to an offset of an object */
		typedef word offset_t;

		/* Use this type when referring to a signed offset of an object */
		typedef sword s_offset_t;

		/* Use this type when referring to an absolute memory address */
		typedef word address_t;

		/* Use this type when referring to an index of an object */
		typedef word index_t;

		/* Use this type when referring to the size of something */
		typedef word size_t;

		/* Use this type when referring to the smallest possible division of memory in streams. */
		typedef byte mem_t;

		// Arithmetic concept
		template<typename T>
		concept Arithmetic = requires (T a)
		{
			(int)a + (int)a;
		};

	}
}

#endif 

