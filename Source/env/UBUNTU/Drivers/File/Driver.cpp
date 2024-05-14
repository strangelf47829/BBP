#include "../../include/FileDriver.h"
#include "../../include/Environment.h"

#include <fstream>

#define _HAS_CXX17
#include <filesystem>
namespace fs = std::filesystem;

Environment::Drivers::Filesystem::FileSystemMode Environment::Drivers::Filesystem::mode;
BBP::std::PATH Environment::Drivers::Filesystem::activeFile("/mnt/v/");

// Reading
std::ifstream readStream;

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
	// Open stream
	readStream.close();
}

// Read a character
bool Environment::Drivers::Filesystem::readCharacter(char &c)
{
	// Stupid workaround ?
	if (readStream.get(c))
		return true;
	return false;
}