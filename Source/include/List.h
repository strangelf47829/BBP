#ifndef BBP_STD_LIST_H
#define BBP_STD_LIST_H

#include "Container.h"

namespace BBP
{
	namespace std
	{

		// Declare List
		template<typename... T>
		struct List
		{};


		// List with more than one element
		template<typename T, typename... K>
		struct List<T,K...>
		{
			// Wrapper for Container
			List(T first, K... others)
				: cont(first, others...)
			{}

			// Default constructor
			List()
			{}

			// Object Getter
			template<std::index_t index>
			auto &get()
			{
				return cont.template get<index>();
			}

			// Pointer getter
			template<std::index_t index>
			void *getPtr()
			{
				return cont.template getPtr<index>();
			}

		private:
			Container<T, K...> cont;
		
		};

	}
}

#endif