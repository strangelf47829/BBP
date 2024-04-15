#ifndef BBP_ELSA_KEYWORDS_H
#define BBP_ELSA_KEYWORDS_H

#include "../Strings.h"
#include "Processor.h"

namespace BBP
{
	namespace elsa
	{

		// Structure used by the lexer to convey information to a keyword functor
		struct KeywordFunctor_args_t
		{

		};

		// Definition for a keyword functor
		using KeywordFunctor_t = void (KeywordFunctor_args_t &);		

		// A keyword as defined by the frontend
		struct Keyword_t
		{
			// The text used to define this keyword
			std::string keyword;

			// Length of the keyword (used in case null-termination isn't possible, such as when reading files etc..)
			std::size_t keywordLength;

			// The index of the action associated with this keyword, as provided by the frontend.
			std::index_t actionIndex;

			// Various flags related to the 

		};

		// A keyword as used by the lexer
		struct KeywordHandle_t
		{

			// 
			void tes()
			{
				Keyword_t t;
				esaProcessor proces;
				t.functor = proces.call;
			}

		};

	}
}

#endif


class KeywordAction
{
	virtual void call() = 0;
};

class SomeClass 
{

};

template<class T>
class GenericKeywordActionIDKGOTOSLEEPYOUFUCKINGUSELESSCUNTTTTT : public KeywordAction
{
	// Template definition
	using KeywordFunctor_t = void (T::*)();

	KeywordFunctor_t functor;

	void call() override
	{

	}

};