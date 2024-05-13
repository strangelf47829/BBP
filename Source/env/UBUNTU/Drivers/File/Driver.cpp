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