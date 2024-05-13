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

bool Environment::Drivers::Filesystem::setModeLoadPath(BBP::std::size_t, BBP::std::word *)
{
	// Set mode into READPATH
	mode = READPATH;

	// Return true
	return true;
}

bool Environment::Drivers::Filesystem::queryFileMetadata(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If no arguments specified, return true
	if (argc == 0)
		return true;

	// If argv is nullptr, return false
	if (argv == nullptr)
		return false;

	// Get file status if requesting at least one argument
	if (argc >= 1)
		argv[0] = !!(doesPathExist());

	// Get file size if requesting at least two arguments. Also check if file actually exists
	if (argc >= 2 && argv[0])
		argv[1] = getFileSize();

	// Return true
	return true;
}