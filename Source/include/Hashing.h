#ifndef BBP_STD_HASH_H
#define BBP_STD_HASH_H

#include "stddef.h"

namespace BBP
{
	namespace std
	{
		/* Type of a hash */
		typedef uint32_t hash_t;

		// Empty hash
		constexpr hash_t emptyHash = 0;

		// Hashing template
		template<typename T>
		concept Hashable_t = requires (T & a)
		{
			(hash_t)a;
		};

		// For each hashable thing
		template<Hashable_t T>
		static inline hash_t hash(T &&a)
		{
			// Get hash
			hash_t hash = (hash_t)a;

			// If hash is emptyHash, return that plus one
			if (hash == emptyHash)
				hash++;

			return hash;
		}

	}
}

#endif
