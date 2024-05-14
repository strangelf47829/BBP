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

bool Environment::Drivers::Filesystem::getFileType(BBP::std::size_t argc, BBP::std::word *argv)
{
	// If argc is 0 or argv is nullptr return false
	if (argc == 0 || argv == nullptr)
		return false;

	// If does not exist then return none
	if (doesEntityExist() == false)
		argv[0] = 0; // NONE

	// If file exists return file
	else if (isFile())
		argv[0] = 1;

	// If directory exists return directory
	else if (isDirectory())
		argv[0] = 2;

	// Otherwise unkown
	else
		argv[0] = 3;

	// Success
	return true;
}

bool Environment::Drivers::Filesystem::openFile(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Do something based on mode
	switch (mode)
	{
	case FileSystemMode::READ:
		openFileReading();
		return true;
	}

	// Something went wrong.
	return false;
}

bool Environment::Drivers::Filesystem::closeFile(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Do something based on mode
	switch (mode)
	{
	case FileSystemMode::READ:
		closeFileReading();
		return true;
	}

	// Something went wrong.
	return false;
}

bool Environment::Drivers::Filesystem::setReadMode(BBP::std::size_t argc, BBP::std::word *argv)
{
	mode = READ;
}

bool Environment::Drivers::Filesystem::setWriteMode(BBP::std::size_t argc, BBP::std::word *argv)
{
	mode = WRITE;
}