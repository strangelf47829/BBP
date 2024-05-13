#include "../../include/FileDriver.h"
#include "../../include/Environment.h"

#include <fstream>

Environment::Drivers::Filesystem::FileSystemMode Environment::Drivers::Filesystem::mode;
BBP::std::PATH Environment::Drivers::Filesystem::activeFile("/mnt/v/");

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