#ifndef BBP_ELSA_PROCESSOR_H
#define BBP_ELSA_PROCESSOR_H

#include "Keywords.h"

namespace BBP
{
	namespace elsa
	{

		// Define a signature for processor action
		using processor_action_t = keyword_return_t(*) ();

		// Define new class of keywords
		struct Processor_functor : public keyword_functor_t
		{

			// Keep a reference to function
			processor_action_t action;

			// Override
			keyword_return_t operator() () override;

			// Constructor
			Processor_functor(keyword_return_t(*act)()) : action(act) {}

			// Other constructor
			void *operator new(std::__cxx_size_t, Processor_functor *addr) { return addr; }
		};
		
		// Relates a list of functions to a list of strings
		class processor_db
		{
			// Page of functors
			std::PAGE<processor_action_t> actions;

			// Resource allocator used to allocate this page
			std::pageAllocator *allocator;

			// Where currently the actions are being whateverd
			std::index_t atAction;

			// Dictionary relating 
			std::HashedDictionary<std::string, std::index_t> actionIdx;

		public:

			// Constructor
			processor_db(std::pageAllocator *, std::size_t);

			// Register
			void Register(std::string, processor_action_t);

			// Reset database
			void Reset();

			// Indexers
			processor_action_t &operator[] (std::index_t);
			processor_action_t &operator[] (std::string);

		};


		// A keyword database
		class keyword_db
		{

			// Allocator
			std::pageAllocator *allocator;

			// Page of keywords
			std::PAGE<keyword_t> keywords;

			// Page of functors
			std::PAGE<Processor_functor> functors;

			// Keeps track of which index is currently being used
			std::index_t activeKeyword;

			// Which processor database is being used to retrieve functors
			processor_db processorDB;

		public:

			// constructor
			keyword_db(std::pageAllocator *all, std::size_t size, std::size_t processorSize);

			// Register something
			void Register(std::string, std::string);

			// Reset
			void Reset();

			// Indexers
			keyword_t &operator[] (std::index_t);

			// Get process DB
			processor_db &getProcessor();
		};


	}
}

#endif
