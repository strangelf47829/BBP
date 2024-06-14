#include "../Drivers/FileSystem/Filedrvhdr.h"

#include <fstream>

#define _HAS_CXX17
#include <filesystem>
namespace fs = std::filesystem;

// Iterator for inspection
fs::path directoryPath;
fs::directory_iterator directoryIterator;

// Modes
Host::Drivers::FileSystem::FileSystemMode Host::Drivers::FileSystem::mode;
Host::Drivers::FileSystem::FileSystemMode secondaryMode;

// Open an inspection
void Host::Drivers::FileSystem::openInspection()
{
	// Create path
	directoryPath = fs::path(activeFile.relName());

	// Create iterator
	directoryIterator = fs::directory_iterator(directoryPath);

	// Update inspector status

}

// Step an inspection
bool Host::Drivers::FileSystem::stepInspection()
{
	// Error code
	std::error_code ec;

	// Step over iterator
	directoryIterator.increment(ec);

	// Return false if error
	if (ec)
		return false;

	// Otherwise return true
	return true;
}

BBP::std::size_t Host::Drivers::FileSystem::nameSizeInspection()
{
	// Get size of string
	return BBP::std::strlen((BBP::std::c_string)directoryIterator->path().c_str());
}

int Host::Drivers::FileSystem::getEntityTypeInspection()
{
	// If does not exist return 0
	if (directoryIterator->exists() == false)
		return 0;

	// If normal file return 1
	if (directoryIterator->is_regular_file())
		return 1;

	// If directory return 2
	if (directoryIterator->is_directory())
		return 2;

	// Return 3 for unkown
	return 3;
}

BBP::std::size_t Host::Drivers::FileSystem::prepareNameEmission()
{
	// Save mode
	secondaryMode = mode;

	// set mode to inspect
	mode = FileSystemMode::INSPECT;

	// Return fie size
	return nameSizeInspection();
}

// 
void Host::Drivers::FileSystem::emitName(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &string)
{
	// Copy string
	BBP::std::strcpy(&string, (BBP::std::c_string)directoryIterator->path().c_str());

	// Write null-terminator
	string[amount] = 0;

	// Restore mode
	mode = secondaryMode;
}

bool Host::Drivers::FileSystem::inspectorStatus()
{
	// Return true if
	return (directoryIterator == std::filesystem::end(directoryIterator)) == false;
}