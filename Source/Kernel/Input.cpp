#include "../include/Kernel.h"
#include "../include/drvcmd.h"

// Wait until key is available, then retrieve
BBP::std::string_element BBP::system::Kernel::getKey()
{
	// Core implemented
	return singleton.Core().firmware->getKey();
}

// Retrieve key anyway
BBP::std::string_element BBP::system::Kernel::getKeyAsync()
{
	// Core implemented
	return singleton.Core().firmware->getKeyAsync();
}