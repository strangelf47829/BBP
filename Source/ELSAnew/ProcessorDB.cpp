#include "../include/ELSA/Processor.h"

// Constructor
BBP::elsa::processor_db::processor_db(std::pageAllocator *all, std::size_t size)
{
	// Check if allocator is valid
	if (all == nullptr)
		throw std::exception("Invalid argument: nullptr", EBADR);

	// Set allocator
	allocator = all;

	// Allocate amount
	actions = std::PAGE<processor_action_t>(size, (processor_action_t *)all->calloc(size, sizeof(processor_action_t)));

	// Reset indexer
	atAction = 0;
}

// Register something
void BBP::elsa::processor_db::Register(std::string str, processor_action_t action)
{
	// Store action in atAction
	actions[atAction] = action;

	// Store that index in indexes
	actionIdx.add(str, atAction);

	// Increment atAction
	atAction++;
}

// Reset
void BBP::elsa::processor_db::Reset()
{
	// If allocator and data exists,
	if (allocator && actions.data)
	{
		// Free data
		allocator->free(actions.data);
		actions = std::PAGE<processor_action_t>();
	}

	// Reset atAction
	atAction = 0;

	// Reset dictionary
	actionIdx.Reset();
}

BBP::std::size_t BBP::elsa::keyword_db::getCount()
{
	return keywordCount;
}

// Access information
BBP::elsa::processor_action_t &BBP::elsa::processor_db::operator[] (std::index_t idx)
{
	return actions[idx];
}

BBP::elsa::processor_action_t &BBP::elsa::processor_db::operator[] (std::string str)
{
	return actions[actionIdx[str]];
}