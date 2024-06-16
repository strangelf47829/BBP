#include "../../include/FileDriver.h"
#include "../../include/Environment.h"

bool Environment::Drivers::Filesystem::initializeFilesystem(BBP::std::size_t, BBP::std::word *)
{
	return true;
}

bool Environment::Drivers::Filesystem::deinitializeFilesystem(BBP::std::size_t, BBP::std::word *)
{
	return true;
}