#include "../include/ELSA/Processor.h"

BBP::elsa::keyword_return_t BBP::elsa::Processor_functor::operator() ()
{
	// If no functor is available, throw error
	if (action == nullptr)
		throw std::exception("No functor defined for keyword", ENODATA);

	// Otherwise return functor
	return action();
}