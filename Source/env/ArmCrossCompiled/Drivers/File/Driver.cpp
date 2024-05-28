#include "../../include/FileDriver.h"
#include "../../include/Environment.h"

#include <fstream>

#define _HAS_CXX17
#include <filesystem>
namespace fs = std::filesystem;

Environment::Drivers::Filesystem::FileSystemMode Environment::Drivers::Filesystem::mode;
Environment::Drivers::Filesystem::FileSystemMode secondaryMode;
BBP::std::PATH Environment::Drivers::Filesystem::activeFile("/mnt/v/");

// Reading
std::ifstream readStream;
std::ofstream writeStream;

// Iterator for inspection
fs::path directoryPath;
fs::directory_iterator directoryIterator;

// Secondary mode

bool Environment::Drivers::Filesystem::doesPathExist()
{
	// Open fstream
	::std::ifstream _file(activeFile.relName());

	// Store file isopen
	bool exists = _file.is_open();

	// Close file
	_file.close();

	// Return result
	return exists;
}


BBP::std::word Environment::Drivers::Filesystem::getFileSize()
{
	// Open fstream
	::std::ifstream _file(activeFile.relName());

	// Check if file is open. If it is not cause a kernel panic
	if (!_file.is_open())
		BBP::std::panic();

	// Get bytecount
	_file.seekg(0, ::std::ios::end);
	BBP::std::size_t size = _file.tellg();
	_file.seekg(0, ::std::ios::beg);

	// Close file
	_file.close();

	// Return bytecount
	return size;
}

bool Environment::Drivers::Filesystem::isDirectory()
{
	// Get C string representation
	BBP::std::c_string pathName = activeFile.relName();

	// Then get path
	fs::path path(pathName);

	// Then return is directory
	return fs::is_directory(path);
}

bool Environment::Drivers::Filesystem::isFile()
{
	// Get C string representation
	BBP::std::c_string pathName = activeFile.relName();

	// Then get path
	fs::path path(pathName);

	// Then return is file
	return fs::is_regular_file(path);
}

bool Environment::Drivers::Filesystem::doesEntityExist()
{
	// Get C string representation
	BBP::std::c_string pathName = activeFile.relName();

	// Then get path
	fs::path path(pathName);

	// Then return is file
	return fs::exists(path);
}

// Open for reading
void Environment::Drivers::Filesystem::openFileReading()
{
	// Open stream
	readStream = std::ifstream(activeFile.relName(), ::std::ios_base::binary);
}

// Close for reading
void Environment::Drivers::Filesystem::closeFileReading()
{
	// Close stream
	readStream.close();
}

// Open for writing
void Environment::Drivers::Filesystem::openFileWriting()
{
	// Open stream
	writeStream = std::ofstream(activeFile.relName(), ::std::ios::out | ::std::ios_base::binary);
}

// Close for writing
void Environment::Drivers::Filesystem::closeFileWriting()
{
	// Close stream
	writeStream.close();
}

// Write a file into
void Environment::Drivers::Filesystem::writeFile(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &data)
{
	// Write a file
	for (BBP::std::index_t idx = 0; idx < amount; idx++)
		writeStream << data[idx];
}

// Read a character
bool Environment::Drivers::Filesystem::readCharacter(char &c)
{
	// Stupid workaround ?
	if (readStream.get(c))
		return true;
	return false;
}

// Open an inspection
void Environment::Drivers::Filesystem::openInspection()
{
	// Create path
	directoryPath = fs::path(activeFile.relName());

	// Create iterator
	directoryIterator = fs::directory_iterator(directoryPath);

	// Update inspector status

}

// Step an inspection
bool Environment::Drivers::Filesystem::stepInspection()
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

BBP::std::size_t Environment::Drivers::Filesystem::nameSizeInspection()
{
	// Get size of string
	return BBP::std::strlen((BBP::std::c_string)directoryIterator->path().c_str());
}

int Environment::Drivers::Filesystem::getEntityTypeInspection()
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

BBP::std::size_t Environment::Drivers::Filesystem::prepareNameEmission()
{
	// Save mode
	secondaryMode = mode;

	// set mode to inspect
	mode = FileSystemMode::INSPECT;

	// Return fie size
	return nameSizeInspection();
}

// 
void Environment::Drivers::Filesystem::emitName(BBP::std::size_t amount, BBP::std::PAGE<BBP::std::string_element> &string)
{
	// Copy string
	BBP::std::strcpy(&string, (BBP::std::c_string)directoryIterator->path().c_str());

	// Restore mode
	mode = secondaryMode;
}

bool Environment::Drivers::Filesystem::inspectorStatus()
{
	// Return true if
	return (directoryIterator == std::filesystem::end(directoryIterator)) == false;
}