#ifndef BBP_ELSA_KEYWORDS_H
#define BBP_ELSA_KEYWORDS_H

#include "../Strings.h"
#include "../Dictionary.h"

namespace BBP
{
	namespace elsa
	{

		// Forward declare Translation Unit class
		class TranslationUnit;

		// Forward declare keyword_t
		struct keyword_t;

		// A keyword is an object that the lexer (or pretty much frontend) uses to interact with the processor
		// A keyword has two components: the functor and identifier. The identifier is used by the lexer to actually look for a keyword, the functor is used for interactions

		// This is the type used to return values back and forth between functors etc..
		using keyword_return_t = void;

		// Keyword identifier
		struct keyword_identifier_t
		{
			// Size of this identifier
			std::size_t identifierLength;

			// "mask" used by the lexer
			std::word mask;

			// The actual identifier
			std::string identifier;

			// The 'handle' of this identifier
			std::word handle;

			// Any flags used by the identifier
			struct keyword_flags
			{
				// Tokens are keywords that can be consumed even when infixed, prefixed, or suffixed.
				bool isToken;

				// If this keyword should also be forcibly parsed regardless of context scope
				bool forceParse;

				// A value of 'zero' has no effect. A non-zero value will signal that this keyword is a delimiter that starts a delimiting context with this value.
				// Keywords with the 'forceParse' effect will disregard this value
				std::byte delimitContext;

			} flags;

			// Constructor
			keyword_identifier_t(std::conststring);
			keyword_identifier_t(std::string);
		};

		// Abstract functor
		struct keyword_functor_t
		{

			// Virtual operator
			virtual keyword_return_t operator() (TranslationUnit &, std::index_t, keyword_t &) = 0;

		};

		// A keyword
		struct keyword_t
		{
			// own identifier.
			keyword_identifier_t identifier;

			// own functor (pointer since abstract)
			keyword_functor_t *functor;

			// Precomputed hash
			std::hash_t hashedIdentifier;

			// function to check for ..
			bool Advance(std::string_element);

			// Reset state
			void Reset();

			// Operator
			keyword_return_t operator() (TranslationUnit &, std::index_t, keyword_t &);

			// Constructor
			keyword_t(keyword_identifier_t, keyword_functor_t *);
		};

	}
}

#endif