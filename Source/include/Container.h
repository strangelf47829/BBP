#ifndef BBP_STD_CONTAINER_H
#define BBP_STD_CONTAINER_H

#include "stddef.h"

namespace BBP
{
	namespace std
	{
		// Empty specialization
		template<typename... T>
		struct Container
		{};

		// Compile time heterogenous list
		template<typename T, typename... K>
		struct Container<T, K...>
		{
			// Constructor
			Container(T &v, K& ... trail)
				: value(v),
				trailing(trail...),
				idx(sizeof...(K))
			{}

			// Default constructor
			Container()
				: value(T()),
				trailing(),
				idx(sizeof...(K))
			{}

			// Declare a getter
			template<std::index_t P, typename L>
			struct ListGetter;

			// Declare a specialized getter
			template<typename P, typename... L>
			struct ListGetter<0, Container<P, L...>>
			{
			public:

				// Get object as-is
				static T &get(Container<P, L...> &list)
				{
					return list.value;
				}

				// Get pointer to object
				static void *getPtr(Container<P, L...> &list)
				{
					return &(list.value);
				}

			};

			// Generic getter
			template<std::index_t S, typename P, typename... L>
			struct ListGetter<S, Container<P, L...>>
			{
			public:

				// Get object at other index
				static auto &get(Container<P, L...> &list)
				{
					return ListGetter<S - 1, Container<L...>>::get(list.trailing);
				}

				// Get pointer of other index
				static void *getPtr(Container<P, L...> &list)
				{
					return ListGetter<S - 1, Container<L...>>::getPtr(list.trailing);
				}
			};

			// The value being stored
			T value;

			// Trailing values
			Container<K...> trailing;

			// The index of this list object
			std::index_t idx;

			// Object Getter
			template<std::index_t index>
			auto &get()
			{
				return ListGetter<index, Container<T, K...>>::get(*this);
			}

			// Pointer getter
			template<std::index_t index>
			void *getPtr()
			{
				return ListGetter<index, Container<T, K...>>::getPtr(*this);
			}

		};
	}
}

#endif