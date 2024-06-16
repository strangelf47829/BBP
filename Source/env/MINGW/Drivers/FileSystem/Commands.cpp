#include "../../../include/FileSystemDriver.h"
#include "../../../../include/drvcmd.h"
#include "Filedrvhdr.h"

bool Host::Drivers::FileSystem::initializeFilesystem(BBP::std::size_t, BBP::std::word *)
{
	return true;
}

bool Host::Drivers::FileSystem::deinitializeFilesystem(BBP::std::size_t, BBP::std::word *)
{
	return true;
}

bool Host::Drivers::FileSystem::setModeLoadPath(BBP::std::size_t, BBP::std::word *)
{
	// Set mode into READPATH
	mode = READPATH;

	// Return true
	return true;
}

bool Host::Drivers::FileSystem::queryFileMetadata(BBP::std::size_t argc, BBP::std::word *argv)
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

bool Host::Drivers::FileSystem::getFileType(BBP::std::size_t argc, BBP::std::word *argv)
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

bool Host::Drivers::FileSystem::openFile(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Do something based on mode
	switch (mode)
	{
	case FileSystemMode::READ:
		openFileReading();
		return true;
	case FileSystemMode::WRITE:
		openFileWriting();
		return true;
	}

	// Something went wrong.
	return false;
}

bool Host::Drivers::FileSystem::closeFile(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Do something based on mode
	switch (mode)
	{
	case FileSystemMode::READ:
		closeFileReading();
		return true;
	case FileSystemMode::WRITE:
		closeFileWriting();
		return true;
	}

	// Something went wrong.
	return false;
}

bool Host::Drivers::FileSystem::setReadMode(BBP::std::size_t argc, BBP::std::word *argv)
{
	mode = READ;
}

bool Host::Drivers::FileSystem::setWriteMode(BBP::std::size_t argc, BBP::std::word *argv)
{
	mode = WRITE;
}

bool Host::Drivers::FileSystem::pathInspect(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Open up an inspection.
	openInspection();

	// Done
	return true;
}

bool Host::Drivers::FileSystem::stepInspect(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Step iterator
	bool valid = stepInspection();

	// If argc and argv, set first argument to value
	if (argc && argv)
		argv[0] = valid;

	// Return if could step
	return valid;
}

bool Host::Drivers::FileSystem::emitNameInspect(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Tell the driver to prepare. also save
	BBP::std::size_t nameSize = prepareNameEmission();

	// If argc and argv, write
	if (argc && argv)
		argv[0] = nameSize;

	// return true
	return true;
}

bool Host::Drivers::FileSystem::emitTypeInspect(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Tell the driver to prepare. also save
	int type = getEntityTypeInspection();

	// If argc and argv, write
	if (argc && argv)
		argv[0] = type;

	// return true
	return true;
}

bool Host::Drivers::FileSystem::canStepInspect(BBP::std::size_t argc, BBP::std::word *argv)
{
	// Get inspector status
	bool status = inspectorStatus();

	// If argc and argv, set
	if (argc && argv)
		argv[0] = status;

	// Success
	return true;
}