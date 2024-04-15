#ifndef BBP_STD_CONCEPTS_H
#define BBP_STD_CONCEPTS_H

#include "Hashing.h"

namespace BBP
{
	namespace std
	{

		// Check if something can be hashed an compared.
		template<typename T>
		concept Identifiable = requires (T & a, T & b)
		{
			// Must be hashable.
			Hashable_t<T>;

			// Must be comparable
			(bool)(a == b);
			(bool)(a != b);

			// Must have an empty constructor
			new T();
		};

	}
}

#endif