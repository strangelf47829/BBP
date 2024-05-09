#ifndef BBP_ELSA_KEYWORDS_H
#define BBP_ELSA_KEYWORDS_H

#include "../Strings.h"
#include "Processor.h"

namespace BBP
{
	namespace elsa
	{

		// A keyword is an abstract object that
		template <class T>
		using keyword_functor = void (T::)();

		// And now a keyword struct
		template <class T>
		struct keyword_t
		{
			
			keyword_functor<T> action = delete;

			// Calls the keyword
			void operator() () = delete;
		};
		
		// Own implementation of thingy
		struct myKeyword : public keyword_t<int>
		{

		};

	}
}

#endif