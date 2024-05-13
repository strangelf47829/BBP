#include "../include/ELSA/Processor.h"

// Constructor
BBP::elsa::keyword_db::keyword_db(std::pageAllocator *all, std::size_t size, std::size_t processorSize)
	: processorDB(all, processorSize)
{
	// Check if allocator is valid
	if (all == nullptr)
		throw std::exception("Invalid argument: nullptr", EBADR);

	// Set allocator
	allocator = all;

	// Allocate keywords and functors
	keywords = std::PAGE<keyword_t>(size, (keyword_t *)all->calloc(size, sizeof(keyword_t)));
	functors = std::PAGE<Processor_functor>(size, (Processor_functor *)all->calloc(size, sizeof(Processor_functor)));

	// Reset indexer
	activeKeyword = 0;
}

// Getter
BBP::elsa::processor_db &BBP::elsa::keyword_db::getProcessor()
{
	return processorDB;
}

void BBP::elsa::keyword_db::Register(std::string keyw, std::string func)
{
	// Constructor
	new (&functors[activeKeyword]) Processor_functor(processorDB[func]);

	// Constructor
	keywords[activeKeyword] = keyword_t(keyw, &functors[activeKeyword]);

	// Increment active keyword
	activeKeyword++;
}

void BBP::elsa::keyword_db::Reset()
{
	// If allocator exists, and there is still data
	if (allocator && keywords.data)
	{
		// Free data
		allocator->free(keywords.data);
		keywords = std::PAGE<keyword_t>();
	}

	// If allocator exists, and there is still data
	if (allocator && functors.data)
	{
		// Free data
		allocator->free(functors.data);
		functors = std::PAGE<Processor_functor>();
	}

	// Reset processor
	processorDB.Reset();
}

BBP::elsa::keyword_t &BBP::elsa::keyword_db::operator[] (std::index_t idx)
{
	return keywords[idx];
}